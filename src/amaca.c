/*
 * Lazy C template engine supporting Lua scripting
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
#define TMPL_VAR   "__Amaca_output__"
#define TMPL_PRINT "_print"

static int lua_print(lua_State *l);

char *Amaca_template(char *template);
char *Amaca_template_file(char *filename);

char *Amaca_template(char *template) {
	char *result = NULL;
	char *index = template;

	while (index) {
		int tmp, token_len;
		const char *output, *start, *end, *token;

		start = strstr(index, TMPL_START);

		if (start == NULL)
			break;

		end   = strstr(start, TMPL_END)+2;

		token_len = (end - 2) - (start + 2);
		token = (char *) malloc(token_len + 1);

		if (token == NULL) {
			return NULL;
		}

		token = memcpy((char *) token, start + 2, token_len);

		if (token == NULL) {
			return NULL;
		}

		lua_State *l = luaL_newstate();
		luaL_openlibs(l);
		lua_register(l, TMPL_PRINT, lua_print);

		lua_pushstring(l, "");
		lua_setglobal(l, TMPL_VAR);

		tmp = luaL_dostring(l, token);

		lua_getglobal(l, TMPL_VAR);
		output = lua_tostring(l, -1);

		lua_settop(l, 0);

		{
			char *current;

			size_t src_len = strlen(index);
			size_t str_len = strlen(output);
			size_t sub_len = end - start;

			result = (char *) malloc((src_len - sub_len) + str_len + 1);

			if (result == NULL) {
				return NULL;
			}

			current = result;

			current = strncpy(current, index, start - index);

			if (current == NULL) {
				return NULL;
			}

			current += start - index;

			current = strncpy(current, output, str_len);

			if (current == NULL) {
				return NULL;
			}

			current += str_len;

			current = strncpy(current, end, (index + src_len) - end);

			if (current == NULL) {
				return NULL;
			}
		}

		index  = result;
	}

	return result;
}

char *Amaca_template_file(char *filename) {
	char *str;
	size_t fd_size;
	FILE *fd = fopen(filename, "rb");

	if (fd == NULL) {
		return NULL;
	}

	fseek(fd, 0, SEEK_END);
	fd_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	str = (char *) malloc(fd_size + 1);

	if (str == NULL) {
		return NULL;
	}

	fread(str, sizeof(char), fd_size, fd);

	return Amaca_template(str);
}

static int lua_print(lua_State *l) {
	const char *output;
	char *result = NULL;
	int i, args = lua_gettop(l);

	lua_getglobal(l, "tostring");

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

	lua_getglobal(l, TMPL_VAR);
	output = lua_tostring(l, -1);

	result = (char *) realloc(result, strlen(output) + strlen(result) + 1);
	result = strcat((char *) output, result);

	lua_pushstring(l, result);
	lua_setglobal(l, TMPL_VAR);

	return 0;
}
