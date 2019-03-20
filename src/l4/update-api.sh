#!/bin/bash
set -e

sys_process() {
	echo '#include <l4/stdafx.h>'
	echo '#include <l4/sys/sysnr.h>'
	echo '#include <l4/sys/sysdef.h>'
	echo '#define bool int'
	echo

	#awk -F '//>> ' '{printf "%s,%s", $1, $2}' | \
	sed 's/\*/\* /g' | awk -F'(' '{printf "%s,%s,,\n", $1, substr($2, 1, length($2)-2)}' | sed 's/void,,//' | \
	awk -F' ' -v RS=',' '{printf "%s,%s, ", substr($0, 1, length($0)-length($NF)-1), $NF}' | \
	sed -z 's/,\s,,\s[,\s]*/\n/g' | sed 's/^,\s*//g' | awk -F',' '{if (NF) printf "_SYS%d0(%s)\n", NF/2-1, $0}' | \
	sed 's/_SYS10(\(.*\), ,/_SYS00(\1/' | sed 's/, )/)/' | sed 's/sys_//'
}

libl4=../lib/l4
rm -rf $libl4/c/api.gen/
mkdir $libl4/c/api.gen/
for x in `find h/l4/api -type f -name '*.h'`
do grep sys_ $x | sys_process > $libl4/c/api.gen/`basename -s.h $x`.c
done

#sed -i 's/_SYS20(\([\w\s]+\),recv\(,.+\)?)/_SYS21(\1,recv,\2,int,phase)/' $libl4/c/api.gen/ipc.c

make_sysnr_h() {
	echo '#pragma once'
	echo

	(for x in `find h/l4/api -type f -name '*.h'`
	do grep sys_ $x | awk -F'(' '{print $1}'
	done) | awk -F'sys_' '{printf "#define _SYS_%s %d\n", $2, NR}'
}

make_systab_c() {
	echo '#include <l4/sys/sysnr.h>'
	for x in `find h/l4/api -type f -name '*.h'`
	do echo "#include <l4/api/`basename $x`>"
	done
	echo

	echo 'void *_systab[] = {'
	(for x in `find h/l4/api -type f -name '*.h'`
	do grep sys_ $x | awk -F'(' '{print $1}'
	done) | awk -F'sys_' '{printf "\t[_SYS_%s] = sys_%s,\n", $2, $2}'
	echo '};'
}

make_sysnr_h > h/l4/sys/sysnr.gen.h
make_systab_c > c/sys/systab.gen.c
