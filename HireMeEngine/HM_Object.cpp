/************************************************/
/*												*/
/*	HM_Object.cpp								*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-08-09						*/
/*												*/
/*	Last Update : 2016-08-09					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#include "HM_Object.h"



/*				 IMPLEMENTATION					*/

HM_Object::HM_Object()
{
}


HM_Object::~HM_Object()
{
}

HM_Object::operator bool() const
{
	return isValid();
}
