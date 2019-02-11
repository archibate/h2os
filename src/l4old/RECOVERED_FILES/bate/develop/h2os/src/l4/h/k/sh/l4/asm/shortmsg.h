#pragma once

/* N: keep sync with xxx?? */
enum L4_ShortMsgIndexs {
	L4_ShortMsg_EBX,
	L4_ShortMsg_EDI,
	L4_ShortMsg_ESI,
	L4_ShortMsg_EBP,
	L4_ShortMsgWords,
/* some arch-independent shortcuts: */
	L4_ShortMsg_Word0     = L4_ShortMsg_EBX,
	L4_ShortMsg_Word1     = L4_ShortMsg_EDI,
	L4_ShortMsg_Word2     = L4_ShortMsg_ESI,
	L4_ShortMsg_Word3     = L4_ShortMsg_EBP,
};
