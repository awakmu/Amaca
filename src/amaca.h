/*
 * Template engine Lua-based in C
 *
 * Copyright (c) 2011, Alessandro Ghedini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the Mongrel2 Project, Zed A. Shaw, nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * amaca_eval - parse and eval a given template string
 * @param template Template string
 * @param nargs The number of the following key-value pairs
 * @param ... List of key-value pairs to pass to the template
 *
 * This function evals and evaluates the given template string, and returns
 * a pointer to the resulting string (returns NULL on failure). It also
 * optionally takes a list of key-value pairs that are passed to the template
 * as Lua variables.
 *
 * Example:
 *
 *   char *result = amaca_eval(
 *       template_string, 2,
 *       "my_var1", "Var1 content",
 *       "my_var2", "Var2 content"
 * );
 *
 */

extern char *amaca_eval(const char *template, int nargs, ...);

/**
 * amaca_eval_file - parse and eval a given template file
 * @param filename Template filename
 * @param nargs The number of the following key-value pairs
 * @param ... List of key-value pairs to pass to the template
 *
 * Just like amaca_eval() but takes as argument a template file instead
 * of a string. It also optionally takes a list of key-value pairs that are
 * passed to the template as Lua variables.
 *
 * Example:
 *
 *   char *result = amaca_eval_file(
 *       "/path/to/file.tmpl", 2,
 *       "my_var1", "Var1 content",
 *       "my_var2", "Var2 content"
 * );
 *
 */

extern char *amaca_eval_file(const char *filename, int nargs, ...);

/**
 * amaca_veval - parse and eval a given template string
 * @param template Template string
 * @param nargs The number of the following key-value pairs
 * @param args va_list of key-value pairs to pass to the template
 *
 * This function is equivalent to the function amaca_eval() except that
 * it is called with a va_list instead of a variable number of arguments.
 *
 * Example:
 *
 *   char *result = amaca_veval(template_string, nargs, args);
 *
 */

extern char *amaca_veval(const char *template, int nargs, va_list args);

/**
 * amaca_veval_file - parse and eval a given template file
 * @param filename Template filename
 * @param nargs The number of the following key-value pairs
 * @param args va_list of key-value pairs to pass to the template
 *
 * This function is equivalent to the function amaca_eval_file() except that
 * it is called with a va_list instead of a variable number of arguments.
 *
 * Example:
 *
 *   char *result = amaca_veval_file("/path/to/file.tmpl", nargs, args);
 *
 */

extern char *amaca_veval_file(const char *filename, int nargs, va_list args);
