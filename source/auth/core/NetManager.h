
/*
* breeze License
* Copyright (C) 2014 YaweiZhang <yawei_zhang@foxmail.com>.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include "../core/GlobalFacade.h"
#include <zsummerX/FrameMessageDispatch.h>
#include <zsummerX/FrameTcpSessionManager.h>
#include <ProtoDefine.h>
/*
* NetManager
*/

namespace mongo
{
	class DBClientConnection;
};

class CNetManager
{
public:
	CNetManager();
	//连接所有认证服务和中央服务
	bool Start();

	//
	inline void SendOrgDataToCenter(const char * orgData, unsigned int orgDataLen)
	{
		if (m_onlineCenter.empty())
		{
			LOGW("not found any center. protocol send lost");
			return;
		}
		const ServerAuthSession & sas = m_onlineCenter.at(0);
		CTcpSessionManager::getRef().SendOrgSessionData(sas.aID, sas.sID, orgData, orgDataLen);
	}

	void event_OnSessionHeartbeat(AccepterID aID, SessionID sID);
	void msg_OnDirectServerPulse(AccepterID aID, SessionID sID, ProtocolID pID, ReadStreamPack & rs);

	//register 
	void event_OnSessionEstablished(AccepterID, SessionID);
	void event_OnSessionDisconnect(AccepterID, SessionID);
	void msg_SessionServerAuth(AccepterID aID, SessionID sID, ProtocolID pID, ReadStreamPack & rs);

private:
	tagAcceptorConfigTraits m_configListen; //保存监听配置

	std::vector<ServerAuthSession> m_onlineCenter;


};




































#endif
