/************************************************/
/*												*/
/*	HM_FileHandler.cpp							*/
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

#include "HM_FileHandler.h"



/*				 IMPLEMENTATION					*/

	// --- STATIC VARIABLES ---

std::list<std::string> HM_FileHandler::m_listOfOpenedFiles;


	// --- CONSTRUCTOR & DESTRUCTOR ---

/*		Constructor
*
*		brief : Initialize variables and load the content of the file into the list
*
*		params : 
*			- filePath				(string)	: path to file to open
*			- readWriteRights		(uint)		: allows the file to be read, written or both
*			- replaceExistingFile	(bool)		: if opened for writing, set to true to delete existing
*												> file with the same name
**/
HM_FileHandler::HM_FileHandler(const HM_Path& filePath,
							   unsigned int ReadWriteRights,
							   bool replaceExistingFile)
							   : m_filePath("")
{

	if(filePath != "" && ReadWriteRights != HM_FILE_NULL)
		openFile(filePath, ReadWriteRights, replaceExistingFile);

}

/*		Destructor
*
*		brief : empty the list of strings and remove the file from the opened files list
*
**/
HM_FileHandler::~HM_FileHandler()
{
	// the content of the file stored in the object is cleared
	if(m_fileContent.size() > 0)
		m_fileContent.clear();

	// the file path is removed from the list of opened files
	m_listOfOpenedFiles.remove(m_filePath.getAbsolutePath());
}

/*		isValid
*
*		overload : HM_Object
*		
*		brief : determine if the class instance is usable or unset
*
*		return : bool -> indicates if the item can be used or not
*
**/
bool HM_FileHandler::isValid() const
{
	if(m_filePath == "" && m_readWriteRights == HM_FILE_NULL)
		return false;

	return true;
}

/*		openFile
*
*		pre : if a file is already opened in the instance, it will be replaced entirely
*
*		brief : opens a new file
*
*		params :
*			- filePath				(string)	: path to file to open
*			- readWriteRights		(uint)		: allows the file to be read, written or both
*			- replaceExistingFile	(bool)		: if opened for writing, set to true to delete existing
*												> file with the same name
*
*		return : bool -> indicates if the file has succesfully loaded or not
*		
**/
bool HM_FileHandler::openFile(const HM_Path &filePath,
							  unsigned int readWriteRights,
							  bool replaceExistingFile)
{
	
	m_filePath = filePath;
	m_readWriteRights = readWriteRights;

	// Check to see if file is not already opened
	if (std::find(m_listOfOpenedFiles.begin(), m_listOfOpenedFiles.end(), filePath.getAbsolutePath()) == m_listOfOpenedFiles.end())
	{
		// if file is not opened...

		// if we open a file for reading or for writing in a file already existing
		// we load its content in the class member 'm_fileContent' (list<string>)
		if (readWriteRights == HM_FILE_READ || !replaceExistingFile)
		{
			if (!loadFileInStringList())
				return false;
		}
		else
		{
			if (!checkFileCreation())
				return false;
		}

		// We store the path to the file to indicate it is already opened
		m_listOfOpenedFiles.push_back(m_filePath.getAbsolutePath());

	}
	else
	{
		// if file is already opened...

		// Error message
		std::cout << "Error : Can't open the same file twice. File : " << m_filePath << std::endl; 
		fprintf(stderr, "Error : Can't open the same file twice. File : %s \n", m_filePath.getAbsolutePath().c_str());

		// The object isn't set
		m_filePath = "";
		m_readWriteRights = HM_FILE_NULL;

		return false;
	}

	return true;

}


	// --- ACCESSORS ---

/*		getListOfOpenedFiles
*
*		return : list<string> -> copy of a list of strings representing the file path of all opened files
*
**/
const std::list<std::string>& HM_FileHandler::getListOfOpenedFiles() const
{
	return m_listOfOpenedFiles;
}

