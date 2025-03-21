SOURCE_FILES := $(wildcard *.c)
OBJ_FILES := $(SOURCE_FILES:.c=.o)
EXC_FILE := ds

CC := cc
CFLAGS := -Wall -Wextra -O2 -Istructures

ifeq ($(OS),Windows_NT)
    RM := del /Q
else
    RM := rm -f
endif

all: $(EXC_FILE)

$(EXC_FILE): $(OBJ_FILES)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ_FILES) $(EXC_FILE)

.PHONY: all clean

