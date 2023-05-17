////////////////////////////////////////////////////////////////////////////////
//  File Name: main.cpp
//
//  Functions:
//      SDK����DEMO.
//
//  History:
//  Date        Modified_By  Reason  Description	
//
////////////////////////////////////////////////////////////////////////////////
#include "SDCommon.h"
#include "SDLog.h"
#include "SDIniFile.h"
#include "SDClient.h"
#include "SDConsoleIFace.h"
#include "SDTerminalP2PSdk.h"

#if defined(WIN32) && defined(_DEBUG_)
#include <afx.h>
#endif

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <signal.h>
#endif

#define SECTION_CONFIG					"Config"
#define KEY_NAME_LOCAL_IP				"LocalIp"			//���ط���ʹ�õ�IP
#define KEY_NAME_LOCAL_PORT				"LocalPort"			//���ؽ������ݵĶ˿�
#define KEY_NAME_FEC_REDUN_METHOD		"FecRedunMethod"	//FEC���෽��
#define KEY_NAME_FEC_REDUN_RATIO		"FecRedunRatio"		//FEC����ȱ���
#define KEY_NAME_FEC_MIN_GROUP_SIZE		"FecMinGroupSize"	//FEC Min Group size
#define KEY_NAME_FEC_MAX_GROUP_SIZE		"FecMaxGroupSize"	//FEC Max Group size
#define KEY_NAME_FEC_ENABLE_ACK			"FecEnableAck"		//�Ƿ�֧��������ACK����
#define KEY_NAME_FEC_JITTER_BUFF_MS		"FecJitterBuffMs"	//���ջ���JB��ʱ�䣬��λms

#define KEY_NAME_RECV_FILE_SAVE			"SaveRecvData"		//�Ƿ񱣴��յ������ݵ��ļ�


CSDClient	g_Client;


FEC_REDUN_TYPE_P2P g_eRedunMethod		= P2P_FEC_AUTO_REDUN_TYPE;
UINT		g_unRedunRatio				= 30;
UINT		g_unFecMinGroupSize			= 16;
UINT		g_unFecMaxGroupSize			= 64;
BOOL		g_bEnableAck				= FALSE;
UINT		g_unJitterBuffMs			= 200;
BOOL		g_bSaveRecvData				= TRUE;

//������
int main(int argc, char *argv[])
{
	int nRet = 0;

	//��ʼ��DEMO��־ģ��
	char strIniFileName[MAX_PATH];
	GetSameExeFile(strIniFileName, ".ini");
	SDLOG_INIT("./log", SD_LOG_LEVEL_INFO, strIniFileName);


	//��ȡ����
	CSDIniFile *pIniFile = new CSDIniFile;
	pIniFile->ReadIniFile(strIniFileName);

	//����IP
	char strLocalIp[64];
	memset(strLocalIp, 0x0, sizeof(strLocalIp));
	pIniFile->SDGetProfileString(SECTION_CONFIG, KEY_NAME_LOCAL_IP, strLocalIp, 64);

	//�û�����
	CLIENT_USER_TYPE_P2P eUserType = CLIENT_USER_TYPE_AV_RECV_ONLY;

	//����PORT
	UINT unLocalPort = (UINT)pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_NAME_LOCAL_PORT, 4000);


	//NACK����
	UINT unEnableNack = (UINT)pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_NAME_FEC_ENABLE_ACK, FALSE);
	if (unEnableNack != 0)
	{
		g_bEnableAck = TRUE;
	}
	else
	{
		g_bEnableAck = FALSE;
	}

	//����JitterBuff����ʱ�䣬��λms
	g_unJitterBuffMs = (UINT)pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_NAME_FEC_JITTER_BUFF_MS, 150);


	//�Ƿ񱣴��յ����ļ�
	UINT unSaveRecvData = (UINT)pIniFile->SDGetProfileInt(SECTION_CONFIG, KEY_NAME_RECV_FILE_SAVE, FALSE);
	if (unSaveRecvData != 0)
	{
		g_bSaveRecvData = TRUE;
	}
	else
	{
		g_bSaveRecvData = FALSE;
	}


	SDLOG_PRINTF("CAVProcess", SD_LOG_LEVEL_INFO, "AckEnable:%s JittBuff:%d!", g_bEnableAck == TRUE ? "Y":"N", g_unJitterBuffMs);

	delete pIniFile;
	

	SDLOG_PRINTF("Test", SD_LOG_LEVEL_INFO, "LocalIp:%s LocalPort:%d!", strLocalIp, unLocalPort);


	//�������Է���
	if (!g_Client.Start(strLocalIp, unLocalPort, eUserType))
	{
		SDLOG_PRINTF("Test", SD_LOG_LEVEL_ERROR, "Test start fail...");
		SDLOG_CLOSE();
		return FALSE;
	}
	else
	{
		SDLOG_PRINTF("Test", SD_LOG_LEVEL_INFO, "sdk test start success...");
		SDLOG_PRINTF("Test", SD_LOG_LEVEL_INFO, "input exit will end test process!");
	}

	//ѭ��
	CSDConsleIFace::RunCommandLine("sdk_test");

	g_Client.Close();
	

	SDLOG_PRINTF("Test", SD_LOG_LEVEL_INFO, "sdk test over success...");
	SDLOG_CLOSE();

	return TRUE;
}

