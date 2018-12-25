/************************************************/
/*												*/
/*	HM_Log.h									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-07-27						*/
/*												*/
/*	Last Update : 2016-08-20					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#define _ADDLINETOLOG(message) \
	addLineToLog(message, __FILE__, __FUNCTION__, __LINE__)
#define _ADDLINETOLOG_LINE(message, lineNumber) \
	addLineToLog(message, __FILE__, __FUNCTION__, lineNumber)

#include "HM_FileHandler.h"



/*				     HEADER						*/

/*		Class HM_Log
*
*		pre : the project needs a 'logs' directory at its source
*
*		brief : Creates a log for which you can add lines
*
**/
class HM_Log : public HM_Object
{

public:
			/** Public functions **/

	// Constructor
	HM_Log();
	// Constructor
	HM_Log(std::string name);
	// Destructor
	~HM_Log();

	// Object overloaded function : checks validity of object (is it usable?)
	virtual bool isValid() const override;

	// Creates the log file handler
	bool createLog(std::string name);
	// Adding a new line to the log
	void addLineToLog(std::string message, std::string fileName, std::string functionName, int lineNumber);
	// Creates a log file and writes 'finalLog' in it
	void flush();


private:
			/** Private Variables **/

	// Name of the log
	std::string m_name;

	// Reference to file handler
	HM_FileHandler m_file;

	// Number used for ID of log entries
	unsigned int m_entryID;

};

