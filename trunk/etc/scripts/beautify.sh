#!/bin/sh

# -gnu : style
# -i4 : set indent level to n (4) spaces 
# -bli0 : indent braces n (0) spaces
# -nut : use spaces instead of tabs
# -l100 : maximum line length n (100)
# -ncs : no space after casts
# -psl : put the type of a procedure on the line before its name
# -nfca : do not format any comments

indent_opt="-gnu -i4 -bli0 -nut -l100 -ncs -psl -nfca"

echo "Input search directory : "
read search_dir

echo "Convert all files of specified name in search directory"
echo "Input file name(\*.c, \*.h, Makefile, ...): "
read search_file_name


find $MOST_ROOT/${search_dir} -name ${search_file_name} -print -exec indent ${indent_opt} {} \;

echo "finished"
