#include "PRADIS_scheme_parser.h"	

#include <algorithm>
#include <string>				
#include <fstream>
#include <iostream>

using namespace std;

sch_parser::sch_parser()
{

}

sch_parser::~sch_parser()
{

}

string trim(string str) {
	size_t left = str.find_first_not_of(" ");
	size_t right = str.find_last_not_of(" ");
	return str.substr(left, right - left + 1);
}

bool isDouble(std::string const& str)
{
	return !str.empty() && str.find_first_not_of("0123456789.e-") == std::string::npos;
}

bool isInteger(std::string const& str)
{
	return !str.empty() && str.find_first_not_of("-0123456789") == std::string::npos;
}

bool isCFDHeader(std::string Name)
{
	return !Name.empty() && Name.find("CFDHeader") != std::string::npos;
}

bool isBoundary(std::string Name)
{
	return !Name.empty() && Name.find("Boundary") != std::string::npos;
}

bool isFluid(std::string Name)
{
	return !Name.empty() && Name.find("Fluid") != std::string::npos;
}

bool isDomain(std::string Name)
{
	return !Name.empty() && Name.find("Domain") != std::string::npos;
}

bool isParameter(std::string Name)
{
	return !Name.empty() && Name.find("Parameter") != std::string::npos;
}

bool isPort(std::string Name)
{
	return !Name.empty() && Name.find("Port") != std::string::npos;
}

void fillPropertyName(sch_parser_component* pc)
{
	if(isCFDHeader(pc->Model))
	{						
		pc->props[0].name = "Problem";
		pc->props[1].name = "Solver";
		pc->props[2].name = "Dynamic";
	}else
	{
		if (isBoundary(pc->Model))
		{
			pc->props[0].name = "Name";
			pc->props[1].name = "Type";
		}else
		{
			if(isDomain(pc->Model))
			{
				pc->props[0].name = "Name";
			}else
			{
				if(isFluid(pc->Model))
				{
					pc->props[0].name = "Name";
					pc->props[1].name = "FluidChoice";
				}else
				{
					if(isParameter(pc->Model))
					{
						pc->props[0].name = "Name";
						pc->props[1].name = "Value";
					}else
					{
						if(isPort(pc->Model))
						{
							pc->props[0].name = "Num";
							pc->props[1].name = "Type";
						}
					}
				}
			}
		}
	}
}

int sch_parser::read(std::string line)
{
	sch_parser_component* pc = new sch_parser_component();
	
	pc->Model = line.substr(1, line.find(' '));	  // MODEL
	line = line.substr(line.find(' ') + 1);


	pc->Name = line.substr(0, line.find(' '));		  // NAME
	if (pc->Name == "*") pc->Name = "";	   // for PRADIS_Ground
	line = line.substr(line.find(' ') + 1);


	pc->Active = line.substr(0, line.find(' '));	 // ACTIVE
	line = line.substr(line.find('"') + 1);

	int count = 0;
	while (line.length() > 3 && line.find('"') != string::npos)			// PROPERTIES
	{
		sch_parser_property* pp = new sch_parser_property();

		string buffer = line.substr(0, line.find('"'));

		pp->value = buffer;															// write data to VALUE

		transform(buffer.begin(), buffer.end(), buffer.begin(),
			[](unsigned char c) { return tolower(c); });
		if (buffer == "true" || buffer == "false") {
			pp->type = Boolean;
		}
		else {

			if (isInteger(buffer)) {
				pp->type = Integer;
			}
			else {
				if (isDouble(buffer)) {
					pp->type = Double;
				}
				else {
					pp->type = String;
				}
			}
		}
		(pc->props).push_back(*pp);
		line = line.substr(line.find('"') + 1);  // skip "_0_" between properties
		line = line.substr(line.find('"') + 1);
		count++;
	}
	fillPropertyName(pc);
	listComp.push_back(*pc);
	return 0;
}  

int sch_parser::parse(const std::string& file_name)
{
	string s; // сюда будем класть считанные строки
	ifstream file(file_name); // файл из которого читаем (для линукс путь будет выглядеть по другому)
	if (file)
	{
		do
		{
			getline(file, s);
			s = trim(s);
		} while (s != "<Components>");
		getline(file, s);

		do
		{
			s = trim(s);
			read(s);
			getline(file, s);

		} while (s != "</Components>");


		file.close(); // обязательно закрываем файл что бы не повредить его

		return 1;
	}
	else return 0;
}

void sch_parser::getComponentList(std::vector<sch_parser_component>& list)
{
	list = listComp;
}


////////////////////////////////////////////////////////////


sch_parser_component::sch_parser_component()
{
	Model = "";
	Name = "";
	Active = "";
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
	if (atoi(this->Active.c_str()) == 1) return true;
	else return false;  
	//return this->Active;
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