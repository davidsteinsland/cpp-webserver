
all:
	#g++ -o webserver /c/mingw/lib/libws2_32.a -lwsock32 app.cpp *.o
	
	g++ **/*.cpp app.cpp -lwsock32 -o webserver 2>make.log
	
	@echo "See make.log for output."

lib:
	g++ -c http/request.cpp http/response.cpp utils/ioutils.cpp utils/stringutils.cpp
	ar rvs modules/libsol.a request.o response.o ioutils.o stringutils.o
	rm request.o response.o ioutils.o stringutils.o