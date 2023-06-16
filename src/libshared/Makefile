CC = g++
CPPFLAGS = -fPIC -pedantic -Wall -Wextra
LDFLAGS = -shared
#LIB = libshared.a
TARGETS = libshared.so
OBJ = libshared.o 

libshared.so: libshared.o
	$(CC) $(LDFLAGS) -o libshared.so libshared.o

libshared.o: libshared.cpp 
	$(CC) $(CPPFLAGS) -c libshared.cpp -o libshared.o 

# ${LIB}: ${OBJ}
# 	ar rs ${LIB} ${OBJ}
# 	ranlib ${LIB}


clean:
	rm -f *.o
	rm -f ${TARGETS}

