#ifndef _PROCESS_DEL_GROUP_H_
#define _PROCESS_DEL_GROUP_H_

#include "processor.h"

class ProcessDelGroup : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
};


#endif // _PROCESS_DEL_GROUP_H_
