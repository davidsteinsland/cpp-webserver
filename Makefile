
all:
	g++ -Wall -g http/*.cpp net/unix/*.cpp utils/*.cpp webserver/*.cpp webserver/unix/*.cpp app.cpp -ldl -o webserver.out 2>make.log
	
	@echo "See make.log for output."

lib:
	g++ -c http/request.cpp http/response.cpp utils/ioutils.cpp utils/stringutils.cpp
	ar rvs modules/libsol.a request.o response.o ioutils.o stringutils.o
	rm request.o response.o ioutils.o stringutils.o
