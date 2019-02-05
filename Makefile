SUBDIRS := lib src

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)
clean: $(SUBDIRS)

$(SUBDIRS):
	make -C $@ $(MAKECMDGOALS)


.PHONY: all $(SUBDIRS)
