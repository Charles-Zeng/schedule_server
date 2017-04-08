#include "commonFunction.h"

#include <boost/format.hpp>

std::string time2Str( struct tm *lt, int nFormat )
{
	std::string timeStr;
	switch (nFormat)
	{
	case 0: //ת����ʽ��yyyymmdd/hh/mi/ss
		timeStr = boost::str(boost::format("%d%02d%02d/%02d/%02d/%02d") % (lt->tm_year + 1900) % (lt->tm_mon + 1) % lt->tm_mday % lt->tm_hour % lt->tm_min % lt->tm_sec);		
		break;
	case 1: //ת����ʽ��yyyy-mm-dd hh:mi:ss
		timeStr = boost::str(boost::format("%d-%02d-%02d %02d:%02d:%02d") % (lt->tm_year + 1900) % (lt->tm_mon + 1) % lt->tm_mday % lt->tm_hour % lt->tm_min % lt->tm_sec);
		break;
	case 2: //ת����ʽ��yyyy-mm-dd 00:00:00
		timeStr = boost::str(boost::format("%d-%02d-%02d 00:00:00") % (lt->tm_year + 1900) % (lt->tm_mon + 1) % lt->tm_mday);
		break;
	case 3: //ת����ʽ��hhmiss
		timeStr = boost::str(boost::format("%02d%02d%02d") % lt->tm_hour % lt->tm_min % lt->tm_sec);
		break;
	case 4: //ת����ʽ��yyyymmddhh0000
		timeStr = boost::str(boost::format("%02d%02d%02d%02d0000") % (lt->tm_year + 1900) % (lt->tm_mon + 1) % lt->tm_mday % lt->tm_hour );
		break;
	case 5: //ת����ʽ��mmdd
		timeStr = boost::str(boost::format("%02d%02d") % (lt->tm_mon + 1) % lt->tm_mday);
		break;
	case 6: //ת����ʽ��yyyymmdd000000
		timeStr = boost::str(boost::format("%02d%02d%02d000000") % (lt->tm_year + 1900) % (lt->tm_mon + 1) % lt->tm_mday );
		break;
	case 7: //ת����ʽ��yyyymmddhhmiss
		timeStr = boost::str(boost::format("%d%02d%02d%02d%02d%02d") % (lt->tm_year + 1900) % (lt->tm_mon + 1) % lt->tm_mday % lt->tm_hour % lt->tm_min % lt->tm_sec);
		break;
	default:
		break;
	}

	return timeStr;
}

