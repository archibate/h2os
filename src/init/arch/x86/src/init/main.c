void init_main(void)
{
	extern void do_syscall();
	do_syscall();
	for (;;);
}
