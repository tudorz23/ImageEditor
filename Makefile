# Marius-Tudor Zaharia 313CAa 2022-2023 
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

TARGETS = image_editor

build: $(TARGETS)

image_editor: main.o commands.o utils.o load.o save.o apply.o rotate.o
	$(CC) $(CFLAGS) main.o commands.o utils.o load.o save.o apply.o rotate.o -lm -o image_editor

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

commands.o: commands.c commands.h
	$(CC) -c $(CFLAGS) commands.c

utils.o: utils.c utils.h
	$(CC) -c $(CFLAGS) utils.c

load.o: load.c load.h
	$(CC) -c $(CFLAGS) load.c

save.o: save.c save.h
	$(CC) -c $(CFLAGS) save.c

apply.o: apply.c apply.h
	$(CC) -c $(CFLAGS) apply.c

rotate.o: rotate.c rotate.h
	$(CC) -c $(CFLAGS) rotate.c

pack:
		zip -FSr 313CA_ZahariaMarius-Tudor_Proiect.zip Makefile *.c *.h *.pgm *.ppm

run: $(TARGETS)
	valgrind --leak-check=full ./$(TARGETS)

clean:
		rm -f *.o $(TARGETS)

.PHONY: pack clean
