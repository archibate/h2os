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
	pkg=${1?package name}
	deps=`awk -F = '{print $1,$2}' $ipm_src/$pkg/package.ini | read_ini_deps`

	echo $pkg
	for d in $deps
	do
		#if ! grep -qs $d $ipm_var/packages.lst
		#then
		dep_rescue $d
		#fi
	done
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
		for x in boot bin
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
	pkgs=
	pkgcnt=0
	for p in $*
	do pkgs+=$(echo `dep_rescue $p | sort -u`)
	done

	echo ======================================
	echo "packages: $pkgs"
	echo ======================================

	for p in $pkgs
	do real_install $p
	done
}

op=${1?ipm operation}
shift
case $op in
init) ipm_init;;
install) ipm_install $*;;
*) echo bad ipm operation $1 >&2;;
esac
