#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include <common/dataStruct.h>
#include <logger/logger.h>

class Processor
{
public:
	Processor() {}
	virtual ~Processor() {}
	virtual void process(const HttpRequest& req, HttpResponse& resp) = 0;
};
#endif // _PROCESSOR_H_
