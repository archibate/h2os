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

src_install()
{
	dir=${1?dir}
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
				$ipm_sudo cp -r $dir/out/$x $ipm_dest
			fi
		done
	fi
}

pkg_install()
{
	pkg=${1?pkg}
	echo "--> extracting $p"
	sh -c "cd $ipm_dest && tar -xzvf $pkg"
}

ipm_archive()
{
	p=${1?package name}
	dir=$ipm_src/$p
	pkg=$ipm_pub/$p.pkg
	tar -czvf $pkg $dir
}

real_install()
{
	p=${1?package name}
	dir=$ipm_src/$p
	pkg=$ipm_pub/$p.pkg
	if [ -d $dir ]
	then src_install $dir
	elif [ -f $pkg ]
	then pkg_install $pkg
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
ipm_$op $*
