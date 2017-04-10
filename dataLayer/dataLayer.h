#ifndef _DATA_LAYER_H_
#define _DATA_LAYER_H_

#include <string>
#include <common/dataStruct.h>

class DataLayer
{
public:
	static void saveImage(const ImageInfo& imageInfo);
	static std::string getOneToNGroupIds();
	static AlarmParam getAlarmParam();
	static void saveSuspectAlarm(const SuspectAlarm& suspectAlarm);
};

#endif // _DATA_LAYER_H_
