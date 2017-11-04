#include "calc.h"

namespace NCalculation
{
	const double CalcWorker::calc(const std::string &expression) {
		this->expression_ = expression;
		return this->calc();
	}
	
	const double CalcWorker::calc() {
		for (auto iter = this->expression_.begin(); iter != this->expression_.end(); ++iter) {
			if (true == isdigit(*iter)) {
				this->push_back_operand(iter);
			}
			else if (true == this->isoperator(*iter)) {
				this->push_back_operator(iter);
			}
			else if (' ' != *iter) {
				throw std::runtime_error("CalcWorker::calc(): not valid input expression.");
			}
		}
		while (this->operators_.size() > 0) {
			this->operator_from_stack_to_opn();
		}
		return this->calc_opn(this->expression_opn_);
	}

	const double CalcWorker::calc_opn(expression_opn_list &opn) const {
		std::vector<double> values_vec(0);
		for (auto iter = opn.begin(); opn.end() != iter; ++iter) {
			if (true == this->is_float(*iter)) {
				double value = 0;
				std::stringstream(*iter) >> value;
				values_vec.push_back(value);
			}
			else if (true == this->isoperator(*iter)) {
				if (values_vec.size() < 2) {
					throw std::runtime_error("CalcWorker::calculation_opn(): not valid input expression.");
				}
				auto second_val = values_vec.back();
				values_vec.pop_back();
				auto first_val = values_vec.back();
				values_vec.pop_back();
				auto local_res = this->coumputation(first_val, second_val, (*iter)[0]);
				values_vec.push_back(local_res);
			}
		}
		if (values_vec.size() > 1) {
			throw std::runtime_error("CalcWorker::calculation_opn(): not valid input expression.");
		}
		return 0 == values_vec.size() ? 0 : values_vec[0];
 	}

	void CalcWorker::operator_from_stack_to_opn() {
		std::stringstream sstream;
		sstream<<this->operators_.top();
		this->expression_opn_.push_back(sstream.str());
		this->operators_.pop();
	}

	void CalcWorker::stack_permutation(char new_oper) {
		auto new_oper_type = this->get_operators_type(new_oper);
		if (new_oper_type >= e_operators_type::LOW_T &&
			e_operators_type::HIGH_T != new_oper_type) {
			while (	0 != this->operators_.size() &&
					new_oper_type <= this->get_operators_type(this->operators_.top())) {
				this->operator_from_stack_to_opn();
			}
		}
		if (e_operators_type::RIGHT_BKT == new_oper_type &&
			0 != this->operators_.size()) {
			while (e_operators_type::LEFT_BKT != this->get_operators_type(this->operators_.top())) {
				this->operator_from_stack_to_opn();
			}
			this->operators_.pop();
			return;
		}
		this->operators_.push(new_oper);
	}

	const double CalcWorker::coumputation(double first, double second, char operation) const {
		// std::cout<<"coumputation "<<first<<operation<<second<<std::endl;
		switch(operation) {
			case '+': return first + second;
			case '-': return first - second;
			case '*': return first * second;
			case '/': 
						{
							if (0 == second) {
								throw std::runtime_error("Zero divide error " + std::to_string(first) + std::string(1, operation) + std::to_string(second));
							}
							return first / second;
						}
			case '^': return pow(first, second);
			default: 
				{
					std::stringstream err;
					err<<"CalcWorker::coumputation(): coumputation error. There is no such operation: "<<operation;
					throw std::runtime_error(err.str());
				}
		}
	}


	int CalcWorker::get_operators_type(char operand) const {
		switch(operand) {
			case '(':
				{
					return e_operators_type::LEFT_BKT;
				}
			case ')':
				{
					return e_operators_type::RIGHT_BKT;
				}
			case '+': case '-':
				{
					return e_operators_type::LOW_T;
				}
			case '*': case '/':
				{
					return e_operators_type::MIDLE_T;
				}
			case '^':
				{
					return e_operators_type::HIGH_T;
				}
			default:
				{
					return -1;
				}
		}
	}


	bool CalcWorker::is_float(const std::string& str) const {
	    std::istringstream iss(str);
	    float ans;
	    iss >> std::noskipws >> ans; 
	    return iss.eof() && !iss.fail(); 
	}

	bool CalcWorker::isoperator(const char symbol) const {
		std::vector<char> operators = {'+', '-', '*', '/', '^', '(', ')'};
		return std::find(operators.begin(), operators.end(), symbol) != operators.end();
	}

	bool CalcWorker::isoperator(std::string &str) const {
		std::vector<char> operators = {'+', '-', '*', '/', '^', '(', ')'};
		auto end_pos = std::remove(str.begin(), str.end(), ' ');
		str.erase(end_pos, str.end());
		return !str.empty() && 1 == str.size() && std::find(str.begin(), str.end(), str[0]) != operators.end();
	}

	bool CalcWorker::is_valid_seq_operators(char prev, char last) const {
		auto prev_type = this->get_operators_type(prev);
		auto last_type = this->get_operators_type(last);
		if (prev_type >= e_operators_type::LOW_T &&
			last_type >= e_operators_type::LOW_T) {
			return false;
		}
		if (e_operators_type::RIGHT_BKT == prev_type &&
			e_operators_type::LEFT_BKT == last_type) {
			return false;
		}
		if (prev_type >= e_operators_type::LOW_T &&
			e_operators_type::RIGHT_BKT == last_type) {
			return false;
		}
		if (prev_type == e_operators_type::LEFT_BKT &&
			last_type >= e_operators_type::LOW_T &&
			'-' != last) {
			return false;
		}
		return true;
	}


	void CalcWorker::push_back_operand(std::string::iterator &iter) {
		auto start_substr(iter);
		auto is_double = false;
		while (this->expression_.end() != iter + 1) {
			if (true == isdigit(*(iter + 1))) {
				++iter;
			}
			else if (false == is_double &&
					'.' == *(iter + 1)) {
				++iter;
				is_double = true;
			}
			else if ('.' == *(iter + 1)) {
				throw std::runtime_error("CalcWorker::push_back_operand(): not valid input expression.");
			}
			else {
				break;
			}
		}
		std::string value(start_substr, iter + 1);
		if (0 != this->operators_.size() &&
			'~' == this->operators_.top()) {
			value = "-" + value;
			this->operators_.pop();
		}
		this->expression_opn_.push_back(value);
	}

	void CalcWorker::push_back_operator(std::string::iterator &iter) {
		if ('-' == *iter) {
			if (0 == this->expression_opn_.size()) {
				if (this->expression_.end() != iter + 1) {
					if ('(' == *(iter + 1)) {
						this->expression_opn_.push_back("0");
						this->operators_.push('-');
						return;
					}
				}
				this->operators_.push('~');
				return;
			}
			else if ('(' == *(iter - 1)) {
				this->operators_.push('~');
				return;
			}
		}
		
		if (this->expression_.begin() != iter) {
			auto is_prev_operator = this->isoperator(*(iter - 1));
			if (true == is_prev_operator) {
				auto prev_operator = *(iter - 1);
				if (false == this->is_valid_seq_operators(prev_operator, *iter)) {
					throw std::runtime_error("CalcWorker::push_back_operator(): not valid input expression.");
				}
			}
		}
		this->stack_permutation(*iter);
	}
}