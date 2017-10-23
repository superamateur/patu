#ifndef __PARAM_PARSE_H
#define __PARAM_PARSE_H
#include "func_defs.h"

// ParamParse is singleton class
class ParamParse
{
MAKE_SINGLETON(ParamParse);
public:
	static void set_args(int argc, char** argv);
	static int argc(void) { return m_argc; }
	static char** argv(void) { return &m_argv; }

private:
	static int m_argc;
	static char* m_argv;
};
#endif // __PARAM_PARSE_H