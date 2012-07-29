#!/bin/sh

rm -f TAGS
find $MOST_ROOT -name \*.c -print -exec etags -a -o $MOST_ROOT/etc/TAGS {} \;
find $MOST_ROOT -name \*.h -print -exec etags -a -o $MOST_ROOT/etc/TAGS {} \;
echo "finished"