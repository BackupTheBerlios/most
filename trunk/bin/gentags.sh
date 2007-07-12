#!/bin/sh

rm -f TAGS
find ../ -name \*.c -print -exec etags -a -o TAGS {} \;
find ../ -name \*.h -print -exec etags -a -o TAGS {} \;
