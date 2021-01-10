CFLAGS=-g -Wall -pedantic

.PHONY: all
all: mt-httpd 

mt-httpd: mt-httpd.c
	gcc $(CFLAGS) -pthread -o $@ $^

st-httpd: st-httpd.c
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f st-httpd mt-httpd
