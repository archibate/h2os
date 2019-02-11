#!/usr/bin/env perl

use strict;
use warnings;


sub prhdr {
	my $func = @_;

	print <<EOF
globl sys_$func
sys_$func:
	push ebp
	mov ebp, esp
EOF
;
}


sub arg_word {
	my $n = @_;
	return "dword [ebp + ".(8 + $n*4)."]";
}


my @regs = qw(ebx edi esi ebp);

sub put_word {
	my $ret;
	if ($put_n < $#regs) {
		return "$regs[$put_n]";
	} else {
		return "dword [l4ide_buffer + ".($put_n - $#regs)."]";
	}
}
