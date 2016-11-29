/************************************************/
/*												*/
/*	MSAA_Activator.h							*/
/*												*/
/*	Version 1.0									*/
/*												*/
/*	Created : 2016-11-24						*/
/*												*/
/*	Last Update : 2016-11-24					*/
/*												*/
/*	Author : Maxime Schambourg					*/
/*												*/
/************************************************/

/*				  PREPROCESSOR					*/

#pragma once

#include "..\Components\HM_Component.h"



/*				     HEADER						*/

class MSAA_Activator :
	public HM_Component
{

public:
	MSAA_Activator(HM_SceneObject* owner) : HM_Component(owner),
	m_isActivated(false), m_activationStart(0), m_activatedTime(0){};
	~MSAA_Activator();

	MSAA_Activator* create(HM_SceneObject* owner) const override;

	bool setup(std::map<std::string, void*> descr) override;
	bool onSetupEnd(std::map<std::string, void*> descr) override;

	virtual void update() override;

	virtual void display() override {};

	virtual void onCollision(HM_SceneObject* other, Direction direction) override;

private:
	
	bool m_isActivated;
	Uint32 m_activationStart;
	Uint32 m_activatedTime;

};

