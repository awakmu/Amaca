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

See the [documentation](http://alexbio.github.com/Amaca) for more information.

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
