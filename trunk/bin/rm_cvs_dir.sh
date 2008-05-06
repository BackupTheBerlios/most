#!/bin/sh

find $MOST_ROOT -name \CVS -exec rm -r -i -v $1 {} \; 
