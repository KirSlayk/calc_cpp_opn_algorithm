#include <algorithm>
#include "polish_convertor.h"
namespace NPolish
{	
	const std::deque<NToken::Token> PolishConvertor::conv_expr_to_pol_notation(const std::string &expression) {
		std::stack<NToken::Token>().swap(this->operator_stack_);
		
		auto separate_lexemes = this->get_separate_lexemes(expression);
		auto separate_tokens = this->get_valid_tokens_sequence(separate_lexemes);
		
		for (auto &token: separate_tokens) {
			this->handle_token(token);
		}
		
		if (0 == this->polish_expr_.size() &&
			this->operator_stack_.size() > 0) {
			throw std::runtime_error("Bad expression syntax. Not enough values.");
		}
		
		this->shift_all_operators();
		return this->polish_expr_;
	}


	const std::vector<std::string> PolishConvertor::get_separate_lexemes(const std::string &expression) const {
		std::vector<std::string> separate_lexemes(0);
		auto expr_ex_space = this->remove_spaces(expression);
		this->detect_unary_operators(expr_ex_space);
		for (size_t i = 0; i < expr_ex_space.size(); ++i) {
			switch(expr_ex_space[i]) {
				case '+' : case '-' :
				case '*' : case '/' :
				case '(' : case ')' :
				case '{' : case '}' :
				case '[' : case ']' :
				case '^' :
							{
								separate_lexemes.push_back(std::string(1, expr_ex_space[i]));
								break;
							}
				case '0' : case '1' : case '2' :
				case '3' : case '4' : case '5' :
				case '6' : case '7' : case '8' : 
				case '9' : case '.' :
							{
								size_t j = i;
								while (	j < expr_ex_space.size() &&
										this->is_digit(expr_ex_space[j])) {
									++j;
								}
								std::string num = std::string(expr_ex_space, i, j - i);
								separate_lexemes.push_back(num);
								i = j - 1;
								break;
							}
				default	:
							{
								throw std::runtime_error("Invalid underfind lexeme: \'" + std::string(1, expr_ex_space[i]) + "\'\n");
							}
			}
		}
		return separate_lexemes;
	}

	const void PolishConvertor::detect_unary_operators(std::string &expression) const {
		if (0 == expression.size()) {
			return;
		}
		if ('-' == expression[0]) {
			expression = "0" + expression;
		}
		std::string::size_type pos;
		while (std::string::npos != (pos = expression.find("(-"))) {
			expression.replace(pos, 2, "(0-");
		}
	}


	const std::vector<NToken::Token> PolishConvertor::get_valid_tokens_sequence(const std::vector<std::string> &separate_lexemes) const {
		std::vector<NToken::Token> sequence(0);
		
		if (0 == separate_lexemes.size()) {
			return sequence;
		}

		auto front_token = NToken::Token(separate_lexemes.front());
		if (NToken::e_type_::OPERATOR == front_token.get_type() ||
			NToken::e_type_::CLS_BK == front_token.get_type()) {
			throw std::runtime_error("Not valid first lexeme in expression: \'" ) ;
		}
		if (NToken::e_type_::DOUBLE == front_token.get_type() &&
			false == this->is_valid_dot_count_indouble(front_token.get_value())) {
			throw std::runtime_error("Not valid first lexeme in expression: \'" ) ;
		}
		sequence.push_back(front_token);

		for (size_t i = 1; i < separate_lexemes.size(); ++i) {
			auto current_token = NToken::Token(separate_lexemes[i]);
			auto &prev_token = sequence.back();
			if (NToken::e_type_::OPERATOR == prev_token.get_type() &&
				NToken::e_type_::OPERATOR == current_token.get_type()) {
				throw std::runtime_error("Bad expression syntax. Two operators are coming in a raw.");
			}
			if (NToken::e_type_::DOUBLE == prev_token.get_type() &&
				NToken::e_type_::DOUBLE == current_token.get_type()) {
				throw std::runtime_error("Bad expression syntax. Two double are coming in a raw.");
			}
			if (NToken::e_type_::CLS_BK == prev_token.get_type() &&
				NToken::e_type_::OPN_BK == current_token.get_type()) {
				throw std::runtime_error("Bad expression syntax. \')(\' in expression.");
			}
			if (NToken::e_type_::OPN_BK == prev_token.get_type() &&
				NToken::e_type_::OPERATOR == current_token.get_type()) {
				throw std::runtime_error("Bad expression syntax. \'(" + current_token.get_value() + "\' in expression.");
			}
			if (NToken::e_type_::OPERATOR == prev_token.get_type() &&
				NToken::e_type_::CLS_BK == current_token.get_type()) {
				throw std::runtime_error("Bad expression syntax. \'" + prev_token.get_value() + ")\' in expression.");
			}
			if (NToken::e_type_::DOUBLE == current_token.get_type()) {
				if (false == this->is_valid_dot_count_indouble(current_token.get_value())) {
					throw std::runtime_error("In double " + current_token.get_value() + " more than one dot.");
				}
			}
			sequence.push_back(current_token);
		}
		return sequence;
	}

	
	const PolishConvertor::e_priority PolishConvertor::get_priority(const NToken::Token &token) const {
		auto value = this->remove_spaces(token.get_value());
		if (NToken::e_type_::OPERATOR != token.get_type() ||
			1 != value.size()) {
			return e_priority::NONE;
		}
		switch(token.get_value()[0]) {
			case '^' :
						{
							return e_priority::VERY_HIGH;
						}
			case '+' :
			case '-' :
						{
							return e_priority::MEDIUM;
						}
			case '*' :
			case '/' :	
						{
							return e_priority::HIGH;
						}
			case ')' :
			case '(' :	
						{
							return e_priority::LOW;
						}
		}
		return e_priority::NONE;
	}


