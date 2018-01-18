CC = g++

FLAGS = -Wall -pthread

OBJ = main.o fonctions.o

EXEC = ip_tester_v3

all: $(OBJ)
	$(CC) $(FLAGS) -o $(EXEC) $(OBJ)
