all:	ca.c ca.h ea.c ea.h ca_params.h params.h main.c
	gcc -O3 -std=c99 -o 2dca_evol ca.c ea.c main.c

run:
	./2dca_evol

clean:
	rm -f 2dca_evol
