/************************************************/
/*												*/
/*	HM_Utilities.h								*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-11						*/
/*												*/
/*	Last Update : 2016-08-20					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include <list>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "HM_Cube.h"



/*				     HEADER						*/

namespace hmu
{

	template<typename T>
	static T* convertListToArray(std::list<T> const &);
	static void* getDataFromString(std::string const &);
	static std::list<std::string> turnStringIntoStringList(std::string const &);
	static std::vector<float> getFloatsInString(std::string const &, 
												unsigned int offset = 0U);
	static std::vector<int> getIntsInString(std::string const &,
											unsigned int offset = 0U);
	static std::vector<std::string> getStringArrayFromString(std::string const & stringContainer,
															 std::string const & separators,
															 unsigned int offset = 0U);


	template<typename T>
	static T* convertListToArray(std::list<T> const & listToConvert)
	{

		if (listToConvert.empty())
			return NULL;

		T* arrayConverted = new T[listToConvert.size()];

		std::list<T>::const_iterator iter;

		int i = 0;
		for (iter = listToConvert.begin(); iter != listToConvert.end(); iter++, i++)
		{
			T currentObject = *iter;

			arrayConverted[i] = currentObject;
		}

		return arrayConverted;
	}

	static void* getDataFromString(std::string const & stringOfData)
	{
		if(stringOfData[0] != ':')
			return NULL;

		unsigned char indexDataStart = static_cast<unsigned char>(stringOfData.find_first_of(':', 1));

		std::string type;
		type = stringOfData.substr(1, indexDataStart - 1);

		std::string data;
		data = stringOfData.substr(indexDataStart + 1);

		if (type == "vec3" || type == "vec2")
		{

			std::vector<float> dataFloat(getFloatsInString(data));

			if(dataFloat.size() != atoi(&type.at(3)))
				return NULL;

			if (type.at(3) == '2')
				return new glm::vec2(dataFloat[0], dataFloat[1]);
			else if (type.at(3) == '3')
				return new glm::vec3(dataFloat[0], dataFloat[1],
									 dataFloat[2]);

		}
		else if (type == "cube")
		{
			
			std::vector<float> dataFloat(getFloatsInString(data));

			if(dataFloat.size() != 6)
				return NULL;

			return new HM_Cube(dataFloat[0], dataFloat[1],
							   dataFloat[2], dataFloat[3],
							   dataFloat[4], dataFloat[5]);

		}
		else if (type == "float")
		{

			float* floatNum = new float(std::stof(data));
			return floatNum;

		}
		else if (type == "int")
		{

			int* integerNum = new int(std::stoi(data));
			return integerNum;

		}
		else if (type == "bool")
		{

			if (data == "false")
				return new bool(false);
			else if(data == "true")
				return new bool(true);
			else
				return NULL;

		}
		else if(type == "str")
		{

			return new std::string(data);

		}

		return NULL;

	}

	// to delete maybe
	/*static float* getFloatArrayFromStringsList(std::list<std::string> listOfFloatStrings, unsigned int* size)
	{
		std::list<std::string>::iterator iter;
		iter = listOfFloatStrings.begin();

		std::list<float> verticesList;

		for (; iter != listOfFloatStrings.end(); iter++)
		{

			std::string currentVerticesString = *iter;

			int cursor = 0;
			int oldCursor = 0;

			while (cursor < static_cast<int>(currentVerticesString.size() - 1))
			{

				cursor = currentVerticesString.find_first_of("-0123456789", cursor);

				oldCursor = cursor;

				cursor = currentVerticesString.find_first_not_of(".-0123456789", cursor);

				if (cursor == -1)
					cursor = currentVerticesString.size();

				std::string currentNumberAsString = currentVerticesString.substr(oldCursor, cursor - oldCursor);

				verticesList.push_back(std::stof(currentNumberAsString));

			}

		}

		*size = verticesList.size();

		return hmu::convertListToArray<float>(verticesList);

	}*/

