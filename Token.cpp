#include "Token.hpp"

Token::Token(void)
: m_str_value(0)
, m_int_value(0)
, m_float_value(0.f)
{
}

Token::Token(const std::string& str)
: m_str_value(str)
, m_int_value(0)
, m_float_value(0.f)
{
	internal_convert();
}

Token::~Token(void)
{
}

void Token::split_by(const char* delim, std::vector<Token>& ret) const
{
	size_t found = 0;
	size_t next_found = 0;
	while(true) {
		next_found = m_str_value.find(delim, found);
		if(next_found != std::string::npos) {
			ret.push_back(Token(m_str_value.substr(found, next_found - found + 1)));
		} else {
			ret.push_back(Token(m_str_value.substr(found, next_found)));
			break;
		}
		found = next_found  + 1;
	}
}

void Token::internal_convert(void)
{
	std::stringstream int_ss(m_str_value);
	std::stringstream flt_ss(m_str_value);
	int_ss >> m_int_value;
	flt_ss >> m_float_value;	
}