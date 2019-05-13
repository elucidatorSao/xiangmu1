all:
	gcc -g service.c -o service -lpthread -lsqlite3
	gcc -g client.c -o client

clean:
	rm service client
 














