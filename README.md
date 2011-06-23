Amaca
=====

![status](http://stillmaintained.com/AlexBio/Amaca.png)

Amaca (pronounced: amàca) is a simple template engine written in C, which
natively supports Lua scripting. This means that you can create your templates
without the need to learn yet another templating language. You'll need just a
minimal knowledge of the plain and simple Lua.

## USAGE

Amaca consists of the following functions:

    char *Amaca_template(const char *template, int nargs, ...);

Which takes as argument a template string and a list of key-value pairs to pass
to the template code, which will be available within the Lua scripts. `nargs`
represents the number of key-value pairs passed to the function.

    char *Amaca_template_file(const char *filename, int nargs, ...);

It's the same as the previous function, but it takes as argument the path to a
template file.

The functions `Amaca_vtemplate()` and `Amaca_vtemplate_file()` are equivalent to
the functions `Amaca_template()` and `Amaca_template_file()` respectively, except
that they are called with a va_list instead of a variable number of arguments.

The templates are just plain text strings or files containing blocks of Lua code.
The code blocks have to be enclosed between `{{` and `}}`.

See the `eg/example.tmpl` file for a template example, and the `eg/example.c`
file for a (very simple) code example.

## INSTALLATION

Amaca is distributed as source code. Install with:

    $ git clone git://github.com/AlexBio/Amaca.git
    $ cd Amaca
    $ make

You can also copy-paste the `src/amaca.{c,h}` files inside your project and use
a custom Makefile to build it.

## COPYRIGHT

Copyright (C) 2011 Alessandro Ghedini <al3xbio@gmail.com>

See COPYING for the license.
