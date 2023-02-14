#include "CoSimulation.h"
#include "PRADIS_scheme_parser.h"
#include <string>
#include <vector>

using namespace std;

Port::Port()
{
	
}
Port::Port(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}


FluidDomain::FluidDomain()
{

}
FluidDomain::FluidDomain(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}

SolidDomain::SolidDomain()
{
	
}
SolidDomain::SolidDomain(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}




Solver::Solver()
{
	
}
Solver::Solver(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}


Task::Task()
{
	
}
Task::Task(sch_parser_component* pc)
{
	this->Model = pc->Model;
	this->Name = pc->Name;
	this->Active = pc->Active;
	pc->getPropertyList(this->Properties);
}

int parseCoSimulation(std::vector<sch_parser_component>& componentList, CFDScheme& scheme)
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



		if (isCFDPort(componentModel))
		{
			Port newPort = Port(&componentList[i]);
			scheme.portList.push_back(newPort);
		}
		else if (isFluidDomain(componentModel))
		{
			FluidDomain newFluidDomain = FluidDomain(&componentList[i]);
			scheme.fluidDomainList.push_back(newFluidDomain);
		}
		else if (isSolidDomain(componentModel))
		{
			SolidDomain newSolidDomain = SolidDomain(&componentList[i]);
			scheme.solidDomainList.push_back(newSolidDomain);
		}
		else if (isSolver(componentModel))
		{
			Solver newSolver = Solver(&componentList[i]);
			scheme.solver = newSolver;
		}
		else if (isTask(componentModel))
		{
			Task newTask = Task(&componentList[i]);
			scheme.task = newTask;
		}
	}
	return 0;
}