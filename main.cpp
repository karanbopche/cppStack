#include "stack.h"
#include "color.h"
#include <cstdlib>
#include <typeinfo>
#include <fstream>

using namespace std;

// flags for various functionalities.....
int maxStackSize = 5;
bool stackChanged = false;
bool stackLoaded = false;
// stacks.........
Stack<char> cStack;
Stack<int> iStack;
Stack<float> fStack;
Stack<double> dStack;

// function prototypes..........
void mainMenu();
void optionMenu(const char* option);
void save();
void loadFromFile();
template<typename type>
const char * checkType(type item);
template <typename T, typename Item>
void stackApp(T& stack, Item item);


int main() {
	char selection=0;	// default invalid operation....
	int failedAttempts = 0;
	char cItem=0;
	int iItem=0;
	float fItem=0;
	double dItem=0;
	
	while(1) {
	loop:
		cout<<"\x1b[1J\x1b[H\n";	// escape sequence to clear display....
		mainMenu();
		bool invalid = false;	// indicate selection was invalid....
		
		switch(selection) {
			case 0:	// dummy case
			break;
			case '1':	// character stack.......
				stackApp(cStack, cItem);
				selection = 0;
				goto loop;

			case '2':	// integer stack........
				stackApp(iStack, iItem);
				selection = 0;
				goto loop;

			case '3':	// float stack...........
				stackApp(fStack, fItem);
				selection = 0;
				goto loop;
			break;
			case '4':	// double stack........
				stackApp(dStack, dItem);
				selection = 0;
				goto loop;
			break;
			case '5':	// change StackSize.....
				cout<<TC_GREEN<<"Current Stack Size : "<<maxStackSize<<C_RST<<endl;
				cout<<TC_MAGENTA<<"Enter new Stack size : "<<C_RST;
				cin>>maxStackSize;
				cStack.resize(maxStackSize);
				iStack.resize(maxStackSize);
				fStack.resize(maxStackSize);
				dStack.resize(maxStackSize);
				selection = 0;
				goto loop;
			break;
			case '6':	// save to file......
				save();
			break;
			case '7':	// load from file........
				loadFromFile();
			break;
			case '8':	// exit program and save stack data...........
				if(!(cStack.isEmpty() && iStack.isEmpty() && fStack.isEmpty() && dStack.isEmpty()) && stackChanged) {
					char choice;
					if(stackChanged)
						cout<<TC_RED<<"stack data has been changed"<<endl;
					else
						cout<<TC_RED<<"Stack is not empty."<<endl;
						
					cout<<TC_MAGENTA<<"Would you like to save data to file ? [y/n] : "<<C_RST;
					while(1) {
						cin>>choice;
						if('y' == choice || 'Y' == choice) {
							save();
							break;
						}
						else if('n' ==  choice || 'N' == choice)
							break;
						else
							cout<<TC_RED<<"Invalid option"<<C_RST;
						failedAttempts++;
						if(failedAttempts > 5)
							break;
	 				}
	 			}
				cout<<TC_GREEN<<"Thank you for using our application."<<C_RST<<endl;
				return 0;
			default:
				invalid = true;
			break;
		}
		// count invalid attempts..........
		if(invalid) {
			failedAttempts++;
			cout<<TC_RED<<"Enter valid option"<<C_RST<<endl;;
			if(failedAttempts > 5)
				return -1;
			if(failedAttempts > 4)
				cout<<TC_RED"otherwise program will terminate."<<C_RST<<endl;
		}
		else
			failedAttempts = 0;

		cout<<TC_GREEN<<"select option : "<<C_RST;
		cin>>selection;
	}
	return 0;
}

// stack handler template............
template <typename T, typename Item>
void stackApp(T& stack, Item item) {
	char selection=0;
	int failedAttempts = 0;	
	const char *curStack = checkType(item);
	
	if((!stack.isEmpty())) {
		if(!stackLoaded) {
			cout<<"\x1b[1J\x1b[H\n";	// escape sequence to clear display....
			cout<<TC_RED<<"Previous data is on stack"<<endl;
			cout<<TC_BLUE<<"do you want to delete previous data : "<<C_RST;
			char choice;
			while(1) {
				cin>>choice;
				if('y' == choice || 'Y' == choice){
					stack.clear();
					break;
				}
				else if('n' ==  choice || 'N' == choice)
					break;
				else
					cout<<TC_RED<<"Invalid option"<<C_RST;
				failedAttempts++;
				if(failedAttempts > 5)
					break;
			}
			failedAttempts = 0;
		}
	stackLoaded = false;	// reset flag....
	}
	while(1) {
		bool failed = false;
		cout<<"\x1b[1J\x1b[H\n";	// escape sequence to clear display....
		optionMenu(curStack);
		switch(selection) {
		
			case 0:	// dummy 
			break;
			case '1':	// push data........
				cout<<TC_MAGENTA<<"Enter the item : "<<C_RST;
				cin>>item;
				try{
					stack.push(item);
					stackChanged = true;
					selection = 0;
					continue;	// continue in next iteration...
				}
				catch(...) {
					cout<<TC_RED<< "stack overflow occured"<<C_RST<<endl;
				}
			break;
			case '2':	// pop data.......
				try{
					item = stack.pop();
					cout<<TC_MAGENTA<<"popped item is "<<C_RST<<item<<endl;
					stackChanged = true;
				}
				catch(...) {
					cout<<TC_RED<<" stack underflow occured"<<C_RST<<endl;
				}
			break;
			case '3':
				stack.display();
			break;
			case '4':
				stack.clear();
			break;
			case '5':
				return;
			break;
			default:
				failed = true;
		}
		if(failed) {
			failedAttempts++;
			cout<<TC_RED<<"Enter valid option"<<C_RST<<endl;;
			if(failedAttempts > 5)
				return;
			if(failedAttempts > 4)
				cout<<TC_RED<<"otherwise program will terminate."<<C_RST<<endl;
		}
		else
			failedAttempts = 0;
			
		cout<<TC_GREEN<<"select option : "<<C_RST;
		cin>>selection;
	}
}

