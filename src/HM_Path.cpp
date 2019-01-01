#include "HM_Path.h"

const char* HM_Path::rootPath = ROOT_FOLDER;

void HM_Path::setPath(const char* relativePath)
{
	if(m_relativePath)
		freePath();

	size_t sizeRelative = sizeof(char) * (strlen(relativePath) + 1);
	if (sizeRelative > 0)
	{
		m_relativePath = static_cast<char*>(malloc(sizeRelative));
		memcpy(m_relativePath, relativePath, sizeRelative);
	}
}