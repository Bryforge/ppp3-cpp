#include "PPP.h"

int main()	// read first and second name
{
	cout << "Please enter your first and second names\n";
	string first;
	string second;
	cin >> first >> second;		// read two strings
	
	auto name = first + ' ' + second;	// concatenate strings
	cout << "Hello, " << name << '\n';
}
