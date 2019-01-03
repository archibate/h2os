
OPTIMIZE=0
DEFINES+=__WORDSIZE__=4
MODES+=32 arch=i386 soft-float
FUNCTIONS+=no-common no-exceptions no-non-call-exceptions freestanding no-builtin
WARNINGS+=all no-unused-function implicit-function-declaration no-main \
					no-format no-error=unused-variable no-error=unused-but-set-variable \
					error=incompatible-pointer-types error=int-conversion
FUNCTIONS+=no-stack-protector

INCPATH+=$Dh
LIBPATH+=$Dc/libc

CFLAGS+=-nostdinc
CFLAGS+=$(OPTIMIZE:%=-O%) $(MODES:%=-m%) \
		$(FUNCTIONS:%=-f%) $(WARNINGS:%=-W%) \
		$(DEFINES:%=-D%) $(INCPATH:%=-I%)
ASMFLAGS+=$(INCPATH:%=-I%/)
LDFLAGS+=-static -nostdlib
LDFLAGS+=$(LIBPATH:%=-L%)
