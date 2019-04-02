#!/bin/sh

sed 's/str\(\w\+\)/wcs\1/g' $1 | sed 's/char/wchar_t/g' | sed 's/wcsing\.h/wchar\.h/'
