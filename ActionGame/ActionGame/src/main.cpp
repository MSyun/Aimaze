//////////////////////////////////////////////////////////////
// 2016.12.25	�v���O�����쐬
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#include	<crtdbg.h>
#include	<Windows.h>
#include	"GameFramework/framework/Application/DXApplication.h"
#include	"DirectX/ActionGameDX9.h"

//************************************************************
// �O���[�o���錾
//************************************************************
DXApplication*		g_pApp = NULL;


/*									//
//			���C���֐�				//
//									*/
int APIENTRY WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	g_pApp = new DXApplication(1280, 720, true);
	g_pApp->RegistDXObj(new ActionGameDX9(g_pApp));
	g_pApp->Run(hInstance);
	delete g_pApp;

	return 0;
}