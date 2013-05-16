OBJ = main.o
CFLAGS = -Wall -std=c89 -ggdb

all: bin

bin: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main

clean:
	rm main $(OBJ)