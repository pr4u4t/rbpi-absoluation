SUBDIRS := bcm xxHash 

.PHONY: $(SUBDIRS)

all: $(SUBDIRS) xxhash
clean: $(SUBDIRS) xxclean
xxhash: 
	cp xxHash/libxxhash.so* ../build
xxclean:
	rm -f ../build/libxxhash.so*

$(SUBDIRS):
	make -C $@ $(MAKECMDGOALS)
