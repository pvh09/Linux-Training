CFLAGS = -Wall -Wno-write-strings -g

all:
	$(CC) $(CFLAGS) $(LDFLAGS) speedtest-cli.c -o speedtest-cli -lm -lpthread -lssl -lcrypto

debug_gdb: speedtest-cli.c $(SRC)
	$(CC) speedtest-cli.c -o gdb_debug -g -lm
	gdb ./gdb_debug
	rm -rf gdb_debug

clean:
	rm -f *.o speedtest-cli
