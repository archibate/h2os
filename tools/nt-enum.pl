#!/usr/bin/perl

use strict;
use warnings;

while (<>) {
	chomp;
	if (/enum (\w*)/) {
		print "const char *__ntNameTableOfEnum_$1\[\] =\n{\n";
		while (<>) {
			chomp;
			if (/^\s*(\w*)\s*(=\s*(\w+))?,\s*$/) {
				if (defined $3) {
					print qq'\t[$3] = "$1",\n';
				} else {
					print qq'\t"$1",\n';
				}
			}
		}
		print "};\n";
		exit;
	}
}
#die;
