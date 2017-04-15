#ifndef _DATA_LAYER_H_
#define _DATA_LAYER_H_

#include <string>
#include <common/dataStruct.h>

class DataLayer
{
public:
	static bool saveImage(const ImageInfo& imageInfo);
	static bool getOneToNGroupIds(std::string &groupIds);
	static bool getAlarmParam(AlarmParam &alarmParam);
	static bool saveSuspectAlarm(const SuspectAlarm& suspectAlarm);
	static bool getLocationId(const std::string& camerCode, std::string& locationId);
};

#endif // _DATA_LAYER_H_
