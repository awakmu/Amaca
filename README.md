Amaca
=====

Amaca (pronounced: amàca) is a tiny template engine written in C, which natively
supports Lua scripting. This means that you can create your templates without
having to learn yet another templating language. You'll need just a minimal
knowledge of the plain and simple Lua.

## GETTING STARTED

~~~~ c
#include <amaca.h>

int main() {
  char *title = "This is a title";
  char *body  = "This is the body";

  char *str = amaca_eval_file(
    "eg/example.tmpl", 2,
    "title", title,
    "body", body
  );

  return 0;
}
~~~~

## OVERVIEW

Amaca consists of the following functions:

~~~~ c
char *amaca_eval(const char *template, int argc, ...);
~~~~

Which takes as argument a template string and a list of key-value pairs to pass
to the template code, which will be available within the Lua scripts. `argc`
represents the number of key-value pairs passed to the function.

~~~~ c
char *amaca_eval_file(const char *filename, int argc, ...);
~~~~

It's the same as the previous function, but it takes as argument the path to a
template file.

~~~~ c
char *amaca_eval_fd(const int fd, int argc, ...);
~~~~

It's the same as the previous functions, but it takes as argument a file descriptor.

The functions `amaca_veval()`, `amaca_veval_file()` and `amaca_veval_fd()` are
equivalent to the functions `amaca_eval()`, `amaca_eval_file()` and
`amaca_eval_fd()` respectively, except that they are called with a `va_list`
instead of a variable number of arguments.

The templates are just plain text strings or files containing blocks of Lua code.
The code blocks have to be enclosed between `{{` and `}}`.

See the `eg/example.tmpl` file for a template example, and the `eg/example.c`
file for a (very simple) code example.

## DEPENDENCIES

 * `luajit` or `lua`

To properly build Amaca, using the bundled Makefile, the `pkg-config` utility is
also recommended.

## BUILDING

Amaca is distributed as source code. Install with:

~~~~
$ make
~~~~

You can also copy-paste the `src/amaca.{c,h}` files inside your project and use
a custom Makefile to build them.

## COPYRIGHT

Copyright (C) 2011 Alessandro Ghedini <al3xbio@gmail.com>

See COPYING for the license.
