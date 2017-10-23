#include "Thread.hpp"

Thread::Thread(void* thread_func, void* args)
: m_handle(NULL)
{
	// create new thread
	m_handle = (HANDLE)_beginthreadex(NULL, 0, (unsigned int (__stdcall *)(void *))(thread_func), args, 0, &m_id);
}

Thread::~Thread(void)
{
	// close thread handle
	if(m_handle) {
		::CloseHandle(m_handle);
		m_handle = NULL;
	}
}

void Thread::join(void)
{
	if(m_handle) {
		::WaitForSingleObject(m_handle, INFINITE);
	}
}

void Thread::join(long long time_out)
{
	if(m_handle) {
		::WaitForSingleObject(m_handle, (DWORD)time_out);
	}
}
