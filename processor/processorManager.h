#ifndef _PROCESSOR_MANAGER_H_
#define _PROCESSOR_MANAGER_H_

#include <cstddef>

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

#include <common/dataStruct.h>

class ProcessorManager : public boost::noncopyable
{
public:
	static ProcessorManager& instance()
	{
		static ProcessorManager m_instance;
		return m_instance;
	}

	bool init(std::size_t threadNum);	

protected:
	ProcessorManager();
	~ProcessorManager();

private:
	void run();
	void processHttpReq(const HttpRequest& req, HttpResponse& resp);
	bool isPriorityReq(E_HTTP_TYPE reqType);

private:
	bool m_isRun;
	std::size_t m_threadNum;	
	boost::thread_group m_threadPool;
};


#endif // _PROCESSOR_MANAGER_H_
