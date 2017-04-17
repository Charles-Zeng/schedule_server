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

	value["createTime"] = templateInfo.createTime;
	value["gender"] = boost::lexical_cast<std::string>(templateInfo.gender);
	value["groupId"] = templateInfo.groupId;
	value["imgString"] = templateInfo.imageStr;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildDelTemplate( int id )
{
	Json::Value value;

	value["id"] = id;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildAddGroupId( const GroupIdInfo& groupIdInfo )
{
	Json::Value value;

	value["id"] = groupIdInfo.id;
	value["name"] = groupIdInfo.name;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildDelGroupId( const std::string& id )
{
	Json::Value value;

	value["id"] = id;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildGetFaceInfo( const std::string& pic )
{
	Json::Value value;

	value["pic"] = pic;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildOneToOne( const OneToOneInfo& oneToOneInfo )
{
	Json::Value value;

	value["pic1"] = oneToOneInfo.pic1Base64;
	value["pic2"] = oneToOneInfo.pic2Base64;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildOneToN( const OneToNInfo& oneToNInfo )
{
	std::list<std::string> groupIdsStrList;
	typedef std::list<int32_t>::const_iterator IT;
	for (IT it = oneToNInfo.groupIds.begin(); it != oneToNInfo.groupIds.end(); ++it)
	{
		groupIdsStrList.push_back(boost::lexical_cast<std::string>(*it));
	}

	Json::Value value;

	value["threshold"] = oneToNInfo.threshold;
	value["count"] = oneToNInfo.count;
	value["gender"] = boost::lexical_cast<std::string>(oneToNInfo.gender);
	value["groupIds"] = boost::algorithm::join(groupIdsStrList, ",");
	value["pic"] = oneToNInfo.picBase64;

	return value.toStyledString().c_str();
}

std::string CJsonBuilder::buildDynamicOneToN( const DynamicOneToNReq& dynamicOneToNReq )
{
	Json::Value value;

	value["sourceID"] = dynamicOneToNReq.sourceId;
	value["threshold"] = dynamicOneToNReq.threshold;
	value["count"] = dynamicOneToNReq.count;
	value["groupIds"] = dynamicOneToNReq.groupIds;
	value["pic"] = dynamicOneToNReq.pic;

	return value.toStyledString().c_str();
}

