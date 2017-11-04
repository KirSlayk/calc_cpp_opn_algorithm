#include <iostream>
#include <string>
#include "calc.h"

// void test() {
// 	std::vector<std::string> test_vec = {"","0","1.1","-1.1","-1.1 + 2.3 * 3",
// 	"-1.1 + 2.3 * 3 / 2.2","-1.1 + 2.3 * 3 / 0","-1.1+2.3*3 / 1","-1.1+-1.1",
// 	"-1.1+(-1.1)","(-3-3-3*2)",".2","-1.1 + 3 - 45 * (- 2+4.223 / ( -4.1 / ( 2 * 3.1 ) + 0.3 )) - 4 * ((1.1 - .1) / 2)",
// 	"3^4","2^3^4","-1.1 + 3 - 45 * (- 2+4.223^3 / ( -4.1 / ( 20 * 3.1 ) ^ 2 + 0.3 )) - 40 * ((10.1^(-2 + 1) - 1.123) / 2)"};
// 	for (auto &e: test_vec) {
// 		try {
// 			std::cout<<"> "<<e<<std::endl;
// 			auto ans = NCalcWorker::CalcWorker().calculate(e);
// 			std::cout<<"= "<<std::endl;
// 		}
// 		catch (const std::runtime_error &err) {
// 			std::cerr<<err.what()<<std::endl;
// 		}
// 	}
// }

extern "C" double calculate(const char *expr) {
	auto expression = std::string(expr);
	if (true == expression.empty()) {
		return 0;
	}
	try {
		auto ans = NCalcWorker::CalcWorker().calculate(expression);
		return stod(ans);
	}
	catch (const std::runtime_error &err) {
		std::cerr<<err.what()<<std::endl;
		throw err;
	}
	return 0;
}

// int main() {
// 	std::string expression = "";
// 	while (true != std::cin.eof()) {
// 		std::cout<<"> ";
// 		std::getline(std::cin, expression);
// 		if (true == expression.empty()) {
// 			break;
// 		}
// 		try {
// 			auto ans = NCalcWorker::CalcWorker().calculate(expression);
// 			std::cout<<"= "<<std::endl;
// 		}
// 		catch (const std::runtime_error &err) {
// 			std::cerr<<err.what()<<std::endl;
// 		}
// 	}
// 	return 0;
// }