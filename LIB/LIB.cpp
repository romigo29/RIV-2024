#include <iostream>

extern "C"
{
	
	int __stdcall module(int a) {
		return (a < 0) ? -a : a; 
	}

	int __stdcall rest(int a, int b) {
		return a % b;
	}

	void __stdcall writestr(char* str) {
		std::cout << str << std::endl;
	}

	void __stdcall writechar(char ch) {
		std::cout << ch << std::endl;
	}

	void __stdcall writeint(int n) {
		std::cout << n << std::endl;
	}

	void __stdcall writebool(bool b) {
		if (b == false) {
			std::cout << "false" << std::endl;
		}
		else {
			std::cout << "true" << std::endl;
		}
	}
}