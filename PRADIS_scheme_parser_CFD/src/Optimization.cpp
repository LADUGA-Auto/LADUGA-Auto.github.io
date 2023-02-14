#include "Optimization.h"
#include "PRADIS_scheme_parser.h"

#include <string>
#include <vector>


using namespace std;

Variable::Variable()
{
	
}
Variable::Variable(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}


Data::Data()
{
	
}
Data::Data(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}


Loadcase::Loadcase()
{
		
}
Loadcase::Loadcase(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}


Optimization::Optimization()
{
	
}
Optimization::Optimization(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}





int parseOptimization(std::vector<sch_parser_component> componentList, OPTScheme& scheme)
{
	std::vector<sch_parser_property> propertyList;
	std::string componentName = "", componentModel = "", sValue = ""; // creating data buffers
	bool active = 0;


	for (size_t i = 0; i < componentList.size(); i++)
	{
		componentList[i].getModel(componentModel);		                                           // CoSimulation.Port
		componentList[i].getName(componentName);												   // Port1	
		active = componentList[i].isActive();													   // 1 or 0
		componentList[i].getPropertyList(propertyList); // return propertyList for component	   // 0 "" 0 "" 0 "" 0



		if (isVariable(componentModel))
		{
			Variable newVar = Variable(&componentList[i]);
			scheme.variableList.push_back(newVar);
		}
		else if (isData(componentModel))
		{
			Data newData = Data(&componentList[i]);
			scheme.dataList.push_back(newData);
		}
		else if (isLoadcase(componentModel))
		{
			Loadcase newLoadcase = Loadcase(&componentList[i]);
			scheme.loadcaseList.push_back(newLoadcase);
		}
		else if (isOptimization(componentModel))
		{
			Optimization newOpt = Optimization(&componentList[i]);
			scheme.optimizationList.push_back(newOpt);
		}
	}
	return 0;
}






