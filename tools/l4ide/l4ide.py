#!/usr/bin/env python

def parse_ide(lines):
    while lines:
        line = next(lines)
        line = line.split()
        if not len(line):
            continue
        elif line[0] == 'proto':
            _, proto = line
            do_proto(lines, proto)

def do_proto(lines, proto):
    fh.writeln('l4Ret_t %s(', proto)
    while lines:
        line = next(lines)
        line.split()
        if not len(line):
            continue
        elif line[0] == 'end':
            break
        else:
            inout, type, name = line
            eval('do_' + inout + '_' + type)(name)

argtypes = {
        'word': 'l4Word_t',
        'cap': 'l4CPtr_t',
}

def do_out_word(name):
    fh.writeln('l4Word_t %s,' % name)
