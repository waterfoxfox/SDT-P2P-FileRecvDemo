////////////////////////////////////////////////////////////////////////////////
//  File Name: CSDClient.h
//
//  Functions:
//      客户端对象类.
//
//  History:
//  Date        Modified_By  Reason  Description	
//
////////////////////////////////////////////////////////////////////////////////

#if !defined(_SDCLIENT_H)
#define _SDCLIENT_H

#include "SDCommon.h"
#include "SDTerminalP2PSdk.h"


class CSDClient
{
public:
	CSDClient();
	virtual ~CSDClient();

public:
	BOOL Start(char* strRemoteIp, UINT unRemotePort, CLIENT_USER_TYPE_P2P eUserType);
	void Close();
	
private:
	// SDK回调接口实现
	static void RecvRemoteVideoCallback(void* pObject, unsigned char* data, unsigned int unLen, unsigned int unPTS, VideoFrameInforP2P* pFrameInfo);

private:
	void*				 m_pTerminal;

	BOOL				 m_bClosed;
	FILE*				 m_pfRecvH264File;

};

#endif // !defined(_SDCLIENT_H)
