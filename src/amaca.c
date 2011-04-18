/*
 * C template engine supporting Lua
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
#define TMPL_VAR   "__Amaca_tmpl__"
#define TMPL_PRINT "print"

#define check_value(x) if (x == NULL) return NULL;

char *Amaca_template(const char *template, ...);
char *Amaca_template_file(const char *filename, ...);

static int   lua_print(lua_State *l);
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
	char *str, *ret;
	va_list args;
	size_t fd_size;
	FILE *fd = fopen(filename, "rb");
	check_value(fd);

	/* read template file to memory */
	fseek(fd, 0, SEEK_END);
	fd_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	str = (char *) malloc(fd_size + 1);
	check_value(str);

	fread(str, sizeof(char), fd_size, fd);

	/* eval template with proper args */
	va_start(args, filename);
	ret = eval_template(str, args);
	va_end(args);

	return ret;
}

char *eval_template(const char *template, va_list args) {
	char *index = (char *) template;
	char *start, *end, *token;

	while ((start = strstr(index, TMPL_START)) != NULL) {
		int token_len;
		char *tmpl;

		/* extract block of code */
		end   = strstr(start, TMPL_END)+2;

		token_len = (end - 2) - (start + 2);
		token = (char *) malloc(token_len + 1);
		check_value(token);

		token = memcpy((char *) token, start + 2, token_len);
		check_value(token);

		tmpl = lua_exec(token, args);

		/* replace code block with its result */
		index  = str_replace(index, tmpl, start, end);
	}

	return index;
}

static char *str_replace(char *orig, char *str, char *start, char *end) {
	char *result = NULL;
	char *current;

	size_t src_len = strlen(orig);
	size_t str_len = strlen(str);
	size_t sub_len = end - start;

	result = (char *) malloc((src_len - sub_len) + str_len + 1);
	check_value(result);
	current = result;

	current = strncpy(current, orig, start - orig);
	check_value(current);
	current += start - orig;

	current = strncpy(current, str, str_len);
	check_value(current);
	current += str_len;

	current = strncpy(current, end, (orig + src_len) - end);
	check_value(current);

	return result;
}

static char *lua_exec(char *code, va_list args) {
	int tmp;
	va_list args_copy;
	char *tmpl, *key, *val;

	/* initialize lua state */
	lua_State *l = luaL_newstate();
	luaL_openlibs(l);
	lua_register(l, TMPL_PRINT, lua_print);

	lua_pushstring(l, "");
	lua_setglobal(l, TMPL_VAR);

	/* pass arguments to lua */
	va_copy(args_copy, args);

	while ((key = va_arg(args_copy, char *)) != 0) {
		val = va_arg(args_copy, char *);

		if (val == NULL)
			break;

		lua_pushstring(l, val);
		lua_setglobal(l, key);
	}

	va_end(args_copy);

	/* execute code block */
	tmp = luaL_dostring(l, code);

	/* extract code block result */
	lua_getglobal(l, TMPL_VAR);
	tmpl = (char *) lua_tostring(l, -1);

	lua_settop(l, 0);

	return tmpl;
}

static int lua_print(lua_State *l) {
	const char *tmpl;
	char *result = NULL;
	int i, args = lua_gettop(l);

	lua_getglobal(l, "tostring");

	/* merge print arguments in a single buffer */
	for(i = 1; i <= args; i++) {
		const char *s;

		lua_pushvalue(l, -1);
		lua_pushvalue(l, i);
		lua_call(l, 1, 1);
		s = lua_tostring(l, -1);

		if (result == NULL) {
			result = (char *) malloc(strlen(s) + 1);
			strcpy(result, s);
		} else {
			result = (char *) realloc(result, strlen(result) + strlen(s) + 1);
			result = strcat(result, s);
		}

		lua_pop(l, 1);
	}

	/* update lua result varible */
	lua_getglobal(l, TMPL_VAR);
	tmpl = lua_tostring(l, -1);

	result = (char *) realloc(result, strlen(tmpl) + strlen(result) + 1);

	result = strcat((char *)tmpl, result);

	lua_pushstring(l, result);
	lua_setglobal(l, TMPL_VAR);

	return 0;
}
