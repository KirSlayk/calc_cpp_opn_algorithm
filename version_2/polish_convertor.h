#ifndef CALC_POLISH_CONVERTOR_H_
#define CALC_POLISH_CONVERTOR_H_
#include <vector>
#include <deque>
#include <stack>
#include <string>
#include <iostream>
#include <sstream>
#include "token.h"

namespace NPolish
{
	
	class PolishConvertor
	{
	public:
		PolishConvertor() :	polish_expr_(0)
							, operator_stack_()
							{}
		const std::deque<NToken::Token> conv_expr_to_pol_notation(const std::string &expression);
	private:
		enum e_priority {NONE, LOW, MEDIUM, HIGH, VERY_HIGH};
		
		const std::vector<std::string> get_separate_lexemes(const std::string &expression) const;
		const void detect_unary_operators(std::string &expression) const;

		const std::vector<NToken::Token> get_valid_tokens_sequence(const std::vector<std::string> &separate_lexemes) const;

		const e_priority get_priority(const NToken::Token &token) const;
		const void handle_token(const NToken::Token &token);
		const void handle_double(const NToken::Token &token);
		const void handle_operator(const NToken::Token &token);
		
		const bool is_open_bk(const NToken::Token &token) const;
		const bool is_close_bk(const NToken::Token &token) const;
		const bool is_digit(const char ch) const;
		const bool is_valid_dot_count_indouble(const std::string &lexem) const;
		
		const std::string remove_spaces(const std::string &value) const;
		
		const void shift_subexpression_operators();
		const void shift_all_operators();
	private:
		std::deque<NToken::Token> polish_expr_;
		std::stack<NToken::Token> operator_stack_;
	};
}

#endif // CALC_POLISH_CONVERTOR_H_