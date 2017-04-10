#ifndef _PROCESS_ADD_TEMPLATE_H_
#define _PROCESS_ADD_TEMPLATE_H_

#include "processor.h"

class ProcessAddTemplate : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
};


#endif // _PROCESS_ADD_TEMPLATE_H_
