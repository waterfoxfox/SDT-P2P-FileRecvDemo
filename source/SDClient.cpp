////////////////////////////////////////////////////////////////////////////////
//  File Name: CSDClient.cpp
//
//  Functions:
//      SDK-纯接收端API演示DEMO.
//
//  History:
//  Date        Modified_By  Reason  Description	
//
////////////////////////////////////////////////////////////////////////////////
#include "SDLog.h"
#include "SDClient.h"


extern FEC_REDUN_TYPE_P2P g_eRedunMethod;
extern UINT		g_unRedunRatio;
extern UINT		g_unFecMinGroupSize;
extern UINT		g_unFecMaxGroupSize;
extern BOOL		g_bEnableAck;
extern UINT		g_unJitterBuffMs;
extern BOOL		g_bSaveRecvData;



CSDClient::CSDClient()
{
	//客户端SDK环境初始化
	SDTerminalP2P_Enviroment_Init("./log", P2P_LOG_OUTPUT_LEVEL_INFO);
	m_pTerminal = SDTerminalP2P_Create();
	m_bClosed = TRUE;
	m_pfRecvH264File = NULL;
}

CSDClient::~CSDClient()
{
	SDTerminalP2P_Delete(&m_pTerminal);
	SDTerminalP2P_Enviroment_Free();
	if (m_pfRecvH264File)
	{
		fclose(m_pfRecvH264File);
		m_pfRecvH264File = NULL;
	}
}


BOOL CSDClient::Start(char* strLocalIp, UINT unLocalPort, CLIENT_USER_TYPE_P2P eUserType)
{
	m_bClosed = FALSE;

	if (g_bSaveRecvData)
	{
		m_pfRecvH264File = fopen("recv.h264", "wb");
		if (m_pfRecvH264File == NULL)
		{
			SDLOG_PRINTF("Test", SD_LOG_LEVEL_WARNING, "Open file for recv bitstream save failed!");
		}
	}

	//设置传输相关参数
	SDTerminalP2P_SetTransParams(m_pTerminal, g_unJitterBuffMs, g_eRedunMethod, g_unRedunRatio, g_unFecMinGroupSize, g_unFecMaxGroupSize, g_bEnableAck);

	//设置回调接口
	SDTerminalP2P_SetRecvRemoteVideoCallback(m_pTerminal, RecvRemoteVideoCallback, this);


	int nRet = SDTerminalP2P_Online(m_pTerminal, strLocalIp, unLocalPort, "", 0, eUserType);
	if (nRet < 0)
	{
		SDLOG_PRINTF("Test", SD_LOG_LEVEL_ERROR, "SDTerminalP2P_Online Failed return:%d!", nRet);
		m_bClosed = TRUE;
		return FALSE;
	}

	return TRUE;
}


void CSDClient::Close()
{
	m_bClosed = TRUE;

	SDTerminalP2P_Offline(m_pTerminal);

	if (m_pfRecvH264File)
	{
		fclose(m_pfRecvH264File);
		m_pfRecvH264File = NULL;
	}
}


// SDK回调接口实现

// 收到对端发来的视频码流
void CSDClient::RecvRemoteVideoCallback(void* pObject, unsigned char* data, unsigned int unLen, unsigned int unPTS, VideoFrameInforP2P* pFrameInfo)
{
	CSDClient* pClient = (CSDClient*)pObject;
	if (pClient->m_pfRecvH264File)
	{
		fwrite(data, sizeof(unsigned char), unLen, pClient->m_pfRecvH264File);
	}

}


