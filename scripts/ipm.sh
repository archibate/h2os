#!/bin/bash
set -e

ipm_src=src
ipm_pub=pub
ipm_dest=out

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
		if ! echo $dpkgs | grep $d > /dev/null
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
		for x in boot bin lib include
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
	tar -xvf $pkg
}

ipm_archive()
{
	p=${1?package name}
	dir=$ipm_src/$p
	pkg=$ipm_pub/$p.pkg
	#find $dir/out | sed "s/^`echo src/libc/out | sed 's/\\//\\\\\\//g'`//" | cpio -o
	ocd=`pwd`
	cd $dir
	tar -cvf /tmp/$$-out.pkg out
	cd $ocd
	mv /tmp/$$-out.pkg $pkg
}

real_install()
{
	p=${1?package name}
	dir=$ipm_src/$p
	pkg=$ipm_pub/$p.pkg
	if [ -f $pkg ]
	then pkg_install $pkg
	elif [ -d $dir ]
	then src_install $dir
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

op=${1?ipm operation}
shift
ipm_$op $*
