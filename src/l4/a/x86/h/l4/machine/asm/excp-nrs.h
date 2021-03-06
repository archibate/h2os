#pragma once

enum ExceptionNumbers {
	ExceptionDivideError,ExceptionDebug,ExceptionNMI,ExceptionBreakPoint,ExceptionOverflow,
	ExceptionBoundsCheck,ExceptionInvaidOpcode,ExceptionDeviceNotAvailable,
	ExceptionDoubleFault,ExceptionCoprocessorSegOverrun,ExceptionInvaidTss,
	ExceptionSegmentNotPresent,ExceptionStackSegment,ExceptionGeneralProtection,
	ExceptionPageFault,ExceptionReservedException,ExceptionCoprocessorError,
};
