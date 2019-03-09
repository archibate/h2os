ifdef DEPS
.PHONY: dep
dep: dep.out

CLEAN+=dep.out
dep.out: $(DEPS)
	$(EL) gen $@
	cat $^ > $@
	cat $^ | sed 's/\.o:/\.d:/' >> $@

include dep.out
endif