// template type finder..........
template<typename type>
const char * checkType(type item) {
	const char ty = *typeid(item).name();
	if(ty == 'i')
		return "Integer";
	else if(ty == 'c')
		return "Character";
	else if(ty == 'f')
		return "Float";
	else if(ty == 'd')
		return "Double";

	return "User defined";
}

// save stack data to text file.........
void save() {
	ofstream file;
	char name[20];
	cout<<"Enter file name to save data to :  ";
	cin>>name;
	file.open(name);
	// store stack data in file.......
	file<<cStack.size()<<endl;
	while(!cStack.isEmpty())
		file<<cStack.pop()<<" ";
	
	file<<endl<<iStack.size()<<endl;
	while(!iStack.isEmpty())
		file<<iStack.pop()<<" ";
	
	file<<endl<<fStack.size()<<endl;
	while(!fStack.isEmpty())
		file<<fStack.pop()<<" ";
	
	file<<endl<<dStack.size()<<endl;
	while(!dStack.isEmpty())
		file<<dStack.pop()<<" ";
	
	file.close();
}

// load data from text file..........
void loadFromFile() {
	ifstream file;
	char name[20];
	cout<<"Enter file name to open : ";
	cin>>name;
	file.open(name);
	try
	{
		int size;
		file>>size;
		if(size > cStack.maxSize())
			cStack.resize(size+1);
		while(size--){
			char item;
			file>>item;
			cStack.push(item);
		}

		file>>size;
		if(size > iStack.maxSize())
			iStack.resize(size+1);
		while(size--){
			int item;
			file>>item;
			iStack.push(item);
		}

		file>>size;
		if(size > fStack.maxSize())
			fStack.resize(size+1);
		while(size--){
			float item;
			file>>item;
			fStack.push(item);
		}
	
		file>>size;
		if(size > dStack.maxSize())
			dStack.resize(size+1);
		while(size--){
			double item;
			file>>item;
			dStack.push(item);
		}
		stackLoaded = true;
		cout<<TC_MAGENTA<<"File loaded successfully."<<C_RST<<endl;
	}
	catch(...) {
		cStack.clear();
		iStack.clear();
		fStack.clear();
		dStack.clear();
		cout<<TC_RED<<"failed to load from file"<<C_RST<<endl;
	}
}

// welcome menu.............
void mainMenu() {
	cout<<TC_MAGENTA<<"\t\t****************************"<<endl;
	cout<<TC_GREEN<<"\t\twelcome to stack application"<<endl;
	cout<<TC_MAGENTA<<"\t\t****************************"<<endl<<endl;
 	cout<<TC_MAGENTA<<"select one of the stack from following list"<<endl;
 	cout<<TC_BLUE<<	"1) Character stack"<<endl;
 	cout<<		"2) Interger  stack"<<endl;
 	cout<<		"3) Float     stack"<<endl;
 	cout<<		"4) Double    stack"<<endl;
 	cout<<		"5) Resize    stack"<<endl;
 	cout<<		"6) Save      stack"<<endl;
 	cout<<		"7) Load      stack"<<endl;
 	cout<<		"8) Exit      stack"<<C_RST<<endl<<endl;
}

// stack menu.............
void optionMenu(const char* option) {
	cout<<TC_GREEN<<"\t\tWelcome to "<<TC_CYAN<<option<<TC_GREEN<<" stack menu."<<endl<<endl;
	cout<<TC_MAGENTA<<"Select one of the following operation."<<endl;
	cout<<TC_BLUE<<	"1) Push"<<endl;
	cout<<		"2) Pop"<<endl;
	cout<<		"3) Display"<<endl;
	cout<<		"4) Clear"<<endl;
	cout<<		"5) Exit"<<C_RST<<endl<<endl;
}

