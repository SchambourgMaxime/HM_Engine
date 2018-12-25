/************************************************/
/*												*/
/*	HM_Object.h									*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-08						*/
/*												*/
/*	Last Update : 2016-08-09					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once



/*				     HEADER						*/

class HM_Object
{

public:
	/** Public functions **/

	HM_Object();
	~HM_Object();

	virtual bool isValid() const = 0;

	// Overload of if() testing
	explicit operator bool() const;

};

