menuhide.so:
	gcc -shared -fPIC -g -o menuhide.so menuhide.c `pkg-config pidgin --libs --cflags`

all: menuhide.so

install: menuhide.so
	cp menuhide.so /home/novel/.purple/plugins

clean:
	rm -f menuhide.so
