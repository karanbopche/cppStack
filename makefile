output: main.o
	g++ main.o -o stack.out 

main.o: main.cpp stack.h
	g++ -c main.cpp -Wall  -Wextra

clean: 
	rm *.o stack.out
