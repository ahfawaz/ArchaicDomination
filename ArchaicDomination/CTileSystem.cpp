#include "TileSystem\CTileSystem.h"
#include "BattlePlay.h"


bool CTileSystem::Load(const char* p, vector<vector<CTile*>>& v)
{
	TiXmlDocument doc(p);

	
	//Error Checking
	assert( doc.LoadFile() != false && "CTileSystem:: Could not load an XML file." && doc.ErrorDesc() );
	if(doc.LoadFile())
	{

		TiXmlElement* root = doc.RootElement();



		//Load first element (Grid Info)
		TiXmlElement* element = root->FirstChildElement();
		string elementName = element->Value();

		int gridWidth;
		int gridHeight;
		int tileWidth;
		int tileHeight;
		if(elementName == "Grid")
		{
			element->Attribute("GridWidth",&gridWidth);
			element->Attribute("GridHeight",&gridHeight);
			element->Attribute("TileWidth",&tileWidth);
			element->Attribute("TileHeight",&tileHeight);


		}



		//Load Loop
		element = element->FirstChildElement();
		elementName = element->Value();


		for(int i = 0; i < gridHeight; i++)
		{
			vector<CTile*> vec;
			v.push_back(vec);
			
			for(int j = 0; j < gridWidth; j++)
			{

				if(elementName == "Tile")
				{
					int type, locX, locY, posX, posY;
					const char* ID;
					const char* Obj;

					ID = element->Attribute("TileID");
					element->Attribute("Type",&type);
					Obj = element->Attribute("Object");
					element->Attribute("LocX",&locX);
					element->Attribute("LocY",&locY);
					element->Attribute("PosX",&posX);
					element->Attribute("PosY",&posY);
					
					

				
					CTile* temp = new CTile(posX,posY,tileWidth,tileHeight,ID,Obj,locX,locY,type);


					v[i].push_back(temp);
					if(element->NextSiblingElement() == NULL)
						break;
					else
					{
						element = element->NextSiblingElement();
						elementName = element->Value();
					}

					
					
				}
				
				
				
			}
	
			
		}



		CBattleGrid* bg = new CBattleGrid( v);
		
		

		CBattlePlay::GetInstance()->SetGrid(bg);

	}
	else return false;

	
	
	return true;
}


//CTileSystem::~CTileSystem()
//{
//	delete bg;
//	bg = nullptr;
//
//}
//CTileSystem::CTileSystem()
//{
//	bg = new CBattleGrid();
//}