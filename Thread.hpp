#ifndef __THREAD_H
#define __THREAD_H
#include <windows.h>
#include <process.h>

class Thread
{
public:
	explicit Thread(void* thread_func, void* args);
	virtual ~Thread(void);
	virtual void join(void);
	virtual void join(long long time_out);
private:
	HANDLE m_handle;
	unsigned int m_id;
};
#endif // __THREAD_H