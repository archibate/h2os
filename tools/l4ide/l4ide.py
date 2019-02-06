#!/usr/bin/env python

def parse_ide(lines):
    while lines:
        line = next(lines)
        line = line.split()
        if not len(line):
            continue
        elif line[0] == 'proto':
            _, name = line
            do_proto(lines, name)

def do_proto(lines, name):
    while lines:
        line = next(lines)
        line.split()
        if not len(line):
            continue
        elif line[0] == 'end':
            break
        else:
            inout, type, name = line
            if inout == 'out':
                out_arg(type, name)
            if inout == 'in':
                out_arg(type, name)
