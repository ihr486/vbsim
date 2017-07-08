CSRCS := $(wildcard *.c)
COBJS := $(CSRCS:%.c=%.o)
EXE := sim
CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -O2
LDFLAGS := -lm
.PHONY: all clean
all: $(EXE)
clean:
	-@rm -vf $(EXE) $(COBJS)
$(EXE): $(COBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
