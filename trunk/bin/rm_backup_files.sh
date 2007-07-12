#!/bin/sh

find $1 -name \*~ -exec rm -i -v $2 {} \; 
