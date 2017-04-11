#ifndef _PROCESS_ADD_GROUP_H_
#define _PROCESS_ADD_GROUP_H_

#include "processor.h"

#include <common/dataStruct.h>

class ProcessAddGroup : public Processor
{
public:
	void process(const HttpRequest& req, HttpResponse& resp);

private:
	int getNewGroupId(const std::list<GroupIdInfo> &groupIdInfos);
};


#endif // _PROCESS_ADD_GROUP_H_
