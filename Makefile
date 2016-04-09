SDIR = ./src

CC = gcc
MPI = mpicc
CFLAGS = -Wall -g

all: sequential parallel

sequential: $(SDIR)/sequential.c
	$(CC) -o $@ $< $(CFLAGS)

parallel: $(SDIR)/parallel.c
	$(MPI) -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -rf sequential
	rm -rf parallel

