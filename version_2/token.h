#ifndef CALC_TOKEN_H_
#define CALC_TOKEN_H_
#include <string>
#include <stdexcept>

namespace NToken
{
	enum e_type_ {UNDERFIND, NONE, ISSPACE, DOUBLE, OPERATOR, OPN_BK, CLS_BK};

	struct Token
	{
	public:
		Token() : 	value_("")
					, type_(e_type_::NONE)
					{}

		Token(double value) :	value_(std::to_string(value))
								, type_(e_type_::DOUBLE)
								{}
		Token(char value)	:	value_(std::to_string(value))
		{
			this->set_type(std::to_string(value));
		}
		Token(const std::string &value)	:	value_(value)
		{
			this->set_type(value);			
		}

		inline const e_type_ get_type() const {
			return this->type_;
		}

		inline const std::string get_value() const {
			return this->value_;
		}
	private:
		void set_type(const std::string &value);
	private:
		std::string value_;
		e_type_ type_;
	};
}
#endif // CALC_TOKEN_H_