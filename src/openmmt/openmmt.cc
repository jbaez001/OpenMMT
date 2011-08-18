#include "openmmt/precompiled_headers.h"
#include "openmmt/openmmt.h"
#include "openmmt/ui/settings_dlg.h"
#include "libsys/api.h"

#ifdef _DEBUG
# define new DEBUG_NEW
#endif


// OpenMMTApp

BEGIN_MESSAGE_MAP(OpenMMTApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// OpenMMTApp construction

OpenMMTApp::OpenMMTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only OpenMMTApp object

OpenMMTApp theApp;


// OpenMMTApp initialization

BOOL OpenMMTApp::InitInstance()
{
	CWinApp::InitInstance();

	boost::scoped_ptr<CShellManager> pShellManager(new CShellManager);

	SetRegistryKey(L"OpenMMT");

	SettingsDlg dlg;
	m_pMainWnd = &dlg;
  
  dlg.DoModal();

	return FALSE;
}

