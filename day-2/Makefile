HELPERS_DIR := ../helpers/
CFLAGS := -Wall -Werror -Wextra -pedantic -ggdb -g -Wno-gnu-pointer-arith
CC := clang
PROJECT := day-2

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HELPERS_SRCS := $(wildcard $(HELPERS_DIR)*.c)
HELPERS_OBJS := $(HELPERS_SRCS:$(HELPERS_DIR)%.c=$(HELPERS_DIR)%.o)
LIBHELPERS := $(HELPERS_DIR)libhelpers.a

all: $(PROJECT)

$(PROJECT): $(OBJS) $(LIBHELPERS)
	$(CC) $(CFLAGS) $(OBJS) -L$(HELPERS_DIR) -lhelpers -o $@

$(LIBHELPERS): $(HELPERS_OBJS)
	ar rcs $@ $^

$(HELPERS_DIR)%.o: $(HELPERS_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(PROJECT)
	./$(PROJECT)

.PHONY: clean

clean:
	rm -f $(PROJECT) $(OBJS) $(HELPERS_OBJS) $(LIBHELPERS)
