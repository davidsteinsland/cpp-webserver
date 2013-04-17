
all:
	g++ -Wall -g -iquote $(CURDIR) http/*.cpp net/unix/*.cpp utils/*.cpp webserver/*.cpp webserver/unix/*.cpp app.cpp -ldl -o solhttpd 2>make.log
	
	@echo "See make.log for output."

lib:
	g++ -iquote $(CURDIR) -shared -fPIC -c http/request.cpp http/response.cpp utils/ioutils.cpp utils/stringutils.cpp
	ar -rv modules/libsol-linux.a request.o response.o ioutils.o stringutils.o
	rm request.o response.o ioutils.o stringutils.o

modules: lib
	make -C modules/basic/
	make -C modules/hello/
