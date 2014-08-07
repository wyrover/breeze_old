#include "Application.h"
#include <log4z/log4z.h>
#include <zsummerX/FrameTcpSessionManager.h>
#include <zsummerX/FrameMessageDispatch.h>
#include "core/GlobalFacade.h"
#include <ServerConfig.h>
#include <MongoManager.h>
#include "core/NetManager.h"
#include "logic/LoginHandler.h"
using namespace zsummer::log4z;

Appliction::Appliction()
{
	
}

Appliction::~Appliction()
{
}

Appliction & Appliction::getRef()
{
	static Appliction g_facade;
	return g_facade;
}


bool Appliction::Init(std::string filename, unsigned int index)
{
	bool ret = false;


	ret = GlobalFacade::getRef().getServerConfig().Parse(filename, DBAgentNode, index);
	if (!ret)
	{
		LOGE("getServerConfig failed." );
		return ret;
	}
	ret = GlobalFacade::getRef().getMongoManger().ConnectMongo(GlobalFacade::getRef().getMongoManger().getInfoMongo(),GlobalFacade::getRef().getServerConfig().getInfoMongoDB());
	if (!ret)
	{
		LOGE("ConnectInfo mongo failed.");
		return ret;
	}
	ret = CTcpSessionManager::getRef().Start();
	if (!ret)
	{
		LOGE("CTcpSessionManager Start false.");
		return ret;
	}
	ret = GlobalFacade::getRef().getNetManger().Start();
	if (!ret)
	{
		LOGE("NetManager Start false.");
		return ret;
	}

	std::vector<CBaseHandler*> handlers;
	handlers.push_back(new CLoginHandler());
	for (auto ptr:handlers)
	{
		if (!ptr->Init())
		{
			LOGW("Appliction Init handler false");
			return false;
		}
	}
	
	LOGI("Appliction Init success.");
	return true;
}

void Appliction::RunPump()
{
	return CTcpSessionManager::getRef().Run();
}

void Appliction::Stop()
{
	CTcpSessionManager::getRef().Stop();
}