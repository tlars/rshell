#!/bin/sh

( echo A && echo B ) || ( echo C && echo D )
( echo A && echo B ) || ( echo C && echo D ) && ( echo E || echo F )
( eho A && echo B ) || ( echo C && echo D )
( eho A && echo B ) && ( echo C && echo D )
( echo A && echo B ) && echo C
( echo A && eho B ) || echo C


