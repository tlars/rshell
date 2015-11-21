#!/bin/sh


echo test -e rshell.cc && echo path exists 
echo test -d test && echo path exists 
echo test -f bob.cpp && echo path exists 
echo test bob.cpp || echo path is not there


