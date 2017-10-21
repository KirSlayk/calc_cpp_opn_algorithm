#ifndef CALCCPP_CALC_H_
#define CALCCPP_CALC_H_
#include <string>
#include <sstream>
#include <stack>
#include <list>
#include <cctype>
#include <vector>
#include <cmath>
#include <iostream>
#include <exception>
namespace NCalculation
{
	typedef std::stack<char> operators_stack;
	typedef std::list<std::string> operands_list;

	class CalcWorker
	{
	public:
		explicit CalcWorker(const std::string &expression) : 	expression_(expression)
																, operands_(operands_list(0))
																, operators_(operators_stack())
																{}
		const double calc(const std::string &expression);
		const double calc();

	private:
		CalcWorker(const CalcWorker &) = delete;
		CalcWorker &operator=(const CalcWorker &) = delete;
		CalcWorker *operator&() = delete;
		const CalcWorker *operator&() const = delete;

		int get_operators_type(char operand) const;

		bool isoperator(const char symbol) const;
		bool is_valid_seq_operators(char first, char second) const;

		void push_back_operand(std::string::iterator &iter);
		void push_back_operator(std::string::iterator &iter);

		void interim_calc(char new_oper);
		void top_calculation();
		const double coumputation(double first, double second, char operation) const;

	private:
		std::string expression_;
		operands_list operands_;
		operators_stack operators_;

	private:
		enum e_operators_type {LEFT_BKT, RIGHT_BKT, LOW_T, MIDLE_T, HIGH_T};

	};
}
#endif // CALCCPP_CALC_H_
