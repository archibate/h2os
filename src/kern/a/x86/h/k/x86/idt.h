#pragma once

#include <x86/gate.h>

#define INTR_IRQ0 64
#define INTR_SWI0 128
#define NIDT_ENTS 256

enum IntrNumbers {
	ExceptionDivideError,ExceptionDebug,ExceptionNMI,ExceptionBreakPoint,ExceptionOverflow,
	ExceptionBoundsCheck,ExceptionInvalidOpcode,ExceptionDeviceNotAvailable,
	ExceptionDoubleFault,ExceptionCoprocessorSegOverrun,ExceptionInvalidTss,
	ExceptionSegmentNotPresent,ExceptionStackSegment,ExceptionGeneralProtection,
	ExceptionPageFault,ExceptionReservedException,ExceptionCoprocessorError,
	IntrIRQ0 = INTR_IRQ0,
	IntrSyscall = INTR_SWI0,
};

extern struct gatedesc idt[NIDT_ENTS];
