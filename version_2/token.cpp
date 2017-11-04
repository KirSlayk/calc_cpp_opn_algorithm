#include "token.h"
#include <iostream>
namespace NToken
{
	void Token::set_type(const std::string &value) {
		this->type_ = e_type_::NONE;
		auto prev_type = this->type_;
		for (auto &ch: value) {
			switch(ch) {
				case '+' : case '-' : 
				case '*' : case '/' :
				case '^' :
							{
								this->type_ = e_type_::OPERATOR;
								break;
							}
				case '(' : case '{' : case '[' :
							{
								this->type_ = e_type_::OPN_BK;
								break;
							}
				case ')' : case '}' : case ']' :
							{
								this->type_ = e_type_::CLS_BK;
								break;
							}
				case '0' : case '1' : case '2' :
				case '3' : case '4' : case '5' :
				case '6' : case '7' : case '8' :
				case '9' : case '.' :
							{
								this->type_ = e_type_::DOUBLE;
								break;
							}
				case ' ' : case '\t' :
							{
								this->type_ = e_type_::ISSPACE;
								break;
							}
				default	:
							{
								this->type_ = e_type_::UNDERFIND;
								break;
							}

			}
			if (e_type_::NONE != prev_type &&
				prev_type != this->type_) {
				throw std::runtime_error("NToken::Token::set_type() error: lexeme parse error.");
			}
			prev_type = this->type_;	
		}
	}
}