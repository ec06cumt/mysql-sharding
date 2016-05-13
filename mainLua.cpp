/*
 * mainTree.cpp
 *
 *  Created on: 2016��5��6��
 *      Author: liug
 */

/*
*
* tree create and implment
*
*/

#ifndef __BINARY_TREE_H_
#define __BINARY_TREE_H_

#include "luaWrapper.hpp"
#include <stdio.h>
#include <sys/time.h>

#include "UserRouteConf.hpp"
#include "Singleton.h"

typedef luacpp::luatable luatable;
typedef luacpp::luaObject luaObject;
#define any_cast luacpp::any_cast

void test_lua_return_value(lua_State *L)
{
	printf("-----lua return int value to C++-----\n");
	try{
		int iSum = luacpp::call<int>(L,"add",10,20);
		printf("sum:%d\n",iSum);
	}catch(std::string &err)
	{
		printf("%s\n",err.c_str());
	}
}

void test_pass_luatable(lua_State *L)
{
	printf("-----pass luatable to lua-----\n");
	try{
		luatable lt;
		for(int i = 0; i < 20; ++i)
			lt.push_back(i);
		luacpp::call<void>(L,"test3",lt);

	}catch(std::string &err)
	{
		printf("%s\n",err.c_str());
	}
}

void test_get_array(lua_State *L)
{
	printf("-----get array from lua-----\n");
	try{
		luatable lt = luacpp::call<luatable>(L,"get_lua_array");
		for(int i = 0 ;i < (int)lt.size(); ++i)
			printf("%s\n",any_cast<std::string>(lt[i]).c_str());
	}catch(std::string &err)
	{
		printf("%s\n",err.c_str());
	}
}


void test_get_InsertIPDatabase(lua_State *L,unsigned int uiUID)
{
	// printf("-----get InsertIP from lua-----\n");
	try{
		luatable lt = luacpp::call<luatable>(L,"GetInsertIPAndDBName",uiUID);
//		for(int i = 0 ;i < (int)lt.size(); ++i)
//			printf("%s\n",any_cast<std::string>(lt[i]).c_str());
	}catch(std::string &err)
	{
		printf("%s\n",err.c_str());
	}
}

struct timeval g_timeStart;
struct timeval g_timeEnd;
struct timeval g_diff;

void test_lua_getIPAndDatabase()
{
	luacpp::luaWrapper lw;
	for(unsigned int i = 200000;i<210000;++i)
	{
		lw.dofile("../lua/getRouteDatabase.lua");
		test_get_InsertIPDatabase(lw,i);
	}
}

int main(int argc,char** argv)
{
	gettimeofday(&g_timeStart,0);
	int bRet = Singleton<CUserRouteConf>::GetInstance().Init();
	bRet = Singleton<CUserRouteConf>::GetInstance().LoadRouteInfoConfig("../conf/GetRouteIPAndDatabaseName.xml");
	if(bRet)
	{
		printf("Failed to load GetRouteIPAndDatabaseName.xml config %d\n",bRet);
		return -1;
	}
	unsigned int j = 200000;
	for(unsigned int i = 0;i< 200000;++i)
	{
		STCELL stCellNode;
		bRet = Singleton<CUserRouteConf>::GetInstance().GetCellByUID(i,stCellNode,0);
		if(bRet)
		{
			printf("*******not found %u********\n",i);
		}
		else
		{
			if(i > 99995 && i < 100005)
			{
				printf("userid:%u,id:%u,name:%s,ip:%s,port:%u,database:%s,startid:%u,endid:%u\n"
							,i
							,stCellNode.iCellID,stCellNode.szCellName,stCellNode.szIP,stCellNode.iPort
							,stCellNode.szDatabase,stCellNode.uiStartID,stCellNode.uiEndID);
			}
		}

		if(j< 200020)
		{
			STCELL stCellNode2;
			bRet = Singleton<CUserRouteConf>::GetInstance().GetCellByUID(j,stCellNode2,1);
			if(bRet)
			{
				printf("*******not found %u********\n",j);
			}
			else
			{
				printf("userid:%u,id:%u,name:%s,ip:%s,port:%u,database:%s,startid:%u,endid:%u\n"
							,j
							,stCellNode2.iCellID,stCellNode2.szCellName,stCellNode2.szIP,stCellNode2.iPort
							,stCellNode2.szDatabase,stCellNode2.uiStartID,stCellNode2.uiEndID);
			}
			++j;
		}
	}


	//test_lua_getIPAndDatabase();

	gettimeofday(&g_timeEnd,0);
	float time_use = ((g_timeEnd.tv_sec-g_timeStart.tv_sec)*1000000+(g_timeEnd.tv_usec-g_timeStart.tv_usec));
	printf("cost time_use:%f,s:%f\n",time_use,time_use/1000000);

//	lw.reloadfile("../lua/addlua.lua");


	/*
	test_lua_return_value(lw);
	test_lua_return_value(lw);

	test_pass_luatable(lw);
	test_get_array(lw);

	bool bSet = false;
	while(true)
	{
		if(!bSet)
		{
			if(argc == 2)
			{
				printf("reload:%s\n",argv[1]);
				bSet = true;
			}
		}
		usleep(1000);
	}
	*/
	return 0;
}

#endif





