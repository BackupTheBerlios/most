#!/bin/sh

find $1 -name \CVS -exec rm -r -i -v $2 {} \; 
