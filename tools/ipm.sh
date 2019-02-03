#!/bin/bash

set -e

ipm_src=src
ipm_dest=isodir
ipm_var=out/var/ipm

ipm_init()
{
	rm -rf $ipm_var
	mkdir -p $ipm_var
	touch $ipm_var/packages.lst
}

read_ini_deps()
{
	while read -r k v
	do
		case $k in
		deps) echo $v;;
		esac
	done
}
dep_rescue()
{
	true ${1?package name}
	for d in `awk -F = '{print $1,$2}' $ipm_src/$1/package.ini | read_ini_deps`
	do
		if ! echo $dpkgs | grep $d
		then
			dep_rescue $d
		fi
	done
	dpkgs+="$1 "
	let pkgcnt+=1
}

real_install()
{
	p=${1?package name}
	dir=$ipm_src/$p
	if [ -f $dir/Makefile ]
	then
		echo "--> building $p"
		make -sBj4 -C $dir
	fi
	if [ -d $dir/out ]
	then
		echo "--> installing $p"
		mkdir -p $ipm_dest
		for x in boot bin lib
		do
			if [ -d $dir/out/$x ]
			then
				echo "  + $ipm_dest/$x"
				cp -r $dir/out/$x $ipm_dest
			fi
		done
	fi
	echo "+++ installed $p"
	echo --------------------------------------
}

ipm_install()
{
	dpkgs=
	pkgcnt=0
	for p in $*
	do dep_rescue $p
	done

	echo ======================================
	echo "packages: $dpkgs"
	echo ======================================

	for p in $dpkgs
	do real_install $p
	done
}

ipm_init
op=${1?ipm operation}
shift
case $op in
install) ipm_install $*;;
*) echo bad ipm operation $1 >&2;;
esac
