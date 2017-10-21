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
		while (this->operands_.size() >= 2 &&
			   0 != this->operators_.size()) {
			this->top_calculation();
		}
		if (0 == this->operands_.size() ||
			(0 != this->operands_.size() && 
			0 != this->operators_.size()) ||
			(1 != this->operands_.size() &&
			0 == this->operators_.size())) {
			throw std::runtime_error("CalcWorker::calc(): not valid input expression.");
		}
		auto ans = this->operands_.back();
		this->operands_.pop_back();
		return ans;
	}

	void CalcWorker::interim_calc(char new_oper) {
		auto new_oper_type = this->get_operators_type(new_oper);
		if (new_oper_type >= e_operators_type::LOW_T &&
			new_oper_type < e_operators_type::HIGH_T) {
			while (0 != this->operators_.size() &&
				new_oper_type <= this->get_operators_type(this->operators_.top())) {
				if (this->operands_.size() < 2) {
					throw std::runtime_error("CalcWorker::interim_calc(): not valid input expression.");
				}
					this->top_calculation();
			}
		}
		if (e_operators_type::RIGHT_BKT == new_oper_type &&
			0 != this->operators_.size()) {
			while (e_operators_type::LEFT_BKT != this->get_operators_type(this->operators_.top())) {
				this->top_calculation();				
			}
			this->operators_.pop();
			return;
		}
		this->operators_.push(new_oper);
	}

	void CalcWorker::top_calculation() {
		auto second_val = this->operands_.back();
		this->operands_.pop_back();
		auto first_val = this->operands_.back();
		this->operands_.back() = this->coumputation(first_val, second_val, this->operators_.top());
		this->operators_.pop();
	}

	const double CalcWorker::coumputation(double first, double second, char operation) const {
		switch(operation) {
			case '+': return first + second;
			case '-': return first - second;
			case '*': return first * second;
			case '/': return first / second;
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


	bool CalcWorker::isoperator(const char symbol) const {
		std::vector<char> operators = {'+', '-', '*', '/', '^', '(', ')'};
		return std::find(operators.begin(), operators.end(), symbol) != operators.end();
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
			'-' != last) {
			return false;
		}
		return true;
	}

	void CalcWorker::push_back_operand(std::string::iterator &iter) {
		auto start_substr = iter;
		auto is_double = false;
		double current_value = *iter - '0';
		while (this->expression_.end() != iter + 1) {
			if (true == isdigit(*(iter + 1))) {
				++iter;
				// current_value = current_value * 10 + (*(++iter) - '0');
			}
			else if (false == is_double &&
					'.' == *(iter + 1)) {
				++iter;
				// double factional = 0;
				// auto factional_degree = 0;
				// while (	this->expression_.end() != iter + 1 &&
				// 		true == isdigit(*(iter + 1))) {
				// 	factional = factional * 10 + (*(++iter) - '0');
				// 	++factional_degree;
				// }
				// current_value += (factional / pow(10.0, factional_degree));
			}
			else if ('.' == *(iter + 1)) {
				throw std::runtime_error("CalcWorker::push_back_operand(): not valid input expression.");
			}
			else {
				break;
			}
		}
		auto value(std::string(start_substr, iter));
		this->operands_.push_back(value);
	}

	void CalcWorker::push_back_operator(std::string::iterator &iter) {
		if ('-' == *iter) {
			if (0 == this->operands_.size() ||
				'(' == *(iter - 1)) {
				this->operands_.push_back(0);
				this->operators_.push(*iter);
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
		this->interim_calc(*iter);
	}

}