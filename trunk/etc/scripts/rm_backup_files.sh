#!/bin/sh

# -i promt before every removal

find $MOST_ROOT -name \*~ -exec rm -v {} \; 
echo "finished"