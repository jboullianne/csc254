#!/bin/sh
 
gcc -g3 -o myprogram test.c
/home/hoover/u/cs254/bin/dwarfdump ./myprogram | python2 xref.py