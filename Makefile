# Makefile - riff

.PHONY: all
all: \
 	CDAL \
 	riff


.PHONY: riff
riff: CDAL
	cd riff/ && $(MAKE) -f riff.mk

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

.PHONY: debian
debian:	clean all 
	mkdir -p debian/DEBIAN
	mkdir -p debian/usr/bin
	mkdir -p debian/usr/lib
	mkdir -p debian/usr/share/doc/riff
	mkdir -p debian/usr/share/man/man1
	mkdir doc/man/temp
	nroff -man doc/man/man1 > doc/man/temp/riff.1
	gzip doc/man/temp/riff.1
	
	cp packages/control debian/DEBIAN/
	cp riff/riff debian/usr/bin/
	cp LICENSE debian/usr/share/doc/riff/
	cp README debian/usr/share/doc/riff/

	cp CDAL/libcdal.so debian/usr/lib/
	cp -r doc/man/temp/riff.1.gz debian/usr/share/man/man1
	
	fakeroot dpkg-deb --build debian packages/riff.deb || error
	rm -rf debian
	rm -rf doc/man/temp

DESTDIR := 
PREFIX := /usr
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
    
