#ifndef __STACK_H__
#define __STACK_H__

#include <iostream>

template<class s>
class Stack {
	s *stack;
	int capacity;
	int top;	// top of stack....

public:
	Stack();
	Stack(int size);
	Stack(Stack<s>& copy);
	~Stack();

	bool push(s);
	s pop();
	s peek();	// peek at top...
	void display();
	bool isEmpty();
	void clear();	// clear stack.......
	int size();	// total elements in stack....
	void resize(int size);
	int maxSize();	// capacity of stack...
};

template<class s>
Stack<s>::Stack() {
	capacity = 5;
	top = -1;
	stack = new s[capacity];
}

template<class s>
Stack<s>::Stack(int size) {
	capacity = size;
	top = -1;
	stack = new s[capacity];
}

template<class s>
Stack<s>::Stack(Stack<s>& copy) {
	capacity = copy.capacity;
	top = copy.top;
	stack = new s[copy.capacity];
	for(int i=0; i<=top;i++)
		stack[i] = copy.stack[i];
}

template<class s>
Stack<s>::~Stack() {
	delete[] stack;
}

template<class s>
void Stack<s>::resize(int size) {
	s *newStack;
	capacity = size;
	newStack = new s[capacity];
	for(int i=0;i<=top && i<capacity;i++)
		newStack[i] = stack[i];
	delete[] stack;
	if(top+1 >= capacity)	// limit top to capacity.
		top = capacity-1; 
	stack = newStack;
}

template<class s>
void Stack<s>::display() {
	if(top < 0)
		std::cout<<"Stack empty"<<std::endl;
	for(int i=0;i<=top;i++)
		std::cout<<"stack["<<i<<"] = "<<stack[i]<<std::endl;
}

template<class s>
bool Stack<s>::isEmpty() {
	return (top < 0);
}

template<class s>
void Stack<s>::clear() {
	top = -1;
}

template<class s>
int Stack<s>::size() {
	return top+1;
}

template<class s>
int Stack<s>::maxSize() {
	return capacity;
}

template<class s>
bool Stack<s>::push(s item) {
	top++;
	if(top >= capacity){
		top--;
		throw "stack overflow";
	}
	else
		stack[top] = item;
	return true;
}

template<class s>
s Stack<s>::pop() {
	if(top < 0)
		throw "stack underflow";
	return stack[top--];
}

template<class s>
s Stack<s>:: peek() {
	s temp;	// empty object....
	if(top >= 0)
		temp =  stack[top];
	return temp;
}

#endif
