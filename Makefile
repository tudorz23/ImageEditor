# Marius-Tudor Zaharia 313CAa 2022-2023 
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS = image_editor

build: $(TARGETS)

image_editor: main.o commands.o utils.o load.o save.o apply.o rotate.o
	$(CC) $(CFLAGS) main.o commands.o utils.o load.o save.o apply.o rotate.o -lm -o image_editor

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

commands.o: commands.c
	$(CC) -c $(CFLAGS) commands.c

utils.o: utils.c
	$(CC) -c $(CFLAGS) utils.c

load.o: load.c
	$(CC) -c $(CFLAGS) load.c

save.o: save.c
	$(CC) -c $(CFLAGS) save.c

apply.o: apply.c
	$(CC) -c $(CFLAGS) apply.c

rotate.o: rotate.c
	$(CC) -c $(CFLAGS) rotate.c

pack:
		zip -FSr 313CA_ZahariaMarius-Tudor_Proiect.zip Makefile *.c *.h *.pgm *.ppm

clean:
		rm -f *.o $(TARGETS)

.PHONY: pack clean
