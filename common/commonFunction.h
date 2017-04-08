#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_

#include <time.h>
#include <string>

/// ================================================
///
/// @brief       时间转换为字符
///
/// @details     将传入的时间参数按照指定的格式转化
///
/// @param[in]   lt        时间结构体
/// @param[in]   nFormat     转换格式
///            nFormat = 0 : yyyymmdd/hh/mi/ss
///            nFormat = 1 : yyyy-mm-dd hh:mi:ss
///            nFormat = 2 : yyyy-mm-dd 00:00:00
///            nFormat = 3 : hhmiss
///            nFormat = 4 : yyyymmddhh0000
///            nFormat = 5 : mmdd
///            nFormat = 6 : yyyymmdd000000
///            nFormat = 7 : yyyymmddhhmiss
///
/// @return 转换后的时间字符串
/// ================================================
std::string time2Str(struct tm *lt, int nFormat);

#endif // _COMMON_FUNCTION_H_
