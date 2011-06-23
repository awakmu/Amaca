/*
 * Template engine Lua-based in C
 *
 * Copyright (c) 2011, Alessandro Ghedini <al3xbio@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define TMPL_START "{{"
#define TMPL_END   "}}"

#define check_value(x) assert(x);

char *Amaca_template(const char *template, int nargs, ...);
char *Amaca_template_file(const char *filename, int nargs, ...);

static char *lua_exec(char *code, int nargs, va_list args);
static char *eval_template(const char *template, int nargs, va_list args);
static char *str_replace(char *orig, char *str, char *start, char *end);

char *Amaca_template(const char *template,  int nargs, ...) {
	char *ret;
	va_list args;

	/* eval template with proper args */
	va_start(args, nargs);
	ret = eval_template(template, nargs, args);
	va_end(args);

	return ret;
}

char *Amaca_template_file(const char *filename,  int nargs, ...) {
	va_list args;
	size_t fd_size;
	char *str, *ret;

	FILE *fd = fopen(filename, "rb");
	check_value(fd);

	/* read template file into memory */
	fseek(fd, 0, SEEK_END);
	fd_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	str = calloc(fd_size + 1, 1);
	check_value(str);

	fread(str, sizeof(char), fd_size, fd);

	fclose(fd);

	/* eval template with proper args */
	va_start(args, nargs);
	ret = eval_template(str, nargs, args);
	va_end(args);

	free(str);

	return ret;
}

static char *eval_template(const char *template, int nargs, va_list args) {
	char *start, *end;
	char *index = calloc(strlen(template) + 1, 1);
	check_value(index);

	index = strcpy(index, template);

	while ((start = strstr(index, TMPL_START)) != NULL) {
		size_t block_len;
		char *tmpl, *block, *nindex;

		/* extract block of code */
		end = strstr(start, TMPL_END)+2;

		block_len = (end - 2) - (start + 2);
		block = calloc(block_len + 1, 1);
		check_value(block);

		block = memcpy(block, start + 2, block_len);
		check_value(block);

		tmpl = lua_exec(block, nargs, args);

		/* replace code block with its output */
		nindex = str_replace(index, tmpl, start, end);

		free(index);
		free(block);

		index = nindex;
	}

	return index;
}

static char *str_replace(char *orig, char *str, char *start, char *end) {
	char *result = NULL;

	size_t src_len = strlen(orig);
	size_t str_len = strlen(str);
	size_t sub_len = end - start;

	result = calloc((src_len - sub_len) + str_len + 1, 1);
	check_value(result);

	result = strncpy(result, orig, start - orig);
	check_value(result);

	result = strncat(result, str, str_len);
	check_value(result);

	result = strncat(result, end, (orig + src_len) - end);
	check_value(result);

	return result;
}

static char *lua_exec(char *code, int nargs, va_list args) {
	int tmp, i;
	char *tmpl;
	va_list args_c;

	/* initialize lua state */
	lua_State *l = luaL_newstate();
	luaL_openlibs(l);

	/*
	 * pass arguments to lua
	 * use a copy of args because it is reused
	 */
	va_copy(args_c, args);

	for (i = 0; i < nargs; i++) {
		char *key, *val;
		char *key_c, *val_c;

		key = va_arg(args_c, char *);
		val = va_arg(args_c, char *);

		if (val == NULL) {
			break;
		}

		key_c = calloc(strlen(key) + 1, 1);
		key_c = strcpy(key_c, key);

		val_c = calloc(strlen(val) + 1, 1);
		val_c = strcpy(val_c, val);

		lua_pushstring(l, val_c);
		lua_setglobal(l, key_c);
	}

	va_end(args_c);

	/* execute code block */
	tmp = luaL_dostring(l, code);
	check_value(tmp == 0);

	/* extract code block result */
	tmpl = calloc(lua_strlen(l, -1) + 1, 1);

	tmpl = strcpy(tmpl, lua_tostring(l, -1));

	lua_settop(l, 0);
	lua_close(l);

	return tmpl;
}
