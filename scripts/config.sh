#!/usr/bin/env bash
set -e

echo "# automaticly generated by $0" > tools/config.mak

for attr in `ls presets`
do
	echo "$attr:"
	select val in `ls presets/$attr | awk -F. '{print $1}'`
	do break
	done
	cat presets/$attr/$val.cfg >> tools/config.mak
done
