#include "dataLayer.h"
#include <database/DBAdaptor.h>
#include <common/commonFunction.h>
#include <logger/logger.h>
#include "../tool/uuid.h"

#include <time.h>
#include <stdio.h>

using namespace oracle::occi;

bool DataLayer::saveImage( const ImageInfo& imageInfo )
{
	bool ret = false;

	std::string sqlStr = "INSERT INTO TB_MONITOR_PHOTO (ID, LOCATION_ID, MONITOR_TIME, PHOTO_PATH, CASE_PHOTO_ID) VALUES(:f1, :f2, to_date( :f3,'YYYY-MM-DD HH24:MI:SS'), :f4, :f5 )";

	try
	{
		ConnectionObj connObj;

		Statement *stmt = connObj.conn->createStatement();
		stmt->setSQL(sqlStr);
		stmt->setString(1, generate_uuid_string());
		stmt->setString(2, imageInfo.locationId);

		time_t monitorTime = imageInfo.monitorTime;
		tm* timeInfo = localtime(&monitorTime);
		std::string monitorTimeStr = time2Str(timeInfo, 1);	

		stmt->setString(3, monitorTimeStr);
		stmt->setString(4, imageInfo.photoPath);
		stmt->setString(5, imageInfo.templateId);

		stmt->executeUpdate();
		connObj.conn->commit();
		connObj.conn->terminateStatement(stmt);

		ret = true;
	}
	catch (exception &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "%s:%d, 执行SQL出错: %s", __FILE__, __LINE__, e.what());
	}
	
	return ret;
}

bool DataLayer::getOneToNGroupIds(std::string &groupIds)
{
	bool ret = false;
	std::string sqlStr = "SELECT LIBRARY_ID FROM TB_PARAM_LIBRARY";
	
	try
	{
		ConnectionObj connObj;

		Statement *stmt = connObj.conn->createStatement();
		stmt->setSQL(sqlStr);

		ResultSet *rs = stmt->executeQuery();
		
		while (rs->next())
		{
			groupIds = rs->getString(1);
		}

		stmt->closeResultSet(rs);
		connObj.conn->terminateStatement(stmt);

		ret = true;
	}
	catch (exception &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "%s:%d, 执行SQL出错: %s", __FILE__, __LINE__, e.what());
	}

	return ret;
}

bool DataLayer::getAlarmParam(AlarmParam &alarmParam)
{
	bool ret = false;
	std::string sqlStr = "SELECT ALARM_THRESHOLD, MAX_RET_NUMBERS FROM TB_ALARM_PARAM";

	try
	{
		ConnectionObj connObj;

		Statement *stmt = connObj.conn->createStatement();
		stmt->setSQL(sqlStr);

		ResultSet *rs = stmt->executeQuery();

		while (rs->next())
		{
			alarmParam.alarmThreshold = rs->getFloat(1);
			alarmParam.maxReturnNumber = rs->getInt(2);
		}

		stmt->closeResultSet(rs);
		connObj.conn->terminateStatement(stmt);

		ret = true;
	}
	catch (exception &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "%s:%d, 执行SQL出错: %s", __FILE__, __LINE__, e.what());
	}

	return ret;
}

bool DataLayer::saveSuspectAlarm( const SuspectAlarm& suspectAlarm )
{
	bool ret = false;
	std::string sqlStr = "INSERT INTO TB_SUSPECT_ALARM (FACE_ID, MONITOR_ID, ALARM_TIME, ALARM_ADDRESS, SIMILARITY, SUSPECT_STATE, SUSPECT_TYPE ) \
						 VALUES( :f1, :f2, to_date( :f3,'YYYY-MM-DD HH24:MI:SS'), :f4, :f5, :f6, f7 )";

	try
	{
		ConnectionObj connObj;

		Statement *stmt = connObj.conn->createStatement();
		stmt->setSQL(sqlStr);

		stmt->setString(1, suspectAlarm.faceId);
		stmt->setString(2, suspectAlarm.monitorId);

		time_t alarmTime = suspectAlarm.alarmTime;
		tm* timeInfo = localtime(&alarmTime);
		std::string alarmTimeStr = time2Str(timeInfo, 1);
		stmt->setString(3, alarmTimeStr);

		stmt->setString(4, suspectAlarm.alarmAddress);
		stmt->setFloat(5, suspectAlarm.similarity);
		stmt->setString(6, "1"); //1：未处理 2：已处理
		stmt->setString(7, "1"); //1：布控自动告警 2：人工确认告警 3：人工比对告警

		stmt->executeUpdate();

		connObj.conn->terminateStatement(stmt);

		ret = true;
	}
	catch (exception &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "%s:%d, 执行SQL出错: %s", __FILE__, __LINE__, e.what());
	}

	return ret;
}

