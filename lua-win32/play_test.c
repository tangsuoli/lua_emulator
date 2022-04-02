#include <Windows.h>
#include <MMSystem.h>

#pragma comment(lib,"winmm.lib")
#include <stdbool.h>

extern HWND g_hwnd;

MCI_OPEN_PARMS m_mciOpen;  //打开参数
MCI_PLAY_PARMS m_mciPlay;  //播放参数

bool PlayMP3(char * szFileName)
{
	m_mciOpen.lpstrDeviceType = "mpegvideo"; //要操作的文件类型
	m_mciOpen.lpstrElementName = szFileName; //要操作的文件路径
	MCIERROR mcierror=mciSendCommand( 0,MCI_OPEN,MCI_OPEN_TYPE | MCI_OPEN_ELEMENT ,(DWORD)&m_mciOpen); //打开文件命令
	if(mcierror){
		return false;
	}else{
		m_mciPlay.dwCallback=(DWORD)g_hwnd;
		m_mciPlay.dwFrom = (DWORD)0; //播放起始位置ms为单位
		mciSendCommand(m_mciOpen.wDeviceID, MCI_PLAY, MCI_NOTIFY|MCI_FROM, (DWORD)(LPVOID)&m_mciPlay);
	}
	return true;
}



int StopMP3()
{
	int dwResult = 0;

	if (m_mciOpen.wDeviceID)
	{
		dwResult = mciSendCommand(m_mciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);
		m_mciOpen.wDeviceID = 0;
	}

	return dwResult;
}
