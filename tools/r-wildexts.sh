#!/bin/sh

r_dirs() {
  subdirs() {
    ls -l $1 | grep '^d' | awk '{print $9}'
  }

  echo $1

  for d in `subdirs $1`
  do
    r_dirs $1$d/
  done
}

for d in "" `r_dirs`
do
  for x in $*
  do
    ls $d*.$x 2>/dev/null
  done
done
