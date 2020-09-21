CFLAGS+=-I../include/

all: client

client:client.o
	gcc $^ -o $@ $(CFLAGS)


clean:
	rm -rf *.0 client
