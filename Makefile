ENVLAD := $(shell command -v ladcomp 2> /dev/null)
RUNLAD := $(shell command -v ladrun 2> /dev/null)

SDIR = ./src

CC = gcc
MPI = mpicc
MPIR = mpirun -np 16
LADC = ladcomp
LADR = ladrun

LADFLAGS = -env mpicc
CFLAGS = -Wall -g

all: sequential parallel

sequential: $(SDIR)/sequential.c
	$(CC) -o $@ $< $(CFLAGS)

parallel: $(SDIR)/parallel.c
ifndef ENVLAD
	$(MPI) -o $@ $< $(CFLAGS)
else
	$(LADC) $(LADFLAGS) $< -o $@ $(CFLAGS)
endif

run: parallel
ifndef RUNLAD
	$(MPIR) $<
else
	$(LADR) $<
endif

run_seq: sequential
	./$<

.PHONY: clean

clean:
	rm -rf sequential
	rm -rf parallel

