#!/bin/sh
 
make
/home/hoover/u/cs254/bin/dwarfdump ./picoc | python2 xref.py