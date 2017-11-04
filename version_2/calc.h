#ifndef CALC_CALC_H_
#define CALC_CALC_H_
#include <math.h>
#include "polish_convertor.h"

namespace NCalcWorker
{
	class CalcWorker
	{
	public:
		CalcWorker() {}
		const std::string calculate(const std::string &expression);
	private:
		const NToken::Token execute_operation(const double l_val, const double r_val, const std::string &opreation) const;
	};
}
#endif // CALC_CALC_H_