menuhide.so:
	gcc -shared -fPIC -o menuhide.so menuhide.c `pkg-config pidgin --libs --cflags`

all: menuhide.so

install: menuhide.so
	cp menuhide.so ~/.purple/plugins

clean:
	rm -f menuhide.so
