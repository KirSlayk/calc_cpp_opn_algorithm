#ifndef CALCCPP_CALC_H_
#define CALCCPP_CALC_H_
#include <string>
#include <sstream>
#include <stack>
#include <list>
#include <cctype>
#include <vector>
#include <iterator>
#include <cmath>
#include <iostream>
#include <exception>
namespace NCalculation
{
	typedef std::stack<char> operators_stack;
	typedef std::list<std::string> expression_opn_list;

	class CalcWorker
	{
	public:
		explicit CalcWorker(const std::string &expression) : 	expression_(expression)
																, expression_opn_(expression_opn_list(0))
																, operators_(operators_stack())
																{}
		const double calc(const std::string &expression);
		const double calc();
		const double calc_opn(expression_opn_list &) const;

	private:
		CalcWorker(const CalcWorker &) = delete;
		CalcWorker &operator=(const CalcWorker &) = delete;
		CalcWorker *operator&() = delete;
		const CalcWorker *operator&() const = delete;

		int get_operators_type(char operand) const;

		bool is_float(const std::string& str) const;
		bool isoperator(const char symbol) const;
		bool isoperator(std::string &str) const;
		bool is_valid_seq_operators(char first, char second) const;

		void push_back_operand(std::string::iterator &iter);
		void push_back_operator(std::string::iterator &iter);

		void stack_permutation(char new_oper);
		void operator_from_stack_to_opn();

		const double coumputation(double first, double second, char operation) const;

	private:
		std::string expression_;
		expression_opn_list expression_opn_;
		operators_stack operators_;

	private:
		enum e_operators_type {LEFT_BKT, RIGHT_BKT, LOW_T, MIDLE_T, HIGH_T};

	};
}
#endif // CALCCPP_CALC_H_
