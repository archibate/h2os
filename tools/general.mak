
include $(ROOT)/tools/config.mak
include $(ROOT)/tools/ecsta.mak
include $(ROOT)/tools/tools.mak
include $(ROOT)/tools/flags.mak
include $(ROOT)/tools/rules.mak

ifndef VERBOSE
.SILENT:
endif

.PHONY: default
default: all

ifndef GNOOBJS
include $(ROOT)/tools/objects.mak
include $(ROOT)/tools/deps.mak
endif
