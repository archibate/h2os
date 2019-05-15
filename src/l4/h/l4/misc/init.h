#pragma once

#define __$CONC(x, y) x##y
#define _$CONC(x, y) __$CONC(x, y)
#define THIS_INIT \
	static void _$CONC(__Ti, __LINE__)(void); \
	static void *_SECTION(".ctors") _$CONC(__Tip, __LINE__) = _$CONC(__Ti, __LINE__); \
	void _$CONC(__Ti, __LINE__)(void)
