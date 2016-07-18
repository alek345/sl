#!/bin/sh
gcc -g src/main.c src/lex.c src/parse.c src/ast.c -o slc

if [ $? -eq 0 ]; then
	echo Build succeded!;
else
	echo Build failed!;
fi
