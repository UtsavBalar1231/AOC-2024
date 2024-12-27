FLAGS := -Wall -Wextra -g -pedantic

HELPERS_DIR = ../helpers

all: day_1 

helpers.o: $(HELPERS_DIR)/helpers.c $(HELPERS_DIR)/helpers.h
	cc -c $(HELPERS_DIR)/helpers.c $(FLAGS)

day_1: day-1.o helpers.o
	cc -o day-1 day-1.o helpers.o $(FLAGS)

day-1.o: day-1.c $(HELPERS_DIR)/helpers.h
	cc -c day-1.c $(FLAGS)

clean:
	rm -rf day-1 day-1.o helpers.o
