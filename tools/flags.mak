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
          error=incompatible-pointer-types error=int-conversion no-enum-compare
FUNCTIONS+=no-stack-protector
ifdef DEBUG
DBGFLAGS+=stabs+ gdb
endif
endif

ADIRS=.
ifeq ($(shell if [ -d a ]; then echo 1; fi),1)
ADIRS+=a/$(ARCH)
ifeq ($(shell if [ -d a/$(ARCH)/$(BITS) ]; then echo 1; fi),1)
ADIRS+=a/$(ARCH)/$(BITS)
endif
ifeq ($(shell if [ -d a/$(BITS) ]; then echo 1; fi),1)
ADIRS+=a/$(BITS)
endif
endif

include tools/modules.mak

LIBPATH+=$(ROOT)/out/lib
SRCPATH+=$(ADIRS:%=%/c)
INCPATH+=$(ADIRS:%=%/h) $(ROOT)/include $(ROOT)/out/include

CFLAGS+=-nostdinc
CFLAGS+=$(OPTIMIZE:%=-O%) $(MODES:%=-m%) \
        $(FUNCTIONS:%=-f%) $(WARNINGS:%=-W%) \
        $(DEFINES:%=-D%) $(INCPATH:%=-I%) \
	$(DBGFLAGS:%=-g%) $(INCLUDES:%=-include%)
ASMFLAGS+=$(DEFINES:%=-D%) $(INCPATH:%=-I%/)
ASMFLAGS+=-P $(ROOT)/scripts/nasm.inc
LDFLAGS+=-static -nostdlib
LDFLAGS+=$(LIBPATH:%=-L%)
STRIPFLAGS+=-S -O elf32-i386
