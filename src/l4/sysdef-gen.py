#!/usr/bin/env python

regs = 'bDSd'
vregs = ['ebx', 'edi', 'esi']

print('''#pragma once\n''')
print('''#define _$E(x) x''')

def mksys(nx, ny):
    print('''
#define _SYS%d%d(rett, func''' % (nx, ny) + \
        ''.join([', t%d, x%d' % (i,i) for i in range(nx)]  + \
                [', u%d, y%d' % (i,i) for i in range(ny)]) + \
        ''') \\
rett sys_##func(''' + \
        ','.join([' t%d x%d' % (i,i) for i in range(nx)]  + \
                 [' u%d*y%d' % (i,i) for i in range(ny)]) + \
        ''') \\
{ \\
	rett res; \\
	asm volatile ( \\''')

    if ny >= 4 or nx >= 4:
        print('''\t\t"push %%ebp\\n" \\''')
    if nx >= 4:
        print('''\t\t"mov %%edx, %%ebp\\n" \\''')

    print('''\t\t"call libl4_do_syscall\\n" \\''')

    if ny >= 4:
        print('''\t\t"mov %%edx, %%ebp\\n" \\''')
    if ny >= 4 or nx >= 4:
        print('''\t\t"pop %%ebp\\n" \\''')


    print('''\t\t: "=a" (res) \\
''' + ''.join('\t\t, "=%s" (*y%d) \\\n' % (regs[i], i) for i in range(ny)) + \
'''\t\t: "a" (_$E(_SYS_##func)) \\''' + \
      ''.join('\n\t\t, "%s" (x%d) \\'   % (regs[i], i) for i in range(nx)) + '''
                : "ecx", ''' + ''.join('"%s", ' % x for x in vregs[max(ny,nx):]) + '''"cc", "memory"); \\
	return res; \\
}''')

for nx in range(5):
    for ny in range(5):
        mksys(nx, ny)
