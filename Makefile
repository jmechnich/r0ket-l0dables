all:
	ln -sf ../../../flame.c    r0ket/firmware/l0dable
	ln -sf ../../../flamebow.c r0ket/firmware/l0dable
	ln -sf ../../../flamergb.c r0ket/firmware/l0dable
	if grep -q lcdPrintInt r0ket/firmware/l0dable/dbgmesh.c; then \
	  cd r0ket && patch -p1 < ../lcdPrintInt.diff; fi
	cd r0ket/firmware && $(MAKE)
	cd r0ket/firmware && $(MAKE) l0dables
