#!/bin/sh

find $MOST_ROOT -name \*~ -exec rm -i -v $1 {} \; 
