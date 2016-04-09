LAD := $(command -v ladcomp)

SDIR = ./src

CC = gcc
MPI = mpicc
LAD = ladcomp

LADFLAGS = -env mpicc
CFLAGS = -Wall -g

all: sequential parallel

sequential: $(SDIR)/sequential.c
	$(CC) -o $@ $< $(CFLAGS)

parallel: $(SDIR)/parallel.c
ifndef LAD
	$(LAD) $(LADFLAGS) $< -o $@ $(CFLAGS)
endif
	$(MPI) -o $@ $< $(CFLAGS)


.PHONY: clean

clean:
	rm -rf sequential
	rm -rf parallel

