#!/bin/sh

find $1 -name \*.c -print -exec indent -i4 -bli0 -nut -l100 -ncs {} \;
find $1 -name \*.h -print -exec indent -i4 -bli0 -nut -l100 -ncs {} \;
