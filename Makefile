LUAINC = -I/usr/include/lua5.1
LIBXML = -I/usr/include/libxml2
IGRAPH = -I/usr/local/include/igraph
IGRAPHLIB = -L/usr/local/lib


all: testlib.exe genhtml.so 

testlib.exe: libForLua.c main.c
	gcc -w -g -std=c99 libForLua.c $(LUAINC) $(LIBXML) $(IGRAPH) $(IGRAPHLIB) -ligraph main.c -llua5.1 -lxml2 -o testlib.exe


genhtml.so: libForLua.c
	gcc -w -g -std=c99 libForLua.c $(LIBXML) $(LUAINC) $(IGRAPH) $(IGRAPHLIB) -ligraph -fpic -c -o genhtml.o
	gcc -w -O -std=c99 -shared -fpic -llua5.1 -lxml2 -ligraph -o genhtml.so genhtml.o

run:
	lua constellationGen.lua


clean:
	rm *~ *.o *.so *.exe
