#include <libl4/debug.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>
#include <libl4/captrs.h>
#include <libl4/puts.h>

/**
 * halt the machine through L4 debugger
 *
 * @note	would be failed if kernel is not
 * 		configured with debug object support
 *
 * @return	the kernel return value
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Debug_Halt(void)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Debug_Halt,
	};
	return l4Invoke(Libl4_CapDebug, &msg, sizeof(msg));
}

/**
 * print message on console through L4 debugger
 *
 * @note	would be failed if kernel is not
 * 		configured with debug object support
 *
 * @param s	pointer to the message string
 *
 * @return	the kernel return value
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Debug_Puts(const char *s)
{
	return l4Puts(Libl4_CapDebug, s);
}
