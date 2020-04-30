CC=gcc
CFLAGS=-g
SUBMIT=fd-exec leak signals pipes segfault
SUBMITC=$(SUBMIT:=.c)
SUPPLEMENT=need6 
SUPPLEMENTC=$(SUPPLEMENT:=.c)

all: $(SUBMIT) $(SUPPLEMENT)

%: %.c
	$(CC) $(CFLAGS) -o $@ $@.c

.PHONY: clean package

package: $(SUBMITC)
	tar -cvzf submit-me.tgz $(SUBMITC)

clean:
	rm -f $(SUBMIT) $(SUPPLEMENT) submit-me.tgz
