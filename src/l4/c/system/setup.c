#define CALL(func,...) extern void func(); func(__VA_ARGS__);

void generic_setup(void)
{
	//CALL(init_mman);
	CALL(init_kcg);
	CALL(init_idg);
	CALL(init_softirq_aeps);
}
