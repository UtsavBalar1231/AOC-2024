CFLAGS := -Wall -Werror -Wextra -pedantic -ggdb -g -Wno-gnu-pointer-arith
CC := clang
PROJECT := vec_tests

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

all: $(PROJECT)

$(PROJECT): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(PROJECT)
	./$(PROJECT)

.PHONY: clean

clean:
	rm -f $(PROJECT) $(OBJS) 