	const void PolishConvertor::handle_token(const NToken::Token &token) {
		switch(static_cast<int>(token.get_type())) {
			case NToken::e_type_::DOUBLE 	: this->handle_double(token); break;
			case NToken::e_type_::OPERATOR 	: 
			case NToken::e_type_::OPN_BK 	:
			case NToken::e_type_::CLS_BK 	: this->handle_operator(token); break;
		}
	}

	const void PolishConvertor::handle_double(const NToken::Token &token) {
		this->polish_expr_.push_back(token);
	}
	
	const void PolishConvertor::handle_operator(const NToken::Token &token) {
		if (true == this->is_close_bk(token)) {
			this->shift_subexpression_operators();
		}
		else if (true == this->is_open_bk(token) ||
			0 == this->operator_stack_.size()) {
			this->operator_stack_.push(token);
		}
		else {
			while (this->operator_stack_.size() > 0) {
				auto top = this->operator_stack_.top();
				if (e_priority::VERY_HIGH == get_priority(top) &&
					e_priority::VERY_HIGH == get_priority(token)) {
					break;
				}
				if (get_priority(top) >= get_priority(token) &&
					NToken::e_type_::OPN_BK != top.get_type() &&
					NToken::e_type_::CLS_BK != top.get_type()) {
					this->polish_expr_.push_back(top);
					this->operator_stack_.pop();
				}
				else {
					break;
				}
			}
			this->operator_stack_.push(token);
		}
	}


	const bool PolishConvertor::is_open_bk(const NToken::Token &token) const {
		auto value = this->remove_spaces(token.get_value());
		if (1 != value.size() ||
			'(' != value[0]) {
			return false;
		}
		return true;
	}

	const bool PolishConvertor::is_close_bk(const NToken::Token &token) const {
		auto value = this->remove_spaces(token.get_value());
		if (1 != value.size() ||
			')' != value[0]) {
			return false;
		}
		return true;
	}

	const bool PolishConvertor::is_digit(const char ch) const {
		if (0 != std::isdigit(ch) || '.' == ch) {
			return true;
		}
		return false;
	}

	const bool PolishConvertor::is_valid_dot_count_indouble(const std::string &lexem) const {
		if (std::count(lexem.begin(), lexem.end(), '.') > 1) {
			return false;
		}
		return true;
	}


	const std::string PolishConvertor::remove_spaces(const std::string &value) const {
		std::string str_withot_spaces = value;
		str_withot_spaces.erase(std::remove_if(str_withot_spaces.begin(), str_withot_spaces.end(), isspace), str_withot_spaces.end());
		return str_withot_spaces;
	}


	const void PolishConvertor::shift_subexpression_operators() {
		while(this->operator_stack_.size() > 0 &&
			false == this->is_open_bk(this->operator_stack_.top())) {
			this->polish_expr_.push_back(this->operator_stack_.top());
			this->operator_stack_.pop();
		}
		if (0 == this->operator_stack_.size() ||
			false == this->is_open_bk(this->operator_stack_.top())) {
			throw std::runtime_error("Bad expression syntax. The opening parenthesis is missing");
		}
		this->operator_stack_.pop();
	}

	const void PolishConvertor::shift_all_operators() {
		while (this->operator_stack_.size() > 0) {
			this->polish_expr_.push_back(this->operator_stack_.top());
			this->operator_stack_.pop();
		}
	}
}