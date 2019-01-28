OPTIMIZE=0
DEFINES+=__WORDSIZE__=4
ifdef MINGW
DEFINES+=_MINGW=1
endif
MODES+=32 arch=i386 soft-float
FUNCTIONS+=no-common no-exceptions no-non-call-exceptions freestanding no-builtin
ifndef MINGW
WARNINGS+=error all no-unused-function error=implicit-function-declaration no-main \
          no-format no-error=unused-variable no-error=unused-but-set-variable \
          error=incompatible-pointer-types error=int-conversion
FUNCTIONS+=no-stack-protector
DBGFLAGS+=stabs+ gdb
endif

INCPATH+=include arch/$(ARCH)/include \
         $Dinclude $Darch_include/$(ARCH)
LIBPATH+=$Dsrc/libc $Dsrc/libsys

CFLAGS+=-nostdinc
CFLAGS+=$(OPTIMIZE:%=-O%) $(MODES:%=-m%) \
        $(FUNCTIONS:%=-f%) $(WARNINGS:%=-W%) \
        $(DEFINES:%=-D%) $(INCPATH:%=-I%) \
	$(DBGFLAGS:%=-g%)
ASMFLAGS+=$(DEFINES:%=-D%) $(INCPATH:%=-I%/)
ASMFLAGS+=-P $Dnasm.inc
LDFLAGS+=-static -nostdlib
LDFLAGS+=$(LIBPATH:%=-L%)
