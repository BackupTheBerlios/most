#!/bin/sh

echo "Input search directory : "
read search_dir
echo "Convert all files of specified name in search directory"
echo "Input file name(\*.c, \*.h, Makefile, ...): "
read search_file_name

find $MOST_ROOT/${search_dir} -name ${search_file_name} -print -exec fromdos {} \;
echo "finished"
