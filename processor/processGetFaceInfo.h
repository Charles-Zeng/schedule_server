#ifndef _PROCESS_GET_FACE_INFO_H_
#define _PROCESS_GET_FACE_INFO_H_

#include "processor.h"

class ProcessGetFaceInfo : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
};


#endif // _PROCESS_ONE_TO_ONE_H_
