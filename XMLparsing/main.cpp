#include <iostream>
#include "Monster.h"
#include "tinyxml2.h"

using namespace tinyxml2;

bool SaveToXML(const char* filename, const std::vector<Monster>& monsters)
{
	XMLDocument doc;

	XMLDeclaration* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="EUC-KR")");
	doc.LinkEndChild(decl);

	XMLElement* pRootNode = doc.NewElement("monsters");

	for (auto monster : monsters)
	{
		XMLElement* pMonsterNode = doc.NewElement("monster");
		pMonsterNode->SetAttribute("name", monster.GetName().c_str());

		XMLElement* pStatusNode = doc.NewElement("status");
		const Status& status = monster.GetStatus();
		pStatusNode->SetAttribute("level", status.mLevel);
		pStatusNode->SetAttribute("hp", status.mHP);
		pStatusNode->SetAttribute("mp", status.mMP);
		pMonsterNode->LinkEndChild(pStatusNode);

		XMLElement* pItemsNode = doc.NewElement("items");
		for (auto item : monster.GetDropItems())
		{
			XMLElement* pItemNode = doc.NewElement("item");
			pItemNode->SetAttribute("name", item.mName.c_str());
			pItemNode->SetAttribute("gold", item.mGold);
			pItemNode->LinkEndChild(pItemsNode);

		}
		pMonsterNode->LinkEndChild(pItemsNode);

		pRootNode->LinkEndChild(pMonsterNode);
	}

	doc.LinkEndChild(pRootNode);

	return 	doc.SaveFile(filename) == XML_SUCCESS;
}

bool LoadFromXML(const char* filename, std::vector<Monster>& monsters)
{
	XMLDocument doc;

	if (doc.LoadFile(filename) != XML_SUCCESS)
	{
		return false;
	}

	doc.FirstChildElement("monsters");
	XMLElement* pRootNode = doc.FirstChildElement("monsters");
	for (XMLElement* pMonsterNode = pRootNode->FirstChildElement() ; 
		pMonsterNode != nullptr ; pMonsterNode = pMonsterNode->NextSiblingElement())
	{
		Monster monster;

		monster.SetName(pMonsterNode->Attribute("name"));
		auto pStatusNode = pMonsterNode->FirstChildElement("status");

		Status status;
		status.mLevel = pStatusNode->IntAttribute("level");
		status.mHP = pStatusNode->IntAttribute("hp");
		status.mMP = pStatusNode->IntAttribute("mp");
		monster.SetStatus(status);

		auto pItemsNode = pMonsterNode->FirstChildElement("items");
		for (auto pItemNode = pItemsNode->FirstChildElement(); 
			pItemNode != nullptr ; pItemNode = pItemNode->NextSiblingElement())
		{
			Item item;
			item.mName = pItemNode->Attribute("name");
			item.mGold = pItemNode->IntAttribute("gold");
			monster.AddDropItem(item);
		}

		monsters.push_back(monster);
	}

	return true;
}

int main()
{
	std::vector<Monster> monsters;

	Monster monster;
	monster.SetName("??????");
	monster.SetStatus(Status{ 1, 1, 1 });
	monster.AddDropItem(Item{ "?????? ????", 1 });
	monsters.push_back(monster);

	monster.SetName("????????");
	monster.SetStatus(Status{ 5, 5, 5 });
	monster.GetDropItems().clear();
	monster.AddDropItem(Item{ "????", 2 });
	monster.AddDropItem(Item{ "????", 5 });
	monsters.push_back(monster);

	monster.SetName("????");
	monster.SetStatus(Status{ 10, 10, 10 });
	monster.GetDropItems().clear();
	monster.AddDropItem(Item{ "????", 10 });
	monster.AddDropItem(Item{ "????", 10 });
	monsters.push_back(monster);

	SaveToXML("Data/monsters.xml", monsters);

	monsters.clear();
	LoadFromXML("Data/monsters.xml", monsters);
}