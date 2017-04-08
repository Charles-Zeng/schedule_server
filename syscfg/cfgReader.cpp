#include <stdlib.h>
#include "cfgReader.h"

CCfgReader::CCfgReader(const std::string& fileName)
{
	m_config.comment_char = '#';
	m_config.sep_char = '=';
	m_config.str_char = '"';
	ccl_parse(&m_config, fileName.c_str());

	const struct ccl_pair_t       *iter;
	while ((iter = ccl_iterate(&m_config)) != 0) {
		printf("(%s,%s)\n", iter->key, iter->value);
	}
}

CCfgReader::~CCfgReader()
{
	ccl_release(&m_config);
}

bool CCfgReader::getStringValue(const std::string& node, std::string& value)
{
	const char* pStrValue = ccl_get(&m_config, node.c_str());
	if(NULL == pStrValue)
	{
		return false;
	}
	value = pStrValue;
	return true;
}

bool CCfgReader::getIntValue(const std::string& node, int& value)
{
	std::string strResult;
	if(!getStringValue(node, strResult))
	{
		return false;
	}
	value = atoi(strResult.c_str());
	return true;
}