help.o : help.cpp 
	g++ -c help.cpp -o help.o
main.o : main.cpp
	g++ -c main.cpp -o main.o
task.o : task.cpp
	g++ -c task.cpp -o task.o
main : main.o task.o help.o
	g++ main.o task.o help.o -lpthread -o main
	make clear
clear : 
	rm -f *.o
