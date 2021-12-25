CC=g++
CFLAGS=-Wall -g --std=c++11
LDFLAGS=
LDLIBS=
EXEC= bcc

all: $(EXEC)

bcc: bcc.o
	$(CC) -o $@ $^ $(LDFLAGS)

bcc.o: bcc.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
cleanexec:
	rm -rf $(EXEC)
