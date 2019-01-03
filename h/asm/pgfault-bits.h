#pragma once

// Page fault error codes
#define PFEC_PR 0x1	// Page fault caused by protection violation
#define PFEC_W  0x2	// Page fault caused by a write
#define PFEC_U  0x4	// Page fault occured while in user mode
