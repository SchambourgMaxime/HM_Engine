/************************************************/
/*												*/
/*	HM_Log.cpp									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-07-27						*/
/*												*/
/*	Last Update : 2016-08-02					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_Log.h"



/*				 IMPLEMENTATION					*/

	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Default constructor
*
*		brief : initialize variables
*
**/
HM_Log::HM_Log() : m_name(""), m_entryID(0)
{
	
}

/*		Constructor
*
*		pre : save old log if needed, it is destroyed in the constructor
*
*		brief : initialize variables, create file handler and write header to the log file
*
*		params :
*			- name					(string)	: name to give the log file
*			
**/
HM_Log::HM_Log(std::string name) : m_name(name), m_entryID(0)
{

	createLog(name);

}

/*		Destructor
*
*		brief : flush the log to the file and free the file's memory
*
**/
HM_Log::~HM_Log()
{

	m_file.flushTextToFile();

}


	// --- PUBLIC CLASS FUNCTIONS ---

/*		isValid
*
*		Overload : HM_Object
*
*		brief : determine if the class instance is usable or unset
*
*		return : bool -> indicates if the item can be used or not
*
**/
bool HM_Log::isValid() const
{
	return m_file.isValid();
}

/*		addLineToLog
*
*		brief : create a new line with ID, date & time, calling file, function and line number
*
*
**/
bool HM_Log::createLog(std::string name)
{
	// Creation of the file in the 'logs' directory
	std::string filePath = "logs/" + name + ".hmlog";

	// Initialization of the file handler (we erase the old version of the log every time)
	m_file.openFile(filePath, HM_FILE_WRITE, true);

	if (!m_file)
		return false;

	// Creation of the Header Lines
	std::list<std::string> listOfHeaderLines;

	listOfHeaderLines.push_back("LOG ENTRY");
	listOfHeaderLines.push_back("---------");
	listOfHeaderLines.push_back("");
	listOfHeaderLines.push_back(name + ".hmlog");
	listOfHeaderLines.push_back("");
	listOfHeaderLines.push_back("entry ID\tTime\t\t\tFile\t\t\tLineNumber\tMessage\t\t\t\t\t\t");

	// Save of the header lines in the file
	m_file.append(listOfHeaderLines);

	return true;
}

/*		addLineToLog
*
*		brief : create a new line with ID, date & time, calling file, function and line number
*
*		params :
*			- message				(string)	: message included with the line
*			- fileName				(string)	: name of the file creating the line (use __FILE__)
*			- functionName			(string)	: name of the function calling (use __FUNCTION__)
*			- lineNumber			(int)		: line of the call (use __LINE__)
*
**/
void HM_Log::addLineToLog(std::string message, std::string fileName, std::string functionName, int lineNumber)
{
	
	if(!isValid())
		return;

	std::string line;

	char bufferEntryID[33];
	_itoa_s(m_entryID++, bufferEntryID, 10);

	char bufferLineNumber[33];
	_itoa_s(lineNumber, bufferLineNumber, 10);

	std::string fileNameShown;
	int offset = 0;
	bool fileNameTooLong = false;

	if ((offset = fileName.find_last_of('\\')) != -1)
	{
		fileNameShown = fileName.substr(offset + 1);
	}
	else
	{
		fileNameShown = fileName;
	}

	line += bufferEntryID;
	line += "\t\t";
	line += "[";
	line += __DATE__;
	line += " ";
	line += __TIME__;
	line += "]";
	line += "\t";
	if (fileNameShown.size() > 23)
	{
		line += fileNameShown.substr(0, 23);
		line += "\t";
		fileNameTooLong = true;
	}
	else if (fileNameShown.size() > 15)
	{
		line += fileNameShown;
		line += "\t";
	}
	else
	{
		line += fileNameShown;
		line += "\t\t";
	}
	line += bufferLineNumber;
	line += "\t\t";
	line += functionName;
	line += "() : ";
	line += message;

	if (fileNameTooLong)
	{
		std::list<std::string> listOfLines;
		listOfLines.push_back(line);
		int cursor = 23;
		while(cursor < static_cast<int>(fileNameShown.size()))
		{
			std::string nextLine = "\t\t\t\t\t";
			if(cursor + 23 < static_cast<int>(fileNameShown.size()))
				nextLine += fileNameShown.substr(cursor, 23);
			else
				nextLine += fileNameShown.substr(cursor, fileNameShown.size());

			listOfLines.push_back(nextLine);

			cursor += 23;
		}

		m_file.append(listOfLines);
	}
	else
	{
		m_file.append(line);
	}
}

/*		flush
*
*		brief : flush the log contained in the file handler in the actual file
*
**/
void HM_Log::flush()
{

	if(!isValid())
		return;

	m_file.flushTextToFile();

}
