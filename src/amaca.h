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

/**
 * Amaca_template - parse and eval a given template string
 * @param template Template string
 * @param ... List of key-value pairs to pass to the template
 *
 * This function parses and evaluates the given template string, and returns
 * a pointer to the resulting string (returns NULL on failure). It also
 * optionally takes a list of key-value pairs that are passed to the template
 * as Lua variables.
 *
 * Example:
 *
 *   char *result = Amaca_template(
 *       template_string,
 *       "my_var1", "Var1 content",
 *       "my_var2", "Var2 content"
 * );
 *
 */

extern char *Amaca_template(char *template, ...);

/**
 * Amaca_template_file - parse and eval a given template file
 * @param filename Template filename
 * @param ... List of key-value pairs to pass to the template
 *
 * Just like Amaca_template() but takes as argument a template file instead
 * of a string. It also optionally takes a list of key-value pairs that are
 * passed to the template as Lua variables.
 *
 * Example:
 *
 *   char *result = Amaca_template(
 *       "/path/to/file.tmpl",
 *       "my_var1", "Var1 content",
 *       "my_var2", "Var2 content"
 * );
 *
 */

extern char *Amaca_template_file(char *filename, ...);
