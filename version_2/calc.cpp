#include "calc.h"

namespace NCalcWorker
{
	const std::string CalcWorker::calculate(const std::string &expression) {
		auto polish_expr_ = NPolish::PolishConvertor().conv_expr_to_pol_notation(expression);
		if (0 == polish_expr_.size()) {
			return "";
		}
		while (true) {
			auto it = polish_expr_.begin();
			while (polish_expr_.end() != it && it->get_type() == NToken::e_type_::DOUBLE) {
				++it;
			}
			if (polish_expr_.end() == it) {
				break;
			}
			if (NToken::e_type_::OPERATOR == it->get_type()) {
				if (polish_expr_.begin() == it ||
					polish_expr_.begin() == (it - 1)) {
					throw std::runtime_error("Bad syntax expression");
				}
				auto l_val = stod((*(it - 2)).get_value());
				auto r_val = stod((*(it - 1)).get_value());
				auto operation = *(it);
				*(it - 2) = this->execute_operation(l_val, r_val, operation.get_value());
				polish_expr_.erase(it - 1, it + 1);
			}
			else {
				throw std::runtime_error("Bad syntax expression");
			}
		}
		if (1 != polish_expr_.size()) {
			throw std::runtime_error("Bad syntax expression");
		}
		if (NToken::e_type_::DOUBLE != polish_expr_.front().get_type()) {
			throw std::runtime_error("Bad syntax expression");
		}
		return polish_expr_.front().get_value();
	}

	const NToken::Token CalcWorker::execute_operation(const double l_val, const double r_val, const std::string &operation) const {
		if (1 == operation.size()) {
			switch(operation[0]) {
				case '+' : return l_val + r_val;
				case '-' : return l_val - r_val;
				case '*' : return l_val * r_val;
				case '/' : 
							{
								if (0 == r_val) {
									throw std::runtime_error("Zero divide error: " + std::to_string(l_val) + operation + std::to_string(r_val));
								}
								return l_val / r_val;
							}
				case '^' : return pow(l_val, r_val);
			}	
		}
		throw std::runtime_error("Error in execute one operation: " + std::to_string(l_val) + operation + std::to_string(r_val));
	}

}