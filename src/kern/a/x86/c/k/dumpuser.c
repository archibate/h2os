#include <k/printk.h>
#include <k/a/dumpuser.h>
#include <k/kstack.h>
#include <x86/cregs.h>
#include <x86/eflags.h>

void dumpuser(void)
{
	uint eflags = kIFrame[IFrame_EFLAGS];
	dprintk("EAX=%p  ECX=%p", kIFrame[IFrame_EAX], kIFrame[IFrame_ECX]);
	dprintk("EDX=%p  EBX=%p", kIFrame[IFrame_EDX], kIFrame[IFrame_EBX]);
	dprintk("ESI=%p  EDI=%p", kIFrame[IFrame_EAX], kIFrame[IFrame_ECX]);
	dprintk("ESP=%p  EBP=%p", kIFrame[IFrame_ESP], kIFrame[IFrame_EBP]);
	dprintk("SS:ESP=%04x:%p", kIFrame[IFrame_SS], kIFrame[IFrame_ESP]);
	dprintk("CS:EIP=%04x:%p", kIFrame[IFrame_CS], kIFrame[IFrame_EIP]);
#define EF(fl,t,alt) eflags & (fl) ? #t : #alt
	dprintk("EFLAGS=%08x (%s%s%s%s%s%s %s %s %s IOPL=%d %s %s %s %s %s %s)", eflags,
			EF(FL_CF, C, c),
			EF(FL_PF, P, p),
			EF(FL_AF, A, a),
			EF(FL_ZF, Z, z),
			EF(FL_SF, S, s),
			EF(FL_OF, O, o),
			EF(FL_DF, DF, df),
			EF(FL_YF, YF, yf),
			EF(FL_IF, IF, if),
			GET_FL_IOPL(eflags),
			EF(FL_NT, NT, nt),
			EF(FL_RF, RF, rf),
			EF(FL_VM, VM, vm),
			EF(FL_VIF, VIF, vif),
			EF(FL_VIP, VIP, vip),
			EF(FL_ID, ID, id));
	dprintk("INT#%d  ERR=%d", kIFrame[IFrame_IntrNum], kIFrame[IFrame_ErrorCode]);
	dprintk("CR2=%p  CR3=%p", getcr2(), getcr3());
}
