#include "schParser.h"
#include "schematic.h"
#include <string>
sch_parser::sch_parser()
{

}

sch_parser::~sch_parser()
{

}


int sch_parser::read(const std::string& fileName)
{
	scheme = new Schematic(fileName);
	scheme->load();
	return 0;
}



int sch_parser::parse()
{
	Component* comp = scheme->DocComps.first();

		while (comp) 
		{
			sch_parser_component* pc = new sch_parser_component();
			
			pc->Active = comp->isActive;

			pc->Model = std::string (comp->Model);

			pc->Name = std::string (comp->Name);

			Property* proper = comp->Props.first();
			while (proper) 
			{

				sch_parser_property* pp = new sch_parser_property();

				pp->value = std::string (proper->Value); 
				if ((proper->Value).lower() == "true" || (proper->Value).lower() == "false") 
				{
					pp->type = Boolean;
				}
				else 
				{
					
					if (proper->Value.toInt() || proper->Value == "0") 
					{
						pp->type = Integer;
					}
					else 
					{
						if (proper->Value.toDouble() || proper->Value == "0.0" || proper->Value == "0.") 
						{
						  pp->type = Double;
						}
						else 
						{
							pp->type = String;
						}
					}
				}
				(pc->props).push_back(*pp);
				proper = comp->Props.next();
			}
			listComp.push_back(*pc);
			comp = scheme->DocComps.next();
		}
	return 0;
}

void sch_parser::getComponentList(std::vector<sch_parser_component>& list)
{
	list = listComp;
}


////////////////////////////////////////////////////////////

