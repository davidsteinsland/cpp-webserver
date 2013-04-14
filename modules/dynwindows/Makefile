
all:
	g++ -c dynwindows.cpp
	g++ -shared -g -o ../dynwindows.dll \
    -Wl,--export-all-symbols \
    -Wl,--enable-auto-import \
    -Wl,--whole-archive dynwindows.o \
    -Wl,--no-whole-archive ../libsol.a