#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_

#include <time.h>
#include <string>

/// ================================================
///
/// @brief       ʱ��ת��Ϊ�ַ�
///
/// @details     �������ʱ���������ָ���ĸ�ʽת��
///
/// @param[in]   lt        ʱ��ṹ��
/// @param[in]   nFormat     ת����ʽ
///            nFormat = 0 : yyyymmdd/hh/mi/ss
///            nFormat = 1 : yyyy-mm-dd hh:mi:ss
///            nFormat = 2 : yyyy-mm-dd 00:00:00
///            nFormat = 3 : hhmiss
///            nFormat = 4 : yyyymmddhh0000
///            nFormat = 5 : mmdd
///            nFormat = 6 : yyyymmdd000000
///            nFormat = 7 : yyyymmddhhmiss
///
/// @return ת�����ʱ���ַ���
/// ================================================
std::string time2Str(struct tm *lt, int nFormat);

#endif // _COMMON_FUNCTION_H_
