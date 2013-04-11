
all:
	#g++ -o webserver /c/mingw/lib/libws2_32.a -lwsock32 app.cpp *.o
	
	g++ **/*.cpp app.cpp -lwsock32 -o webserver 2>make.log
	
	@echo "See make.log for output."

classes:
	g++ -c **/*.cpp 2>make.log
	
	@echo "See make.log for output."
