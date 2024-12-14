CC = g++

FLAGS = -c

all: hello
	differ
hello: main.o tree/tree.o readrecover/recover.o readrecover/reader.o differential/differential.o
	$(CC) main.o tree/tree.o readrecover/recover.o readrecover/reader.o differential/differential.o -o differ

tree/tree.o: tree/tree.cpp tree/tree.h
	$(CC) $(FLAGS) tree/tree.cpp -o tree/tree.o
	
readrecover/recover.o: readrecover/recover.cpp readrecover/recover.h
	$(CC) $(FLAGS) readrecover/recover.cpp -o readrecover/recover.o

readrecover/reader.o: readrecover/reader.cpp readrecover/reader.h
	$(CC) $(FLAGS) readrecover/reader.cpp -o readrecover/reader.o
	
differential/differential.o: differential/differential.cpp differential/differential.h
	$(CC) $(FLAGS) differential/differential.cpp -o differential/differential.o

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp