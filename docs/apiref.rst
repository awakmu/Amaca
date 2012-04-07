.. _apiref:

*************
Api Reference
*************

.. highlight:: c

Intro
=====

The file :file:`amaca.h` is used to include all the functions and data types
declarations. It's enough to::

   #include <amaca.h>

in each source file to start using Tofu. All the functions are prefixed by
``amaca_``.

The templates are just plain text strings or files containing blocks of Lua
code. The code blocks have to be enclosed between ``{{`` and ``}}``.

.. _string:

String
======

.. c:function:: char *amaca_eval(const char *template, int argc, ...);

   Evaluate the given template string, passing to it the following `argc`
   key-value pairs.

   .. versionadded:: 0.5

.. c:function:: char *amaca_veval(const char *template, int argc, va_list args);

   Equivalent to `amaca_eval` except that it is called with a `va_list` instead
   of a variable number of arguments.

   .. versionadded:: 0.5

.. _file:

File
====

.. c:function:: char *amaca_eval_file(const char *filename, int argc, ...);

   Evaluate the given template file, passing to it the following `argc`
   key-value pairs.

   .. versionadded:: 0.5

.. c:function:: char *amaca_veval_file(const char *filename, int argc, va_list args);

   Equivalent to `amaca_eval_file` except that it is called with a `va_list`
   instead of a variable number of arguments.

   .. versionadded:: 0.5

.. _fd:

File descriptor
===============

.. c:function:: char *amaca_eval_fd(const int fd, int argc, ...);

   Evaluate the given template file descriptor, passing to it the following
   `argc` key-value pairs.

   .. versionadded:: 0.6

.. c:function:: char *amaca_veval_fd(const int fd, int argc, va_list args);

   Equivalent to `amaca_eval_fd` except that it is called with a `va_list`
   instead of a variable number of arguments.

   .. versionadded:: 0.6
