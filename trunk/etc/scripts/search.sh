#!/bin/sh

echo "Serach in all files of specified name in MOST_ROOT"

echo "Input file name(\*.c, \*.h, Makefile, ...): "
read search_file_name

echo "for expression : "
read search_expression

find $MOST_ROOT -name ${search_file_name} -print -exec grep -n -e ${search_expression} {} \;

echo "finished"
