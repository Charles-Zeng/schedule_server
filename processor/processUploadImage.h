#ifndef _PROCESS_UPLOAD_IMAGE_H_
#define _PROCESS_UPLOAD_IMAGE_H_

#include "processor.h"

class ProcessUploadImage : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
};


#endif // _PROCESS_UPLOAD_IMAGE_H_
