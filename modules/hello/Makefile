
all:
	g++ -c hello.cpp
	g++ -shared -g -o ../hello.dll \
    -Wl,--export-all-symbols \
    -Wl,--enable-auto-import \
    -Wl,--whole-archive hello.o \
    -Wl,--no-whole-archive ../libsol.a