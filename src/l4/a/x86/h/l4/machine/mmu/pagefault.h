#pragma once

// TODO: check for https://blog.csdn.net/mozun1/article/details/53306714

// Page fault error codes
#define PageFaultErrorCode_Protect 0x1	// Page fault caused by protection violation
#define PageFaultErrorCode_Write   0x2	// Page fault caused by a write
#define PageFaultErrorCode_User    0x4	// Page fault occured while in user mode
#define PageFaultErrorCode_Instr   0x10	// Page fault caused by instruction fetch
