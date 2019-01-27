void init_main(void)
{
	asm volatile ("int3");
	for (;;);
}
