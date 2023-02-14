#include "parserComponent.h"


sch_parser_component::sch_parser_component()
{
	Model = "";
	Name = "";
	Active = false;
}

sch_parser_component::~sch_parser_component()
{

}


void sch_parser_component::getModel(std::string& modelName)
{
	modelName = this->Model;
}

void sch_parser_component::getName(std::string& componentName)
{
	componentName = this->Name;
}

bool sch_parser_component::isActive()
{
	return this->Active;
}

void sch_parser_component::getPropertyList(std::vector<sch_parser_property>& list)
{
	list = props;
}



/////////////////////////////////////////////////////////////////////


sch_parser_property::sch_parser_property()
{
	value = "";
	type = String;
}

sch_parser_property::~sch_parser_property()
{
  // in header file
}


void sch_parser_property::getType(int& type)
{
	type = this->type;
}

bool sch_parser_property::getStringValue(std::string& value)
{
	if (this->type == String) 
	{
		value = this->value;
		return true;
	}
	return false;
}
bool sch_parser_property::getIntValue(int& value)
{
	if (this->type == Integer) 
	{
		value = atoi((this->value).c_str());
		return true;
	}
	return false;
}

bool sch_parser_property::getBoolValue(bool& value)
{
	if (this->type == Boolean) 
	{
		if (this->value == "false") value = false;
		else value = true;
		return true;
	}
	return false;
}
bool sch_parser_property::getDoubleValue(double& value)
{
	if (this->type == Double) 
	{
		value = strtod((this->value).c_str(), 0); 
		return true;
	}
	return false;
}