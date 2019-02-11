#pragma once

// Page fault error codes
#define PageFaultErrorCode_Protect 0x1	// Page fault caused by protection violation
#define PageFaultErrorCode_Write   0x2	// Page fault caused by a write
#define PageFaultErrorCode_User    0x4	// Page fault occured while in user mode
