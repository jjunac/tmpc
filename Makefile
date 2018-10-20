CFLAGS = -lpthread


all: exercice0-affinity exercice1-1-sujet exercice1-1 exercice1-2 exercice1-2-O1 exercice1-2-O2 exercice1-2-O3

exercice1-2-O1 : exercice1-2.c
	$(CC) $(CFLAGS) -O1 -o $@ $<

exercice1-2-O2 : exercice1-2.c
	$(CC) $(CFLAGS) -O2 -o $@ $<

exercice1-2-O3 : exercice1-2.c
	$(CC) $(CFLAGS) -O3 -o $@ $<

exercice1-3 : exercice1-3.c
	$(CC) $(CFLAGS) -DPADDING=$(PADDING) -o $@ $<

clean:
	rm -f $exercice0-affinity exercice1-1-sujet exercice1-1 exercice1-2 exercice1-2-O1 exercice1-2-O2 exercice1-2-O3 exercice1-3