/*		getFilePath
*
*		return : string -> the path to the file opened now
*
**/
const HM_Path& HM_FileHandler::getFilePath() const
{
	return m_filePath;
}

/*		getReadWriteRights
*
*		return : uint -> corresponds to one of the constant for the read and write rights
*
**/
unsigned int HM_FileHandler::getReadWriteRights() const
{
	return m_readWriteRights;
}

/*		getFileNumberOfLines
*
*		return : uint -> the number of lines in the file
*
**/
unsigned int HM_FileHandler::getFileNumberOfLines() const
{
	return (unsigned int)m_fileContent.size();
}


	// --- PUBLIC CLASS FUNCTIONS ---

/*		getFileContentStringList
*		
*		pre : must have READ rights for this function to work
*
*		brief : get a list of strings storing the content of the opened file
*
*		params :
*			- startLine				(int)		: line to start reading from
*			- finishLine			(int)		: line to stop reading at
*
*		return : list<string> -> copy of the list of strings storing the content of the opened file
**/
std::list<std::string> const HM_FileHandler::getFileContentStringList(int startLine,
																	  int finishLine) const
{

	// Check for READ rights
	if (!checkReadWriteRights(HM_FILE_READ))
		return std::list<std::string>();

	// copy of the file content
	std::list<std::string> fileContentCopy;

	// Check to see if the whole file is to be returned or just a fraction
	if(startLine == -1 && finishLine == -1)
		fileContentCopy = m_fileContent;
	else
		fileContentCopy = getPortionOfFilecontent(startLine, finishLine);

	return fileContentCopy;

}

/*		getFileContentCharArray
*
*		pre : must have READ rights for this function to work
*
*		brief : get a 2 dimensions array of chars storing the content of the opened file
*
*		params :
*			- startLine				(int)		: line to start reading from
*			- finishLine			(int)		: line to stop reading at
*
*		return : char** -> copy of the list of strings storing the content of the opened file
**/
char** const HM_FileHandler::getFileContentCharArray(int startLine, 
													 int finishLine) const
{

	// copy of the file content
	std::list<std::string> fileContentCopy = getFileContentStringList(startLine, finishLine);
	
	if(fileContentCopy.empty())
		return NULL;

	char** arrayOfCharFileContent = new char*[fileContentCopy.size()];

	std::list<std::string>::iterator iter;
	int i = 0;
	for (iter = fileContentCopy.begin(); iter != fileContentCopy.end(); ++iter)
	{
		std::string line = *iter;

		arrayOfCharFileContent[i] = new char[line.size() + 1];
		strcpy_s(arrayOfCharFileContent[i], line.size() + 1, line.c_str());
		arrayOfCharFileContent[i][line.size()] = '\0';

		i++;
	}

	return arrayOfCharFileContent;

}

/*		getFileContentSingleString
*
*		pre : must have READ rights for this function to work
*
*		brief : get a string storing the content of the opened file
*
*		params :
*			- startLine				(int)		: line to start reading from
*			- finishLine			(int)		: line to stop reading at
*
*		return : string -> copy of the list of strings storing the content of the opened file
**/
std::string const HM_FileHandler::getFileContentSingleString(int startLine,
															 int finishLine) const
{

	std::list<std::string> fileContentCopy = getFileContentStringList(startLine, finishLine);

	if (fileContentCopy.empty())
		return NULL;

	std::string singleStringFileContent = "";

	for each (std::string line in fileContentCopy)
		singleStringFileContent += line + '\n';

	return singleStringFileContent;
}

