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
	
	dpkg-deb --build debian packages/riff.deb || error
	rm -rf debian
	rm -rf doc/man/temp
	
.PHONY: install
install: clean all
	mv riff/riff /usr/bin/riff
	mv CDAL/libcdal.so /usr/lib/libcdal.so
	mkdir /usr/share/riff
	mkdir /usr/share/riff/snowball
	cp config/riff.config /usr/share/riff/snowball/config
	ln -s /usr/share/riff/snowball/config /usr/share/riff
	mkdir doc/man/temp
	nroff -man doc/man/man1 > doc/man/temp/riff.1
	gzip doc/man/temp/riff.1
	cp -r doc/man/temp/riff.1.gz /usr/share/man/man1
	rm -rf doc/man/temp

.PHONY: uninstall
uninstall: clean
	rm /usr/bin/riff
	rm /usr/lib/libcdal.so
	rm -rf /usr/share/riff
	rm /usr/share/man/man1/riff.1.gz
    
