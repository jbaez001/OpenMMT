
// OpenMMT.h : main header file for the PROJECT_NAME application
//

#pragma once

#include "resource.h"		// main symbols


// OpenMMTApp:
// See OpenMMT.cpp for the implementation of this class
//

class OpenMMTApp : public CWinApp
{
public:
	OpenMMTApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern OpenMMTApp theApp;