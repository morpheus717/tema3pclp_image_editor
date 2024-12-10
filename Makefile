# Copyright David Vlad-Mihail 311CA 2023-2024
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS = image_editor

build: $(TARGETS)

main: image_editor.c
	$(CC) $(CFLAGS) image_editor.c -o image_editor -lm

pack:
	zip -FSr 311CA_DavidVladMihail_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean