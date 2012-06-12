# Makefile - riff

DESTDIR := 
PREFIX := /usr

.PHONY: all
all: \
 	CDAL \
 	riff

.PHONY: riff
riff: CDAL
	cd riff/ && $(MAKE) -f riff.mk PREFIX=$(PREFIX)

.PHONY: CDAL
CDAL:
	cd CDAL/ && $(MAKE) -f CDAL.mk

.PHONY: clean
clean:
	cd riff/ && $(MAKE) -f riff.mk clean
	cd CDAL/ && $(MAKE) -f CDAL.mk clean
	rm -f packages/riff.deb

.PHONY: depends
depends:
	cd riff/ && $(MAKE) -f riff.mk depends
	cd CDAL/ && $(MAKE) -f CDAL.mk depends
	cd CDAL/ && $(MAKE) -f CDAL.mk depends

INSTDIR = $(DESTDIR)$(PREFIX)

.PHONY: install
install: all
	install -d $(INSTDIR)/bin
	install -m 755 riff/riff $(INSTDIR)/bin

	install -d $(INSTDIR)/lib
	install -m 644 CDAL/libcdal.so $(INSTDIR)/lib

	install -d $(INSTDIR)/share/riff/snowball
	install -m 644 config/riff.config $(INSTDIR)/share/riff/snowball/config
	ln -sf $(PREFIX)/share/riff/snowball/config $(INSTDIR)/share/riff

	mkdir -p doc/man/temp
	nroff -man doc/man/man1 | gzip > doc/man/temp/riff.1.gz
	install -d $(INSTDIR)/share/man/man1
	install -m 644 doc/man/temp/riff.1.gz $(INSTDIR)/share/man/man1
	rm -rf doc/man/temp

.PHONY: uninstall
uninstall:
	rm $(INSTDIR)/bin/riff
	rm $(INSTDIR)/lib/libcdal.so
	rm -rf $(INSTDIR)/share/riff
	rm $(INSTDIR)/share/man/man1/riff.1.gz
    
