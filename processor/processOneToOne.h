#ifndef _PROCESS_ONE_TO_ONE_H_
#define _PROCESS_ONE_TO_ONE_H_

#include "processor.h"

class ProcessOneToOne : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
};


#endif // _PROCESS_ONE_TO_ONE_H_