/*		getFileContentVectorString
*
*		pre : must have READ rights for this function to work
*
*		brief : get a vector of strings storing the content of the opened file
*
*		params :
*			- startLine				(int)		: line to start reading from
*			- finishLine			(int)		: line to stop reading at
*
*		return : vector<string> -> copy of the list of strings storing the content of the opened file
**/
std::vector<std::string> const HM_FileHandler::getFileContentVectorString(int startLine,
																		  int finishLine) const 
{

	std::list<std::string> fileContentCopy = getFileContentStringList(startLine, finishLine);

	if (fileContentCopy.empty())
		return std::vector<std::string>();

	std::vector<std::string> vectorOfStringFileContent {std::begin(fileContentCopy), std::end(fileContentCopy)};

	return vectorOfStringFileContent;
}

/*		getSectionWithTitle
*
*		brief : get a section under a title surrounded with brackets
*
*		params :
*			- headerName			(string)	: Name of the title to the section without brackets
*
*		return : list<string> -> list of strings storing the content below the title given and before the next title
**/
std::list<std::string> const HM_FileHandler::getSectionWithTitle(std::string titleName) const
{

	std::list<std::string> sectionList;

	std::transform(titleName.begin(), titleName.end(), titleName.begin(), ::toupper);

	std::string titleActualName = '[' + titleName + ']';

	int copying = false;

	std::list<std::string>::const_iterator iter;
	for (iter = m_fileContent.begin(); iter != m_fileContent.end(); iter++)
	{
		std::string currentLine = *iter;
		if (!copying)
		{
			if(currentLine == titleActualName)
				copying = true;
		}
		else
		{
			if(currentLine.size() != 0U && (currentLine.at(0) == '[') && (currentLine.at(currentLine.size() - 1) == ']'))
				break;

			sectionList.push_back(currentLine);
		}
	}

	return sectionList;
}

/*		getline
*
*		pre : must have READ rights for this function to work
*
*		brief : get a line from the file content
*
*		params :
*			- lineNumber			(int)		: number of the line to get
*
*		return : string -> copy of the string holding the line desired
**/
std::string const HM_FileHandler::getLine(int line) const
{

	if (!checkReadWriteRights(HM_FILE_READ))
		return NULL;
	
	std::string returnLine;
	
	std::list<std::string>::const_iterator iter = m_fileContent.begin();

	std::advance(iter, line);

	returnLine = *iter;

	return returnLine;
}


/*		overWriteFileContent
*
*		pre : must have WRITE rights for this function to work
*			  for optimised treatment send list of strings with no strings containing '\n'
*
*		brief : erases all existing content and writes text given
*
*		params :
*			- textToWrite			(list<string>)	: list of lines to write in the file
*
**/
void HM_FileHandler::overWriteFileContent(std::list<std::string> textToWrite)
{
	if (!checkReadWriteRights(HM_FILE_WRITE))
		return;

	checkStringListFormating(&textToWrite);

	if (m_fileContent.size() != 0)
		m_fileContent.clear();

	m_fileContent = textToWrite;
}

/*		overWriteFileContent
*
*		pre : must have WRITE rights for this function to work
*			  for optimised treatment send list of strings with no strings containing '\n'
*
*		brief : erases all existing content and writes text given
*
*		params :
*			- textToWrite			(string)	: content to write in the file
*
**/
void HM_FileHandler::overWriteFileContent(std::string textToWrite)
{

	overWriteFileContent(hmu::turnStringIntoStringList(textToWrite));

}

/*		insertAtLine
*
*		pre : must have WRITE rights for this function to work
*			  for optimised treatment send list of strings with no strings containing '\n'
*
*		brief : inserts text given in the file at specified line
*
*		params :
*			- textToWrite			(list<string>)	: list of lines to write in the file
*			- line					(int)			: line where the text should be inserted
*
**/
void HM_FileHandler::insertAtLine(std::list<std::string> textToWrite, int line)
{

	if (!checkReadWriteRights(HM_FILE_WRITE))
		return;
	
	checkStringListFormating(&textToWrite);

	std::list<std::string>::iterator iter;

	iter = m_fileContent.begin();
	std::advance(iter, line);

	m_fileContent.insert(iter, textToWrite.begin(), textToWrite.end());

}

