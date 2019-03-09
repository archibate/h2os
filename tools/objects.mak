SRCS+=$(shell find $(SRCPATH) $(FINDFLAGS))
OBJS+=$(SRCS:%=%.o)
DEPS+=$(SRCS:%=%.d)
