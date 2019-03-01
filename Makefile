CC=g++
CFLAGS=-c -framework OpenGl -I/usr/local/include -lglfw -lGLEW
all: compile

compile: main.o
	$(CC) main.o -o runner -framework OpenGl -I/usr/local/include -lglfw -lGLEW

main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp 

clean:
	rm *.o runner