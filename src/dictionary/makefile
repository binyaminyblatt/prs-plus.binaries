
build:
	rm -f *.o dictionary.*
	${CXX} unicode.c desaturate.c dictionary.cpp -o dictionary
	${STRIP} -s dictionary
	 
clean:
	rm -f *.o dictionary
