/*
 * Created on 2016/05/13 by gliu
 *
 *
 */

#include "UserRouteConf.hpp"
#include "pugixml.hpp"
#include "StringUtility.hpp"

//using namespace pugi;

CUserRouteConf::CUserRouteConf()
{
	Init();
}

CUserRouteConf::~CUserRouteConf()
{

}
bool CUserRouteConf::Init()
{
	m_vectGroups.clear();
	return true;
}

int CUserRouteConf::LoadRouteInfoConfig(const char* pszFilePath)
{
	if(!pszFilePath)
	{
		printf("config file is not exist\n");
		return -1;
	}

	pugi::xml_document oXmlDoc;
	pugi::xml_parse_result oResult = oXmlDoc.load_file(pszFilePath);
	if(!oResult)
	{
		printf("Failed to open xml config file : %s\n", pszFilePath);
		return -2;
	}
	pugi::xml_node oInfoNodes = oXmlDoc.child("config");

	for(pugi::xml_node oOneInfoNode=oInfoNodes.child("Group"); oOneInfoNode; oOneInfoNode=oOneInfoNode.next_sibling())
	{
		STGROUP stNodeGroup;
		stNodeGroup.iGroupID = oOneInfoNode.attribute("gid").as_uint();
		SAFE_STRCPY(stNodeGroup.szGroupName,oOneInfoNode.attribute("name").as_string(),sizeof(stNodeGroup.szGroupName)-1);
		stNodeGroup.bInsert = oOneInfoNode.attribute("bInsert").as_uint();
		stNodeGroup.uiMinID = oOneInfoNode.attribute("minid").as_uint();
		stNodeGroup.uiMaxID = oOneInfoNode.attribute("maxid").as_uint();
		stNodeGroup.ifactor = oOneInfoNode.attribute("factor").as_uint();

		for(pugi::xml_node oOneShardNode = oOneInfoNode.child("Sharding");oOneShardNode;oOneShardNode = oOneShardNode.next_sibling())
		{
			STSHARDING stNodeSharding;
			stNodeSharding.iShardingID = oOneShardNode.attribute("sid").as_uint();
			SAFE_STRCPY(stNodeSharding.szShardingName,oOneShardNode.attribute("name").as_string()
					,sizeof(stNodeSharding.szShardingName)-1);
			stNodeSharding.iHashValue = oOneShardNode.attribute("hashval").as_uint();

			for(pugi::xml_node oOneCellNode = oOneShardNode.child("Cell");oOneCellNode;oOneCellNode = oOneCellNode.next_sibling())
			{
				STCELL stNodeCell;
				stNodeCell.iCellID = oOneCellNode.attribute("cid").as_uint();
				SAFE_STRCPY(stNodeCell.szCellName,oOneCellNode.attribute("name").value(),sizeof(stNodeCell.szCellName)-1);
				SAFE_STRCPY(stNodeCell.szIP,oOneCellNode.attribute("ip").value(),sizeof(stNodeCell.szIP)-1);
				stNodeCell.iPort = oOneCellNode.attribute("port").as_uint();
				SAFE_STRCPY(stNodeCell.szDatabase,oOneCellNode.attribute("database").value(),sizeof(stNodeCell.szDatabase)-1);
				stNodeCell.uiStartID = oOneCellNode.attribute("startid").as_uint();
				stNodeCell.uiEndID = oOneCellNode.attribute("endid").as_uint();

				stNodeSharding.vectCells.push_back(stNodeCell);
			}
			stNodeGroup.vectShardings.push_back(stNodeSharding);
		}

		m_vectGroups.push_back(stNodeGroup);
	}
	return 0;
}

int CUserRouteConf::GetCellByUID(const unsigned int uiUserID,STCELL& stCellNode,bool IsInsert)
{
	if(m_vectGroups.size() == 0)
	{
		return -1;
	}
	std::vector<STGROUP>::iterator it = m_vectGroups.begin();

	// find groups
	for(;it != m_vectGroups.end();++it)
	{
		if(uiUserID >= (*it).uiMinID && uiUserID < (*it).uiMaxID && (IsInsert?((*it).bInsert):true))
		{
			unsigned int iHashValue = uiUserID % ((*it).ifactor);
			std::vector<STSHARDING>::iterator itShard = (*it).vectShardings.begin();
			for(;itShard != (*it).vectShardings.end();++itShard)
			{
				if((*itShard).iHashValue == iHashValue)
				{
					std::vector<STCELL>::iterator itCellNode = (*itShard).vectCells.begin();
					for(;itCellNode != (*itShard).vectCells.end();++itCellNode)
					{
						if(uiUserID >= (*itCellNode).uiStartID && uiUserID < (*itCellNode).uiEndID)
						{
							//printf("uiUserID:%u,%u,%u\n",uiUserID,(*itCellNode).uiStartID,(*itCellNode).uiEndID);
							stCellNode = *itCellNode;
							return 0;
						}
					}
				}
			}
		}
	}

	return -2;
}
