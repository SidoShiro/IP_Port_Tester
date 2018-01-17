CC = g++

FLAGS = -Wall

OBJ = main.o fonctions.o

EXEC = ip_tester_v3

all: $(OBJ)
	$(CC) $(FLAGS) -o $(EXEC) $(OBJECTS)
