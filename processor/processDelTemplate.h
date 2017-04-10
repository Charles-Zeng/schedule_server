#ifndef _PROCESS_DEL_TEMPLATE_H_
#define _PROCESS_DEL_TEMPLATE_H_

#include "processor.h"

class ProcessDelTemplate : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
};


#endif // _PROCESS_DEL_TEMPLATE_H_
