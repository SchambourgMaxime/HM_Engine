#pragma once
#include <corecrt_malloc.h>
#include <string.h>
#include <iostream>
using namespace std;

#define ROOT_FOLDER "../Resources/"

class HM_Path
{
public:
	HM_Path(const std::string& relativePath)
	{
		setPath(relativePath.c_str());
	}

	HM_Path(const char* relativePath)
	{
		setPath(relativePath);
	}

	HM_Path(const HM_Path& other)
	{
		setPath(other.m_relativePath);
	}

	void operator=(const char* relativePath) { setPath(relativePath); }
	void operator=(const HM_Path& other) { setPath(other.m_relativePath); }
	void setPath(const char* relativePath);

	bool operator==(const char* relativePath) const { return strcmp(m_relativePath, relativePath) == 0; };
	bool operator!=(const char* relativePath) const { return strcmp(m_relativePath, relativePath) != 0; };
	bool isValid() { return m_relativePath != nullptr; };

	bool operator<<(const char* relativePath) const { return strcmp(m_relativePath, relativePath) != 0; };

	const std::string getRelativePath() const { return std::string(m_relativePath); };
	const std::string getAbsolutePath() const { return std::string(ROOT_FOLDER) + m_relativePath; };

	operator char*() const { return m_relativePath; }
	operator const char*() const { return m_relativePath; }

	~HM_Path()
	{
		if(m_relativePath != nullptr)
			freePath();
	}

private:

	void freePath() { free(m_relativePath); m_relativePath = nullptr; }

	static const char* rootPath;
	char* m_relativePath = nullptr;
};