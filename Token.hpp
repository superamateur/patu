#ifndef __TOKEN_HPP
#define __TOKEN_HPP
#include "func_defs.h"

class Token
{
public:
	Token(void);
	Token(const std::string& str);
	virtual ~Token(void);
	void split_by(const char* delim, std::vector<Token>& ret) const;

	void  set(const char* str_val) { m_str_value = str_val; internal_convert(); }
	int   get_int(void) const      { return m_int_value; }
	float get_float(void) const    { return m_float_value; }
	const std::string& get_string(void) const { return m_str_value; }

private:
	std::string m_str_value;
	int m_int_value;
	float m_float_value;
	void internal_convert(void);
};

#endif