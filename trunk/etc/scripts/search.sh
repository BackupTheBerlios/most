#!/bin/sh

find $MOST_ROOT -name \*.$1 -print -exec grep -n -e $2 {} \;
