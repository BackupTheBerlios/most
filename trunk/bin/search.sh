#!/bin/sh

find $1 -name \*.$2 -print -exec grep -n -e $3 {} \;
