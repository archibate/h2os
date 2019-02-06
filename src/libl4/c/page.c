#include <libl4/page.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>
#if 0 // {{{
/**
 * map a page in current address space
 *
 * @param cptr	capability to the page
 *
 * @param vaddr	virtual address to be mapped
 *
 * @return	the kernel return value
 *
 * @retval -L4_EMapped	virtual address `vaddr` already mapped
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Page_Map(l4CPtr_t cptr, l4Word_t vaddr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Page_Map,
		[L4_Page_Map_Arg_VAddr] = vaddr,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

/**
 * map a page table in current address space
 *
 * @param cptr	capability to the page table
 *
 * @param vaddr	virtual address to be mapped
 *
 * @return	the kernel return value
 *
 * @retval -L4_EMapped	virtual address `vaddr` already mapped
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Pgtab_Map(l4CPtr_t cptr, l4Word_t vaddr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Pgtab_Map,
		[L4_Pgtab_Map_Arg_VAddr] = vaddr,
	};
	return l4Invoke(cptr, msg, sizeof(msg));
}
#endif // }}}

/**
 * map a page in a page directory
 *
 * @param cptr	capability to the page directory
 *
 * @param vaddr	virtual address to be mapped
 *
 * @param page	capability to the page
 *
 * @return	the kernel return value
 *
 * @retval -L4_ECLookup	cannot lookup capability pointer `page`
 *
 * @retval -L4_ECapType	`page` is not a page capability
 *
 * @retval -L4_EMapped	virtual address `vaddr` already mapped
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Pgdir_MapPage(l4CPtr_t cptr, l4CPtr_t page, l4Word_t vaddr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Pgdir_MapPage,
		[L4_Pgdir_MapPage_Arg_VAddr] = vaddr,
		[L4_Pgdir_MapPage_Arg_PageCPtr] = page,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

/**
 * map a page table in a page directory
 *
 * @param cptr	capability to the page directory
 *
 * @param vaddr	virtual address to be mapped
 *
 * @param pgtab	capability to the page table
 *
 * @return	the kernel return value
 *
 * @retval -L4_ECLookup	cannot lookup capability pointer `pgtab`
 *
 * @retval -L4_ECapType	`pgtab` is not a page table capability
 *
 * @retval -L4_EMapped	virtual address `vaddr` already mapped
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Pgdir_MapPgtab(l4CPtr_t cptr, l4CPtr_t pgtab, l4Word_t vaddr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Pgdir_MapPgtab,
		[L4_Pgdir_MapPgtab_Arg_VAddr] = vaddr,
		[L4_Pgdir_MapPgtab_Arg_PgtabCPtr] = pgtab,
	};
	return l4Invoke(cptr, msg, sizeof(msg));
}

/**
 * unmap a page from a page directory
 *
 * @param cptr	capability to the page directory
 *
 * @param vaddr	virtual address to be mapped
 *
 * @return	the kernel return value
 *
 * @retval -L4_EFault	virtual address `vaddr` not mapped
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Pgdir_UnmapPage(l4CPtr_t cptr, l4Word_t vaddr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Pgdir_UnmapPage,
		[L4_Pgdir_MapPage_Arg_VAddr] = vaddr,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

/**
 * unmap a page table from a page directory
 *
 * @param cptr	capability to the page directory
 *
 * @param vaddr	virtual address to be mapped
 *
 * @return	the kernel return value
 *
 * @retval -L4_EFault	virtual address `vaddr` not mapped
 *
 * @retval -Libl4_Error	unexcepted error
 */
int l4Pgdir_UnmapPgtab(l4CPtr_t cptr, l4Word_t vaddr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Pgdir_UnmapPgtab,
		[L4_Pgdir_UnmapPgtab_Arg_VAddr] = vaddr,
	};
	return l4Invoke(cptr, msg, sizeof(msg));
}
