#include "Reader.hpp"

Reader::Reader(void)
{
}

Reader::Reader(const char* file_name)
: m_file_name(file_name)
{
}

Reader::~Reader(void)
{
}

void Reader::set_file_name(const char *file_name)
{
	m_file_name = std::string(file_name);
}