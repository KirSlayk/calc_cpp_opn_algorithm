/*
    C++11, main.cpp
    Purpose: 	implement the calculator by the reverse Polish algorithm;

    @author Kirill Slaykovskiy BMSTU
    @version 1.0 21/10/17 
*/
#include <iostream>
#include "calc.h"

const std::string double_to_string(double num) {
	auto string_ans = std::to_string(num);
	if (std::string::npos != string_ans.find('.')) {
		while('0' == string_ans.back()) {
			string_ans.pop_back();
		}
		if ('.' == string_ans.back()) {
			string_ans.pop_back();
		}
	}
	return string_ans;
}

int main() {
	std::string expression = "";
	while (!std::cin.eof()) {
		std::getline(std::cin, expression);
		if (true == expression.empty()) {
			return 0;
		}
		try {
			auto ans = NCalculation::CalcWorker(expression).calc();
			std::cout<<double_to_string(ans)<<std::endl;
		}
		catch(const std::runtime_error &err) {
			std::cout<<err.what()<<std::endl;
		}
	}
	return 0;
}