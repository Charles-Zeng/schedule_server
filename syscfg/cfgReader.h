#ifndef _CFG_READER_H_
#define _CFG_READER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <ccl/ccl.h>
#ifdef __cplusplus
}
#endif
#include <string>

class CCfgReader
{
public:
	CCfgReader(const std::string& fileName);
	virtual ~CCfgReader();
	bool getStringValue(const std::string& node, std::string& value);
	bool getIntValue(const std::string& node, int& value);
private:
	ccl_t m_config;
};

#endif	//_CFG_READER_H_
