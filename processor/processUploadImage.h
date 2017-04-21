#ifndef _PROCESS_UPLOAD_IMAGE_H_
#define _PROCESS_UPLOAD_IMAGE_H_

#include "processor.h"
#include <string>

class ProcessUploadImage : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);
protected:
	void getImageFilePath(std::string& strPath, std::string& strFileName);
	bool saveImageFile(const std::string strPath, const std::string &strFileName,
		const std::string& content_base64);
};


#endif // _PROCESS_UPLOAD_IMAGE_H_
