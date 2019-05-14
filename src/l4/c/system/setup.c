void generic_setup(void)
{
	extern void *__CTORS_BEGIN__[], *__CTORS_END__[];
	void **p;
	for (p = __CTORS_BEGIN__; p < __CTORS_END__; p++)
		(*(void (**)(void))p)();
}
