#ifndef _DATA_LAYER_H_
#define _DATA_LAYER_H_

#include <string>
#include <common/dataStruct.h>

class DataLayer
{
public:
	static bool saveImage(const ImageInfo& imageInfo);
	//static bool getOneToNGroupIds(std::string &groupIds);
	static bool getAlarmParam(AlarmParam &alarmParam);
	static bool saveSuspectAlarm(const SuspectAlarm& suspectAlarm);
	static bool getLocationId(const std::string& camerCode, std::string& locationId);
	//static bool getLocationCode(const std::string& camerCode, std::string& locationCode);
	//static bool getAreaCode(std::string& areaCode);
	//static bool getRegionCode(std::string& regionCode);
	static bool getFaceId(const std::string& templateId, std::string& faceId);
	static bool getAlarmAddress(const std::string& monitorFaceId, std::string& alarmAddress);
	static bool getImageFilePathInfo(const std::string& camerCode, std::string& regionCode, std::string& areaCode, std::string& locationCode);
};

#endif // _DATA_LAYER_H_