/*		insertAtLine
*
*		pre : must have WRITE rights for this function to work
*			  for optimised treatment send list of strings with no strings containing '\n'
*
*		brief : inserts text given in the file at specified line
*
*		params :
*			- textToWrite			(string)		: content to write in the file
*			- line					(int)			: line where the text should be inserted
*
**/
void HM_FileHandler::insertAtLine(std::string textToWrite, int line)
{

	insertAtLine(hmu::turnStringIntoStringList(textToWrite), line);

}

/*		append
*
*		pre : must have WRITE rights for this function to work
*			  for optimised treatment send list of strings with no strings containing '\n'
*
*		brief : Write a text at the end of the file
*
*		params :
*			- textToWrite			(list<string>)	: list of lines to write in the file
*
**/
void HM_FileHandler::append(std::list<std::string> textToWrite)
{

	if (!checkReadWriteRights(HM_FILE_WRITE))
		return;

	checkStringListFormating(&textToWrite);

	m_fileContent.insert(m_fileContent.end(), textToWrite.begin(), textToWrite.end());

}

/*		append
*
*		pre : must have WRITE rights for this function to work
*			  for optimised treatment send list of strings with no strings containing '\n'
*
*		brief : Write a text at the end of the file
*
*		params :
*			- textToWrite			(string)		: content to write in the file
*
**/
void HM_FileHandler::append(std::string textToWrite)
{

	append(hmu::turnStringIntoStringList(textToWrite));

}

/*		flushTextToFile
*
*		pre : must have WRITE rights for this function to work
*
*		brief : create file if needed and write text stored in this instance in the file
*
**/
void HM_FileHandler::flushTextToFile()
{
	if (!checkReadWriteRights(HM_FILE_WRITE))
		return;

	std::ofstream outFile(m_filePath.getAbsolutePath());

	std::list<std::string>::iterator iter;

	for (iter = m_fileContent.begin(); iter != m_fileContent.end(); iter++)
	{
		std::string currentString = *iter;
		outFile << currentString << '\n';
	}
}


	// --- PRIVATE CLASS FUNCTIONS ---

/*		loadFileInStringList
*
*		brief : open a file and store its content in this instance of the file handler
*
*		return : bool -> was it possible to load the file in the class instance
*
**/
bool HM_FileHandler::loadFileInStringList()
{

	if(m_fileContent.size() > 0)
		m_fileContent.clear();

	std::ifstream fileTemp(m_filePath.getAbsolutePath());

	if (fileTemp)
	{

		std::string line;

		while (getline(fileTemp, line))
			m_fileContent.push_back(line);

	}
	else
	{

		std::cout << "Warning : File not found : " << m_filePath << std::endl;
		std::cerr << "Warning : File not found : " << m_filePath << std::endl;

		m_filePath = "";
		m_readWriteRights = HM_FILE_NULL;

		return false;
	}

	return true;

}

/*		checkFileCreation
*
*		brief : check if a new file can be created at the m_filePath
*
*		return : bool -> can the file be created at the m_filePath location
*		
**/
bool HM_FileHandler::checkFileCreation()
{
	std::ofstream testFile(m_filePath.getAbsolutePath());

	if (!testFile)
	{
		std::cout << "Error : File \'" << m_filePath << "\' could not be created (Check if hierarchy exists)" << std::endl;
		fprintf(stderr, "Error : File \'%s\' could not be created (Check if hierarchy exists)\n", m_filePath.getAbsolutePath().c_str());

		m_filePath = "";
		m_readWriteRights = HM_FILE_NULL;

		return false;
	}

	testFile.close();

	return true;
}

