/*
 * Created on 2016/05/13 by gliu (516587331@qq.com)
 * 动态配置服务器或者数据的分片设计
 */

#ifndef __USERROUTEDATABAECONF_HPP__
#define __USERROUTEDATABAECONF_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include "Singleton.h"

#define MAX_SHARDNAME_LEN 64
#define MAX_SHARDIP_LEN 24

typedef struct STCELL
{
	unsigned int iCellID;
	char szCellName[MAX_SHARDNAME_LEN];
	char szIP[MAX_SHARDIP_LEN];
	unsigned int  iPort;
	char szDatabase[MAX_SHARDNAME_LEN];
	unsigned int uiStartID;
	unsigned int uiEndID;

	STCELL()
	{
		iCellID = 0;
		memset(szCellName,0,sizeof(szCellName));
		memset(szIP,0,sizeof(szIP));
		iPort = 0;
		memset(szDatabase,0,sizeof(szDatabase));
		uiStartID = 0;
		uiEndID = 0;
	}

	STCELL& operator= (const STCELL& rhs)
	{
		if(this == &rhs)
		{
			return *this;
		}
		iCellID = rhs.iCellID;
		strcpy(szCellName,rhs.szCellName);
		strcpy(szIP,rhs.szIP);
		strcpy(szDatabase,rhs.szDatabase);
		iPort = rhs.iPort;
		uiStartID = rhs.uiStartID;
		uiEndID = rhs.uiEndID;

		return *this;
	}
}stCell;

typedef struct STSHARDING
{
	unsigned int iShardingID;
	char szShardingName[MAX_SHARDNAME_LEN];
	unsigned int iHashValue;
	std::vector<STCELL> vectCells;
	STSHARDING()
	{
		iShardingID = 0;
		memset(szShardingName,0,sizeof(szShardingName));
		iHashValue = 0;
		vectCells.clear();
	}
}stSharding;

typedef struct STGROUP
{
	unsigned int iGroupID;
	char szGroupName[MAX_SHARDNAME_LEN]; // group name
	unsigned int bInsert;						 //	add data or not
	unsigned int uiMinID;				 //	group min id (>=)
	unsigned int uiMaxID;				 //	group max id (<)
	unsigned int ifactor;						 //	取余因子
	std::vector<STSHARDING> vectShardings;
	STGROUP()
	{
		iGroupID = 0;
		memset(szGroupName,0,sizeof(szGroupName));
		bInsert = 0;
		uiMinID = 0;
		uiMaxID = 0;
		ifactor = 0;
		vectShardings.clear();
	}
}stGroup;



class CUserRouteConf
{
public:
	CUserRouteConf();
	~CUserRouteConf();
	bool Init();
	int LoadRouteInfoConfig(const char* pszFilePath);

	int GetCellByUID(const unsigned int uiUserID,STCELL& stCellNode,bool IsInsert);
private:
	std::vector<STGROUP> m_vectGroups;
};

#endif


