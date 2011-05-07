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

#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define TMPL_START "{{"
#define TMPL_END   "}}"

#define check_value(x) if (x == NULL) return NULL;

char *Amaca_template(const char *template, ...);
char *Amaca_template_file(const char *filename, ...);

static char *lua_exec(char *code, va_list args);
static char *eval_template(const char *template, va_list args);
static char *str_replace(char *orig, char *str, char *start, char *end);

char *Amaca_template(const char *template, ...) {
	char *ret;
	va_list args;

	/* eval template with proper args */
	va_start(args, template);
	ret = eval_template(template, args);
	va_end(args);

	return ret;
}

char *Amaca_template_file(const char *filename, ...) {
	va_list args;
	size_t fd_size;
	char *str, *ret;

	FILE *fd = fopen(filename, "rb");
	check_value(fd);

	/* read template file into memory */
	fseek(fd, 0, SEEK_END);
	fd_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	str = (char *) malloc(fd_size + 1);
	check_value(str);

	fread(str, sizeof(char), fd_size, fd);

	fclose(fd);

	/* eval template with proper args */
	va_start(args, filename);
	ret = eval_template(str, args);
	va_end(args);

	free(str);

	return ret;
}

static char *eval_template(const char *template, va_list args) {
	char *start, *end;
	char *index = (char *) malloc(strlen(template) + 1);
	check_value(index);

	index = strcpy(index, template);

	while ((start = strstr(index, TMPL_START)) != NULL) {
		size_t block_len;
		char *tmpl, *block, *nindex;

		/* extract block of code */
		end = strstr(start, TMPL_END)+2;

		block_len = (end - 2) - (start + 2);
		block = (char *) malloc(block_len + 1);
		check_value(block);

		block = memcpy((char *) block, start + 2, block_len);
		check_value(block);

		tmpl = lua_exec(block, args);

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

	result = (char *) malloc((src_len - sub_len) + str_len);
	check_value(result);

	result = strncpy(result, orig, start - orig);
	check_value(result);

	result = strncat(result, str, str_len);
	check_value(result);

	result = strncat(result, end, (orig + src_len) - end);
	check_value(result);

	return result;
}

static char *lua_exec(char *code, va_list args) {
	int tmp;
	va_list args_copy;
	char *tmpl, *key, *val;

	/* initialize lua state */
	lua_State *l = luaL_newstate();
	luaL_openlibs(l);

	/* pass arguments to lua */
	va_copy(args_copy, args); /* do not modify "args", it is reused */

	while ((key = va_arg(args_copy, char *)) != 0) {
		val = va_arg(args_copy, char *);

		if (val == NULL) {
			break;
		}

		lua_pushstring(l, val);
		lua_setglobal(l, key);
	}

	va_end(args_copy);

	/* execute code block */
	tmp = luaL_dostring(l, code);

	if (tmp != 0) {
		return NULL;
	}

	/* extract code block result */
	tmpl = (char *) lua_tostring(l, -1);

	lua_settop(l, 0);
	lua_close(l);

	return tmpl;
}
