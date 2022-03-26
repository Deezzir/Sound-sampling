ifeq ($(shell echo | gcc -E -dM - | grep -c aarch64),1)
	BINARIES:=vol0 vol1 vol2 vol3 vol4 vol5
else
	BINARIES:=vol0 vol1 vol2
endif

LIBRARIES = vol_createsample.o
CCOPTS    = -g -O1 #-fopt-info-vec-all -fno-lto  -fno-tree-vectorize -fno-tree-loop-vectorize 
CCSVEOPT  = -march=armv8-a+sve2 

all:	${BINARIES}

vol_createsample.o:	vol_createsample.c
	gcc -c ${CCOPTS} vol_createsample.c 

vol0:	vol0.c vol_createsample.o vol.h
	gcc ${CCOPTS} vol0.c vol_createsample.o -o vol0

vol1:	vol1.c vol_createsample.o vol.h
	gcc ${CCOPTS} vol1.c vol_createsample.o -o vol1

vol2:	vol2.c vol_createsample.o vol.h
	gcc ${CCOPTS} vol2.c vol_createsample.o -o vol2

ifeq ($(shell echo | gcc -E -dM - | grep -c aarch64),1)
vol3:	vol3.c vol_createsample.o vol.h
	gcc ${CCOPTS} vol3.c vol_createsample.o -o vol3

vol4:	vol4.c vol_createsample.o vol.h
	gcc ${CCOPTS} vol4.c vol_createsample.o -o vol4

vol5:   vol5.c vol_createsample.o vol.h
	gcc ${CCOPTS} ${CCSVEOPT} vol5.c vol_createsample.o -o vol5
endif

clean:	
	rm ${BINARIES}  || true
	rm ${LIBRARIES} || true

