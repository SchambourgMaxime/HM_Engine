/************************************************/
/*												*/
/*	HM_FileHandler.h							*/
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

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>

#include "HM_Object.h"
#include "HM_Utilities.h"

#define HM_FILE_NULL 0
#define HM_FILE_READ 1
#define HM_FILE_WRITE 2
#define HM_FILE_READWRITE 3



/*				     HEADER						*/

/*		Class HM_FileHandler
*
*		brief : allows to read or write a file, create it if needed,
*		overwrite it and insert text into it
*
**/
class HM_FileHandler : public HM_Object
{

public:
			/** Public functions **/

	// Constructor
	HM_FileHandler(std::string const &filePath = "", 
				   unsigned int ReadWriteRights = HM_FILE_NULL,
				   bool replaceExistingFile = false);
	// Destructor
	~HM_FileHandler();

	// Object overloaded function : checks validity of object (is it usable?)
	virtual bool isValid() const override;

	// Opens a file and loads its content in the list of lines
	bool openFile(std::string const &filePath, unsigned int readWriteRights, 
				  bool replaceExistingFile);

		// --- Accessors ---

	std::list<std::string> const & getListOfOpenedFiles() const;
	std::string const & getFilePath() const;
	unsigned int getReadWriteRights() const;	
	unsigned int getFileNumberOfLines() const;


		// --- READ ---

	// Get the file content as a list of lines in strings
	std::list<std::string> const getFileContentStringList(int startLine = -1, 
													int finishLine = -1) const;
	// Get the file content as an array of lines in string arrays
	char** const getFileContentCharArray(int startLine = -1,
								   int finishLine = -1) const;
	// Get the file content as a single string
	std::string const getFileContentSingleString(int startLine = -1,
										   int finishLine = -1) const;
	// Get the file content as a vector of strings
	std::vector<std::string> const getFileContentVectorString(int startLine = -1,
														int finishLine = -1) const;

	// Get a part of file under a title like this : [titleName]
	std::list<std::string> const getSectionWithTitle(std::string titleName) const;

	// Get a single line of the file
	std::string const getLine(int line) const;


		// --- WRITE ---

	// Erase all content in the file and writes list of string given
	void overWriteFileContent(std::list<std::string> textToWrite);
	// Erase all content in the file and writes string given
	void overWriteFileContent(std::string textToWrite);

	// Insert a certain text given in a list of strings into the file 
	// content at the specified line (starts at 0)
	void insertAtLine(std::list<std::string> textToWrite, int line);
	// Insert a certain text given in a string into the file
	// content at the specified line (starts at 0)
	void insertAtLine(std::string textToWrite, int line);

	// Write a certain text given in a list of strings at the end of the file
	void append(std::list<std::string> textToWrite);
	// Write a certain text given in a string at the end of the file
	void append(std::string textToWrite);
	
	// Write the content of the list of string into the file
	void flushTextToFile();

private:
			/** Private functions **/

	// Reads the file and stores its content in a list of string
	bool loadFileInStringList();

	// Checks if a new file can be created
	bool checkFileCreation();

	// Allows to get a portion of the file content as list of strings
	std::list<std::string> getPortionOfFilecontent(int startLine, int finishLine) const;

	// Checks if a list of string does not contain any '\n'
	void checkStringListFormating(std::list<std::string>* listToCheck);

	// Checks if the read write rights allows an action
	bool checkReadWriteRights(unsigned int readWriteRights) const;


			/** Private variables **/

	// List of already opened file to prevent opening a file twice
	static std::list<std::string> m_listOfOpenedFiles;

	// Path of the file to read or write
	std::string m_filePath;

	// Decides if the file can be read, written or both 
	unsigned int m_readWriteRights;

	// list of strings containing the content of the file
	std::list<std::string> m_fileContent;

	// boolean that checks if a file is loaded
	bool m_isAFileLoaded;
};