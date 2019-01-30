for x in `find ${1-sh}`; do tools/nt-enum.pl < $x; echo; done
