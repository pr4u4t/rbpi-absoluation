SUBDIRS := lib src doc

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)
clean: $(SUBDIRS)

$(SUBDIRS):
	make -C $@ $(MAKECMDGOALS)


.PHONY: all $(SUBDIRS)