/*		getPortionOfFilecontent
*
*		brief : get a portion of the file content stored
*
*		params :
*			- startLine				(int)		: line to start reading from
*			- finishLine			(int)		: line to stop reading at
*
*		return : list<string> -> list of lines between given line numbers
**/
std::list<std::string> HM_FileHandler::getPortionOfFilecontent(int startLine, int finishLine) const
{
	if (startLine != -1 && finishLine == -1)
		finishLine = m_fileContent.size() - 1;

	if ((startLine < 0 || finishLine < 0) ||
		(startLine >= static_cast<int>(m_fileContent.size()) || finishLine >= static_cast<int>(m_fileContent.size())))
	{
		std::cout << "Error : Can't read lines of file \'" << m_filePath << "\' under 0 or over the number of existing lines" << std::endl;
		fprintf(stderr, "Error : Can't read lines of file \'%s\' under 0 or over the number of existing lines\n", m_filePath.getAbsolutePath().c_str());

		return std::list<std::string>(0);
	}

	std::list<std::string> portionOfFileContent;
	std::list<std::string>::const_iterator iter = m_fileContent.begin();

	if(startLine != 0)
		std::advance(iter, startLine);

	for(int i = startLine;iter != m_fileContent.end() && i <= finishLine; iter++, i++)
	{
		std::string currentString = *iter;
		portionOfFileContent.push_back(currentString);
	}

	return portionOfFileContent;
}

/*		checkStringListFormating
*
*		brief : check the presence of the '\n' in a list of strings and fixes it
*				by adding new lines if found
*
*		params :
*			- listToCheck			(list<sting>*)	:  pointer to the list that needs checking

**/
void HM_FileHandler::checkStringListFormating(std::list<std::string>* listToCheck)
{

	std::list<std::string>::iterator iter;
	int i = 0;

	for (iter = listToCheck->begin(); iter != listToCheck->end(); iter++, i++)
	{
		
		std::string currentString = *iter;

		if(currentString.find('\n') != std::string::npos)
		{

			std::cout << "Warning : passing list of string with string possessing the \'\\n\' character in file : " << m_filePath << std::endl;
			fprintf(stderr, "Error : Warning : passing list of string with string possessing the \'\\n\' character in file : %s\n", m_filePath.getAbsolutePath().c_str());

			std::list<std::string> stringAsList = hmu::turnStringIntoStringList(currentString);

			listToCheck->insert(iter, stringAsList.begin(), stringAsList.end());
			iter = listToCheck->erase(iter);
			iter--;

		}

	}

}

/*		checkReadWriteRights
*
*		brief : check if the rights of the file handler allows an action
*
*		params :
*			- readWriteRights		(uint)			:  type of right needed for action

**/
bool HM_FileHandler::checkReadWriteRights(unsigned int readWriteRights) const
{
	switch (m_readWriteRights)
	{

	case HM_FILE_NULL:

		std::cout << "Error : File \'" << m_filePath << "\' is not initialized or has no rights";
		fprintf(stderr, "Error : File \'%s\' is not initialized or has no rights\n", m_filePath.getAbsolutePath().c_str());

		return false;

	case HM_FILE_READWRITE:

		return true;

	case HM_FILE_READ:
		if (!(readWriteRights == HM_FILE_READ))
		{
			std::cout << "Error : Can't read file \'" << m_filePath << "\' opened for writing only";
			fprintf(stderr, "Error : Can't read file \'%s\' opened for writing only\n", m_filePath.getAbsolutePath().c_str());

			return false;
		}

		return true;

	case HM_FILE_WRITE:
		if (!(readWriteRights == HM_FILE_WRITE))
		{
			std::cout << "Error : Can't write in file \"" << m_filePath << "\" opened for read only";
			fprintf(stderr, "Error : Can't write in file \'%s\' opened for read only\n", m_filePath.getAbsolutePath().c_str());

			return false;
		}

		return true;

	default:
		
		std::cout << "Error : Number not associated with any read or write right, use HM_FILE_ constant";
		fprintf(stderr, "Error : Number not associated with any read or write right, use HM_FILE_ constant");

		return false;
	}
}