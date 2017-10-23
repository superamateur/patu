#include "ParamParse.hpp"

int ParamParse::m_argc = 1;
char* ParamParse::m_argv = 0;

void ParamParse::set_args(int argc, char** argv)
{
	m_argc = argc;
	m_argv = *argv;
}
