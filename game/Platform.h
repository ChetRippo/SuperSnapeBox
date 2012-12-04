#pragma once

#include "stdafx.h"
//class for platforms. This is not a struct because later these will be more complicated.
class Platform{
public:
	//constructor
	Platform();
	//destructor
	~Platform();
	float			m_X;
	float			m_Y;
	int				m_Width;
	int				m_Height;
};