	/*		turnStringIntoStringList
	*
	*		brief : turn a string and turn it into a list of lines broken down
	*			at the '\n' mark
	*
	*		params :
	*			- stringToTransform		(string)		: string to turn in 
	*													  a list of lines
	*
	*		return : list<string> -> list of lines that composed the given
	*								 string
	**/
	static std::list<std::string> turnStringIntoStringList(std::string const & stringToTransform)
	{
		std::list<std::string> listOfLines;

		int offset = 0;
		int previousOffset = 0;

		while ((offset = stringToTransform.find_first_of('\n', offset + 1)) != -1)
		{
			listOfLines.push_back(stringToTransform.substr(previousOffset, offset - previousOffset));
			previousOffset = offset + 1;
		}

		listOfLines.push_back(stringToTransform.substr(previousOffset, offset - previousOffset));

		return listOfLines;
	}

	/*		getFloatsInString
	*
	*		brief : search and stores floats contained in a given string
	*
	*		params :
	*			- stringContainer	(string)	: container of the floats as strings
	*			- offset			(uint)		: the index where to start the
	*											  search for floats
	*
	*		return : vector<float> -> list of floats in the string
	**/
	static std::vector<float> getFloatsInString(std::string const & stringContainer, 
												unsigned int offset)
	{

		std::vector<float> floats;
		unsigned int cursor = offset, oldCursor = offset;

		while(cursor < stringContainer.size())
		{

			cursor = stringContainer.find_first_of("-0123456789", cursor);

			if(cursor == -1)
				break;

			oldCursor = cursor;
			cursor = stringContainer.find_first_not_of("-.0123456789", cursor);

			if(cursor == -1)
				cursor = stringContainer.size();

			 floats.push_back(stof(stringContainer.substr(oldCursor, cursor)));

		}

		return floats;
	}

	/*		getIntsInString
	*
	*		brief : search and stores ints contained in a given string
	*
	*		params :
	*			- stringContainer	(string)	: container of the floats as strings
	*			- offset			(uint)		: the index where to start the
	*											  search for ints
	*
	*		return : vector<int> -> list of ints in the string
	**/
	static std::vector<int> getIntsInString(std::string const & stringContainer, 
											unsigned int offset)
	{
		
		std::vector<int> ints;
		unsigned int cursor = offset, oldCursor = offset;

		while (cursor < stringContainer.size())
		{

			cursor = stringContainer.find_first_of("-0123456789", cursor);

			if (cursor == -1)
				break;

			oldCursor = cursor;
			cursor = stringContainer.find_first_not_of("-0123456789", cursor);

			if (cursor == -1)
				cursor = stringContainer.size();

			ints.push_back(stoi(stringContainer.substr(oldCursor, cursor)));

		}

		return ints;

	}

	static std::vector<std::string> getStringArrayFromString(std::string const & stringContainer,
															 std::string const & separators,
															 unsigned int offset)
	{

		std::vector<std::string> strings;
		unsigned int cursor = offset, oldCursor = offset;

		while (cursor < stringContainer.size())
		{

			cursor = stringContainer.find_first_of(separators, cursor);

			if(cursor == -1)
				cursor = stringContainer.size();

			strings.push_back(stringContainer.substr(oldCursor, 
													 cursor - oldCursor));

			if(cursor < stringContainer.size())
				oldCursor = cursor = stringContainer.find_first_not_of(separators,
																	   cursor);

		}

		return strings;

	}

	template<typename T>
	static T getDataFromVoid(void* data, bool freeMemory = false)
	{

		T returnData;

		if(data)
			returnData = *(static_cast<T*>(data));

		if(freeMemory)
			delete data;

		return returnData;

	}

	template<typename T>
	static void putDataFromVoid(void* data, T* recepter)
	{

		recepter = *(static_cast<T*>(data));

	}

}