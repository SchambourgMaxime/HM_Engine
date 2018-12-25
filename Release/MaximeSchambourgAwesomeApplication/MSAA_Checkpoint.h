/************************************************/
/*												*/
/*	MSAA_Checkpoint.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-23						*/
/*												*/
/*	Last Update : 2016-11-23					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "..\Components\HM_Component.h"



/*				     HEADER						*/

class MSAA_Checkpoint :
	public HM_Component
{

public:
	/** Public functions **/

	MSAA_Checkpoint(HM_SceneObject* owner) : HM_Component(owner)
	{};
	~MSAA_Checkpoint();

	MSAA_Checkpoint* create(HM_SceneObject* owner) const override;
	bool setup(std::map<std::string, void*> descr) override { return true; };
	virtual void update() override {};
	virtual void display() override {};

	virtual void onCollision(HM_SceneObject* other, Direction direction) override;

};

