#include "PPP.h"

int main()
{
	int age = -1;
	cout << "Please enter your age:\n";
	cin >> age;	// >> reads an integer into age
	cout << "Please enter your name:\n";
	string name;
	cin >> name; 	// >> reads a string into name
	
	int a2 = age+2;	// + adds integers
	auto n2 = name + " Jr.";	// + concatenates strings
	
	int a3 = age-2;	// - subtracts integers
	// string n3 = name - "Jr.";	// error: - isn't defined for strings
	
	cout << "Hello, " << n2 << '\n';
	cout << "Age: " << age << '\n';
}