bool DataLayer::getLocationId(const std::string& camerCode, std::string& locationId)
{
	bool ret = false;
	std::string sqlStr = "SELECT LOCATION_ID FROM TB_CAMERA_INFO where CAMERA_CODE = :f1";

	try
	{
		ConnectionObj connObj;

		Statement *stmt = connObj.conn->createStatement();
		stmt->setSQL(sqlStr);

		stmt->setString(1, camerCode);

		ResultSet *rs = stmt->executeQuery();

		while (rs->next())
		{
			locationId = rs->getString(1);
		}

		stmt->closeResultSet(rs);
		connObj.conn->terminateStatement(stmt);

		ret = true;
	}
	catch (exception &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "%s:%d, 执行SQL出错: %s", __FILE__, __LINE__, e.what());
	}

	return ret;
}

bool DataLayer::getLocationCode( const std::string& camerCode, std::string& locationCode )
{
	bool ret = false;
	std::string sqlStr = "SELECT TB_MONITOR_LOCATION.LOCATION_CODE FROM TB_CAMERA_INFO INNER JOIN TB_MONITOR_LOCATION \
						 ON TB_CAMERA_INFO.LOCATION_ID = TB_MONITOR_LOCATION.ID  where TB_CAMERA_INFO.CAMERA_CODE = :f1";

	try
	{
		ConnectionObj connObj;

		Statement *stmt = connObj.conn->createStatement();
		stmt->setSQL(sqlStr);

		stmt->setString(1, camerCode);

		ResultSet *rs = stmt->executeQuery();

		while (rs->next())
		{
			locationCode = rs->getString(1);
		}

		stmt->closeResultSet(rs);
		connObj.conn->terminateStatement(stmt);

		ret = true;
	}
	catch (exception &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "%s:%d, 执行SQL出错: %s", __FILE__, __LINE__, e.what());
	}

	return ret;
}

bool DataLayer::getAreaCode( std::string& areaCode )
{
	bool ret = false;
	std::string sqlStr = "SELECT AREA_CODE FROM TB_MONITOR_AREA";

	try
	{
		ConnectionObj connObj;

		Statement *stmt = connObj.conn->createStatement();
		stmt->setSQL(sqlStr);

		ResultSet *rs = stmt->executeQuery();

		while (rs->next())
		{
			areaCode = rs->getString(1);
		}

		stmt->closeResultSet(rs);
		connObj.conn->terminateStatement(stmt);

		ret = true;
	}
	catch (exception &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "%s:%d, 执行SQL出错: %s", __FILE__, __LINE__, e.what());
	}

	return ret;
}

bool DataLayer::getRegionCode( std::string& regionCode )
{
	bool ret = false;
	std::string sqlStr = "SELECT REGION_CODE FROM SYS_REGION_INFO";

	try
	{
		ConnectionObj connObj;

		Statement *stmt = connObj.conn->createStatement();
		stmt->setSQL(sqlStr);

		ResultSet *rs = stmt->executeQuery();

		while (rs->next())
		{
			regionCode = rs->getString(1);
		}

		stmt->closeResultSet(rs);
		connObj.conn->terminateStatement(stmt);

		ret = true;
	}
	catch (exception &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "%s:%d, 执行SQL出错: %s", __FILE__, __LINE__, e.what());
	}

	return ret;
}

