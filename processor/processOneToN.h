#ifndef _PROCESS_ONE_TO_N_H_
#define _PROCESS_ONE_TO_N_H_

#include "processor.h"

class ProcessOneToN : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
};


#endif // _PROCESS_ONE_TO_N_H_
