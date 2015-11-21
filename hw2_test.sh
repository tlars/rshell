#!/bin/sh

echo test -e rshell.cc && echo path exists 
echo test -d test && echo path exists 
echo test -f bob.cpp && echo path exists 
echo test bob.cpp || echo path is not there

( echo A && echo B ) || ( echo C && echo D )
( echo A && echo B ) || ( echo C && echo D ) && ( echo E || echo F )
( eho A && echo B ) || ( echo C && echo D )
( eho A && echo B ) && ( echo C && echo D )
( echo A && echo B ) && echo C
( echo A && eho B ) || echo C

 

