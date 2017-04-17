#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

static inline bool createDir(const char *path)
{
	int i, len;

	len = strlen(path);
	char dir_path[len+1];
	dir_path[len] = '\0';

	strncpy(dir_path, path, len);

	for (i=0; i<len; i++)
	{
		if (dir_path[i] == '/' && i > 0)
		{
			dir_path[i]='\0';
			if (access(dir_path, F_OK) < 0)
			{
				if (mkdir(dir_path, 0755) < 0)
				{
					return false;
				}
			}
			dir_path[i]='/';
		}
	}
	
	if (i == len)
	{
		if (access(dir_path, F_OK) < 0)
		{
			if (mkdir(dir_path, 0755) < 0)
			{
				return false;
			}
		}
	}

	return true;
}