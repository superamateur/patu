#ifndef __FUNC_DEFS_H
#define __FUNC_DEFS_H

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <float.h>

#define SET_GET_MACRO(Type, FaceName, MemberName) \
	void set_##FaceName(const Type value) {		  \
		m_##MemberName = value;					  \
	}											  \
	const Type get_##FaceName(void) {			  \
		return m_##MemberName;					  \
	}

#define SET_GET_MACRO_REF(Type, FaceName, MemberName) \
	void set_##FaceName(const Type& value) {		  \
		m_##MemberName = value;					  \
	}											  \
	const Type& get_##FaceName(void) {			  \
		return m_##MemberName;					  \
	}

#define MAKE_SINGLETON(ClassName)										\
private:																\
	ClassName(void);                        /*no outside constructor*/	\
	ClassName(const ClassName&);            /*no copy constructor*/		\
	ClassName& operator=(const ClassName&); /*no assigment*/

#define SHARED_PTR(TypeName) std::tr1::shared_ptr<TypeName>

#define ERR_MSG(msg)																		\
{																							\
	std::cerr << __FILE__ << "::" << __FUNCTION__<< "::" << __LINE__ << "[ERROR]: " << msg; \
	throw(std::exception("msg"));															\
}

#define INFO_MSG(msg)																					\
{																										\
	std::cout << __FILE__ << "::" << __FUNCTION__<< "::" << __LINE__ << " [INFO]: " << msg << std::endl;	\
}

#define WARN_MSG(msg)																					\
{																										\
	std::cout << __FILE__ << "::" << __FUNCTION__<< "::" << __LINE__ << " [WARN]: " << msg << std::endl;	\
}

#define FLT_0 0.f
#define FLT_ESP 1.e-7f
#define EQUAL_FLT(x, y) ((((x) - (y)) < FLT_ESP) && (((x) - (y)) > -FLT_ESP))
typedef std::vector<int>::iterator IntIterType;
typedef std::vector<int>::const_iterator ConstIntIterType;

#endif // __FUNC_DEFS_H