%ifdef _MINGW
	%macro globl 1
		_%1 equ %1
		[global _%1]
	%endmacro

	%macro extrn 1
		[extern _%1]
		%1 equ _%1
	%endmacro

	%macro extrn_main 0
		[extern _mingw_main]
		main equ _mingw_main
	%endmacro
%else
	%macro globl 1
		[global %1]
	%endmacro

	%macro extrn 1
		[extern %1]
	%endmacro

	%macro extrn_main 0
		[extern main]
	%endmacro
%endif
