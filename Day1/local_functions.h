#ifndef LOCAL_HEADER_H
#define LOCAL_HEADER_H

#include "../includes.cpp"

void local_function() {
	std::cout << "Hello from local function!" << std::endl;
};

struct SafeInput {
	bool right;
	int num;
};

SafeInput StringToInput(string input){
	SafeInput res;
	res.right = input[0] == 'R'; 
	string numeric_part = input.substr(1);
	res.num = StringToNumber<int>(numeric_part);
	return res;
}

#endif