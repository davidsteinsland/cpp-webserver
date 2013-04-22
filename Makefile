
all:
	g++ -Wall -g -iquote $(CURDIR) http/*.cpp net/*.cpp utils/*.cpp webserver/*.cpp app.cpp -pthread -ldl -o solhttpd 2>make.log
	@echo "See make.log for output."

lib:
	g++ -iquote $(CURDIR) -shared -fPIC -c http/request.cpp http/response.cpp utils/ioutils.cpp utils/stringutils.cpp
	ar -rv modules/libsol.a request.o response.o ioutils.o stringutils.o
	rm request.o response.o ioutils.o stringutils.o

modules: lib
	make -C modules/basic/
	make -C modules/hello/

install:
	rm -rf build/
	mkdir build/
	cp solhttpd build/
	cp mimes.list build/
	cp -r public_html/ build/
	cp -r errors/ build/
	mkdir build/modules/
	cp -r modules/*.so build/modules/