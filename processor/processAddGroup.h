#ifndef _PROCESS_ADD_GROUP_H_
#define _PROCESS_ADD_GROUP_H_

#include "processor.h"

class ProcessAddGroup : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
};


#endif // _PROCESS_ADD_GROUP_H_
