build: client-prog server-prog

server-prog: server.c
	gcc -o server-prog server.c `pkg-config --libs --cflags gio-2.0 gio-unix-2.0`

client-prog: client.c
	gcc -o client-prog client.c `pkg-config --libs --cflags gio-2.0 gio-unix-2.0`

clean:
	rm client-prog
	rm server-prog
