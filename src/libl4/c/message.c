#include <libl4/message.h>
#include <libl4/syscall.h>
#include <l4/asm/shortmsg.h>
#include <l4/consts.h>
#include <numtools.h>
#include <memory.h>
#define L4_ShortMsgBytes  (L4_ShortMsgWords * sizeof(l4Word_t))
#define L4_MaxExtraBytes  (L4_MaxExtraWords * sizeof(l4Word_t))

static void l4ExtraRewind(void);
static l4Ret_t l4ExtraWrite(const void *buf, size_t len);
static l4Ret_t l4ExtraCapWrite(const l4CPtr_t *caps, size_t capnr);
static l4Ret_t l4ExtraRead(void *buf, size_t size);

static size_t msg_bytes;
static l4Byte_t msg_shrt[L4_ShortMsgBytes];


/**
 * initialize the message to be sent
 */
void l4MsgInit(void)
{
	msg_bytes = 0;
	memset(msg_shrt, 0, sizeof(msg_shrt));
	l4ExtraRewind();
}

/**
 * @return	maxium bytes we can transfer per invocation
 */
size_t l4MsgGetBufSize(void)
{
	return L4_ShortMsgBytes + L4_MaxExtraBytes;
}

/**
 * invoke stage of a message
 *
 * @param cptr	capability to invoke
 *
 * @return	the kernel return value
 */
l4Ret_t l4MsgInvoke(l4CPtr_t cptr)
{
	l4Ret_t ret = l4Syscall(cptr, (l4Word_t*)&msg_shrt);
	msg_bytes = 0;
	l4ExtraRewind();
	return ret;
}

/**
 * write into the message buffer to be sent
 *
 * @param buf	data to write
 *
 * @param size	size of the data
 *
 * @retval 0	success
 *
 * @retval !0	error from kernel
 */
l4Ret_t l4MsgWrite(const void *buf, size_t size)
{
	if (msg_bytes < L4_ShortMsgBytes) {
		size_t len = MIN(size, L4_ShortMsgBytes - msg_bytes);
		memcpy(msg_shrt + msg_bytes, buf, len);
		msg_bytes += len;
		buf += len;
		size -= len;
		if (!size)
			return 0;
	}
	return l4ExtraWrite(buf, size);
}

/**
 * read data from the message buffer of received
 *
 * @param buf	buffer for read
 *
 * @param size	size of the buffer
 *
 * @retval 0	success
 *
 * @retval !0	error from kernel
 */
l4Ret_t l4MsgRead(void *buf, size_t size)
{
	if (msg_bytes < L4_ShortMsgBytes) {
		size_t len = MIN(size, L4_ShortMsgBytes - msg_bytes);
		memcpy(buf, msg_shrt + msg_bytes, len);
		msg_bytes += len;
		buf += len;
		size -= len;
		if (!size)
			return 0;
	}
	return l4ExtraRead(buf, size);
}

#if 0
/**
 * write into the capabiity buffer to be sent
 *
 * @param caps	capability pointers to write
 *
 * @param capnr	number of the capabilities
 *
 * @retval 0	success
 *
 * @retval !0	error from kernel
 */
l4Ret_t l4MsgCapWrite(const l4CPtr_t *caps, size_t capnr)
{
	if (msg_caps < L4_ShortMsgCPtrs) {
		size_t nr = MIN(capnr, L4_ShortMsgCaps - msg_caps);
		memcpy(msg_capshrt + msg_caps, caps, nr * sizeof(l4CPtr_t));
		msg_caps += nr;
		capnr += nr;
		capnr -= nr;
		if (!size)
			return 0;
	}
	return l4ExtraCapWrite(caps, capnr);
}
#endif

#include <libl4/buffer.h>
#include <libl4/captrs.h>

static int need_rewind = 1;

/**
 * rewind kernel's extra buffer
 */
void l4ExtraRewind(void)
{
	need_rewind = 1;
}

/**
 * write to kernel's extra message buffer
 *
 * @param buf	data to write
 *
 * @param size	size of the data
 *
 * @retval 0	success
 *
 * @retval !0	error from kernel
 */
l4Ret_t l4ExtraWrite(const void *buf, size_t size)
{
	l4Ret_t ret = l4Buffer_WriteFrags(Libl4_CapExtra, buf, size, need_rewind);
	need_rewind = 0;
	return ret;
}

#if 0
/**
 * write to kernel's extra capability buffer
 *
 * @param caps	capability pointers to write
 *
 * @param capnr	number of the capabilities
 *
 * @retval 0	success
 *
 * @retval !0	error from kernel
 */
l4Ret_t l4ExtraCapWrite(const l4CPtr_t *caps, size_t capnr)
{
	l4Ret_t ret = l4Buffer_WriteFrags(Libl4_CapExtraCaps,
			caps, capnr * sizeof(l4CPtr_t), need_caprewind);
	need_caprewind = 0;
	return ret;
}
#endif

/**
 * read from kernel's extra message buffer
 *
 * @param buf	buffer for read
 *
 * @param size	size of the data
 *
 * @retval 0	success
 *
 * @retval !0	error from kernel
 */
l4Ret_t l4ExtraRead(void *buf, size_t size)
{
	l4Ret_t ret = l4Buffer_ReadFrags(Libl4_CapExtra, buf, size, need_rewind);
	need_rewind = 0;
	return ret;
}
