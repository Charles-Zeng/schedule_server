#include "jsonBuilder.h"

#include <json/json.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <vector>

std::string CJsonBuilder::buildAddTemplate( const TemplateInfo& templateInfo )
{
	Json::Value value;

	std::string genderStr = boost::lexical_cast<std::string>(templateInfo.gender);
	value["createTime"] = templateInfo.createTime.c_str();
	value["gender"] = genderStr.c_str();
	value["groupIds"] = templateInfo.groupId.c_str();  //由于建模那边json字段grounId改为grounIds
	value["imgString"] = templateInfo.imageStr.c_str();

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildDelTemplate(std::string id )
{
	Json::Value value;

	value["id"] = boost::lexical_cast<int>(id);

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildAddGroupId( const GroupIdInfo& groupIdInfo )
{
	Json::Value value;

	value["id"] = groupIdInfo.id;
	value["name"] = groupIdInfo.name.c_str();;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildDelGroupId( const std::string& id )
{
	Json::Value value;

	value["id"] = id.c_str();;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildGetFaceInfo( const std::string& pic )
{
	Json::Value value;

	value["pic"] = pic.c_str();;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildOneToOne( const OneToOneInfo& oneToOneInfo )
{
	Json::Value value;

	value["pic1"] = oneToOneInfo.pic1Base64.c_str();;
	value["pic2"] = oneToOneInfo.pic2Base64.c_str();;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildOneToN( const OneToNInfo& oneToNInfo )
{
	std::list<std::string> groupIdsStrList;
	typedef std::list<std::string>::const_iterator IT;
	for (IT it = oneToNInfo.groupIds.begin(); it != oneToNInfo.groupIds.end(); ++it)
	{
		groupIdsStrList.push_back(*it);
	}

	Json::Value value;

	//std::string genderStr = boost::lexical_cast<std::string>(oneToNInfo.gender);
	std::string groupIdsStr = boost::algorithm::join(groupIdsStrList, ",");
	value["threshold"] = oneToNInfo.threshold;
	value["count"] = oneToNInfo.count;
	value["gender"] = oneToNInfo.gender.c_str();//genderStr.c_str();
	value["groupIds"] = groupIdsStr.c_str();
	value["pic"] = oneToNInfo.picBase64.c_str();

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildDynamicOneToN( const DynamicOneToNReq& dynamicOneToNReq )
{
	Json::Value value;

	value["sourceID"] = dynamicOneToNReq.sourceId.c_str();
	value["threshold"] = boost::lexical_cast<float>(dynamicOneToNReq.threshold);
	value["count"] = boost::lexical_cast<int>(dynamicOneToNReq.count);
	value["groupIds"] = dynamicOneToNReq.groupIds.c_str();
	value["pic"] = dynamicOneToNReq.pic.c_str();

	return value.toStyledString().c_str();
}

