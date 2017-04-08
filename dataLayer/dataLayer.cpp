#include "dataLayer.h"
#include <database/DBAdaptor.h>
#include <common/commonFunction.h>

#include <time.h>
#include <stdio.h>

using namespace oracle::occi;

void DataLayer::saveImage( const ImageInfo& imageInfo )
{
	std::string sqlStr = "INSERT INTO TB_MONITOR_PHOTO ( CAMERA_ID, MONITOR_TIME, PHOTO, CASE_PHOTO_ID) VALUES( :f1, to_date( :f2,'YYYY-MM-DD HH24:MI:SS'), :f3, :f4 )";

	ConnectionObj connObj;

	Statement *stmt = connObj.conn->createStatement();
	stmt->setSQL(sqlStr);

	stmt->setString(1, imageInfo.camerId);

	time_t monitorTime = imageInfo.monitorTime;
	tm* timeInfo = localtime(&monitorTime);
	std::string monitorTimeStr = time2Str(timeInfo, 1);	

	stmt->setString(2, monitorTimeStr);
	stmt->setString(3, "");
	stmt->setString(4, imageInfo.templateId);

	stmt->executeUpdate();

	connObj.conn->terminateStatement(stmt);
}

