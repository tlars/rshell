#!/bin/sh

echo hello world; ls -a; mkdir test;
sleep 2

echo hello || ls -R
sleep 2

ls -l && mkdir test
sleep 2

ls -a      ; echo hello       ;
