#include<iostream>

#include "bgfx/bgfx.h"

int main() {
	bgfx::init();
	std::cout << "hello world!" << std::endl;
	while (true) {}
}