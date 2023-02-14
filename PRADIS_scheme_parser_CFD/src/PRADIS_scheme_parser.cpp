#include "PRADIS_scheme_parser.h"	
#include "CoSimulation.h"
#include "Optimization.h"

#include <algorithm>
#include <string>				
#include <fstream>
#include <iostream>

using namespace std;

PRADIS_scheme_parser::PRADIS_scheme_parser()
{

}

//bool PRADIS_scheme_parser::load(std::vector<sch_parser_component>& list, std::string path)
//{
//	try {
//		if (this->parser.read(path)) // fill componentList and propertyList
//		{
//			this->parser.getComponentList(list); 
//			return true;
//		}
//		return false;
//	}
//	catch (char const* e)
//	{
//		cout << "FAIL: " << e << endl;
//		return false;
//	}
//	catch (const exception& e) {
//		cout << e.what() << endl;
//		cout << "Ошибка открытия файла!\n";
//	}
//	catch (...)
//	{
//		cout << "ERROR" << endl;
//		return false;
//	}
//	
//}

bool PRADIS_scheme_parser::load(CFDScheme& scheme, istream& stream)
{
	try {
		if (this->parser.read(stream)) // fill componentList and propertyList
		{
			std::vector<sch_parser_component> componentList;
			this->parser.getComponentList(componentList);

			parseCoSimulation(componentList, scheme);

			return true;
		}
		return false;
	}
	catch (char const* msg)
	{
		cout << "FAIL: " << msg << endl;
		return false;
	}
	catch (const exception& e) {
		cout << e.what() << endl;
		cout << "Error open file!\n";
	}
	catch (...)
	{
		cout << "ERROR" << endl;
		return false;
	}
}

bool PRADIS_scheme_parser::load(OPTScheme& scheme, istream& stream)
{
	try {
		if (this->parser.read(stream)) // fill componentList and propertyList
		{
			std::vector<sch_parser_component> componentList;
			this->parser.getComponentList(componentList);
			parseOptimization(componentList, scheme);

			return true;
		}
		return false;
	}
	catch (char const* msg)
	{
		cout << "FAIL: " << msg << endl;
		return false;
	}
	catch (const exception& e) {
		cout << e.what() << endl;
		cout << "Error open file!\n";
		return false;
	}
	catch (...)
	{
		cout << "ERROR" << endl;
		return false;
	}
}

// helper function for write result parsing to file
int writeOutFile(std::string outputFile, std::vector<sch_parser_component> componentList)
{
	std::vector<sch_parser_property> propertyList;
	ofstream file(outputFile, 'w');
	if (file) // writing components info to .txt file
	{


		file << "[ Module ]      |      [ Name ]       |      [ Active ]       |      [ Properties ]\n";
		for (size_t i = 0; i < componentList.size(); i++)
		{
			std::string componentName = "", componentModel = "", sValue = ""; // creating data buffers
			int iValue = 0;
			bool bValue = 0, active = 0;
			double dValue = 0;

			componentList[i].getModel(componentModel);		                                           // CoSimulation.Port
			componentList[i].getName(componentName);												   // Port1	
			active = componentList[i].isActive();													   // 1 or 0
			componentList[i].getPropertyList(propertyList); // return propertyList for component	   // 0 "" 0 "" 0 "" 0

			file << "[ " << componentModel.c_str() << " ] | [ " << componentName.c_str() << " ] | ";

			if (active) file << "[ true ] | ";// active component or not?
			if (!active) file << "[ false ] | ";


			componentList[i].getPropertyList(propertyList); // return propertyList for component

			for (size_t j = 0; j < propertyList.size(); j++)// check type value of property
			{
				if (propertyList[j].getStringValue(sValue)) file << "[ " << propertyList[j].name.c_str() << " : " << sValue.c_str() << " : string ]    ";
				else
				{
					if (propertyList[j].getIntValue(iValue)) file << "[ " << propertyList[j].name.c_str() << " : " << iValue << " : integer ]    ";
					else
					{
						if (propertyList[j].getBoolValue(bValue)) file << "[ " << propertyList[j].name.c_str() << " : " << bValue << " : boolean ]    ";
						else
						{
							if (propertyList[j].getDoubleValue(dValue)) file << "[ " << propertyList[j].name.c_str() << " : " << dValue << " : double ]    ";
						}
					}
				}
			}
			file << "\n\n";
		}
		file.close();
	}
	else
	{
		throw "Error open file for writing!";
		return -1;
	}
	return 0;
}

bool PRADIS_scheme_parser::writeFile(std::string path)
{
	try
	{
		std::vector<sch_parser_component> componentList;
		this->parser.getComponentList(componentList);
		writeOutFile(path, componentList);
		return true;
	}
	catch (char const* msg)
	{
		cout << "FAIL: " << msg << endl;
		return false;
	}
	catch (const exception& e) {
		cout << e.what() << endl;
		cout << "Error open writing file!\n";
		return false;
	}
	catch (...)
	{
		cout << "ERROR" << endl;
		return false;
	}
}

sch_parser::sch_parser()
{

}

sch_parser::sch_parser(std::vector<sch_parser_component>& list, std::string path)
{
	try {
		if (this->read(path)) // fill componentList and propertyList
		{
			this->getComponentList(list);
		}
	}
	catch (char const* msg)
	{
		cout << "FAIL: " << msg << endl;
	}
	catch (...)
	{
		cout << "UNKNOWN ERROR" << endl;
	}
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
//////////
// check CFD/FEM classes

bool isCoSimulation(std::string Model)
{
	return !Model.empty() && Model.find("CoSimulation") != std::string::npos;
}

bool isCFDPort(std::string Name)
{
	return !Name.empty() && Name.find("CoSimulation.Port") != std::string::npos;
}

bool isFluidDomain(std::string Name)
{
	return !Name.empty() && Name.find("CoSimulation.FluidDomain") != std::string::npos;
}

bool isSolidDomain(std::string Name)
{
	return !Name.empty() && Name.find("CoSimulation.SolidDomain") != std::string::npos;
}

bool isSolver(std::string Name)
{
	return !Name.empty() && Name.find("CoSimulation.Solver") != std::string::npos;
}

bool isTask(std::string Name)
{
	return !Name.empty() && Name.find("CoSimulation.Task") != std::string::npos;
}
//////////
//check optimization classes
bool isOptimizationScheme(std::string Model)
{
	return ((!Model.empty() && Model.find("multi.") != std::string::npos) || (!Model.empty() && Model.find("Data") != std::string::npos));
}
bool isVariable(std::string Model)
{
	return !Model.empty() && Model.find("multi.Variable") != std::string::npos;
}
bool isData(std::string Model)
{
	return !Model.empty() && Model.find("Data") != std::string::npos;
}
bool isLoadcase(std::string Model)
{
	return !Model.empty() && Model.find("multi.Loadcase") != std::string::npos;
}
bool isOptimization(std::string Model)
{
	return !Model.empty() && Model.find("multi.Optimization") != std::string::npos;
}


//////////

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
	//return !Name.empty() && Name.find("Port") != std::string::npos;
	return !Name.empty() && Name.find("Port") != std::string::npos;
}

void fillPropertyName(sch_parser_component* pc)
{
	if (isCoSimulation(pc->Model))
	{
		if (isCFDPort(pc->Model))
		{
			pc->Properties[0].name = "Name";
			pc->Properties[1].name = "Interface";
		}
		else
		{
			if (isFluidDomain(pc->Model))
			{
				pc->Properties[0].name = "Fluid";
				pc->Properties[1].name = "Pressure";
				pc->Properties[2].name = "Velocity";
				pc->Properties[3].name = "Temperature";
			}
			else
			{
				if (isSolidDomain(pc->Model))
				{
					pc->Properties[0].name = "Solid";
					pc->Properties[1].name = "Velocity";
					pc->Properties[2].name = "Temperature";
					pc->Properties[3].name = "RotationalVelocity";
				}
				else
				{
					if (isSolver(pc->Model))
					{
						pc->Properties[0].name = "EndTime";
						pc->Properties[1].name = "MaximalIterationCount";
						pc->Properties[2].name = "Tolerance";
						pc->Properties[3].name = "Flag.GetJacobian";
					}
					else
					{
						if (isTask(pc->Model))
						{
							pc->Properties[0].name = "File";
							pc->Properties[1].name = "Description";
						}
					}
				}
			}
		}
	}
	else if (isOptimizationScheme(pc->Model))
	{
		if (isVariable(pc->Model))
		{
			pc->Properties[0].name = "Name";
			pc->Properties[1].name = "Value0";
			pc->Properties[2].name = "Min";
			pc->Properties[3].name = "Max";
		}
		else if (isData(pc->Model))
		{
			pc->Properties[0].name = "Value";
		}
		else if (isLoadcase(pc->Model))
		{
			pc->Properties[0].name = "Scheme_File";
			pc->Properties[1].name = "Criteria";
			pc->Properties[2].name = "Solver";
			pc->Properties[3].name = "OpenSign";
			pc->Properties[4].name = "CloseSign";
		}
		else if (isOptimization(pc->Model))
		{
			pc->Properties[0].name = "Method";
			pc->Properties[1].name = "Variables";
			pc->Properties[2].name = "Loadcases";
			pc->Properties[3].name = "Objective";
			pc->Properties[4].name = "Constraints";
			pc->Properties[5].name = "XTol";
			pc->Properties[6].name = "FTol";
			pc->Properties[7].name = "MaxFunction";
			pc->Properties[8].name = "A";
			pc->Properties[9].name = "p";
		}
	}
	/*if (isCFDHeader(pc->Model))
	{
		pc->props[0].name = "Problem";
		pc->props[1].name = "Solver";
		pc->props[2].name = "Dynamic";
	}
	else
	{
		if (isBoundary(pc->Model))
		{
			pc->props[0].name = "Name";
			pc->props[1].name = "Type";
		}
		else
		{
			if (isDomain(pc->Model))
			{
				pc->props[0].name = "Name";
			}
			else
			{
				if (isFluid(pc->Model))
				{
					pc->props[0].name = "Name";
					pc->props[1].name = "FluidChoice";
				}
				else
				{
					if (isParameter(pc->Model))
					{
						pc->props[0].name = "Name";
						pc->props[1].name = "Value";
					}
					else
					{
						if (isPort(pc->Model))
						{
							pc->props[0].name = "Num";
							pc->props[1].name = "Type";
						}
					}
				}
			}
		}
	}*/
}

bool sch_parser::parse(std::string& line)
{
	sch_parser_component* pc = new sch_parser_component();

	pc->Model = line.substr(1, line.find(' ') - 1);	  // MODEL
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
		(pc->Properties).push_back(*pp);
		line = line.substr(line.find('"') + 1);  // skip "_0_" between properties
		line = line.substr(line.find('"') + 1);
		count++;
	}
	fillPropertyName(pc);
	listComp.push_back(*pc);
	return true;
}

bool sch_parser::read(const std::string& file_name)
{
	string s; // сюда будем класть считанные строки
	ifstream file(file_name); // файл из которого читаем (для линукс путь будет выглядеть по другому)
	if (file)
	{
		do
		{
			getline(file, s);
			this->schemeText.push_back(s);
			s = trim(s);
		} while (s != "<Components>");
		getline(file, s);
		this->schemeText.push_back(s);
		do											 // поиск модуля объектов
		{
			s = trim(s); //	очистка строки от лишних пробелов
			parse(s); // парсинг строки
			getline(file, s);
			this->schemeText.push_back(s); // запись текстовой строки схемы
		} while (s != "</Components>");

		while (getline(file, s))
		{
			this->schemeText.push_back(s);
		}


		file.close(); // закрываем файл чтобы не повредить его
		return true;
	}
	else throw "error read from file";
	return false;
}

bool sch_parser::read(istream& stream)
{
	string s; // сюда будем класть считанные строки
	if (stream)
	{
		do
		{
			getline(stream, s);
			this->schemeText.push_back(s);
			s = trim(s);
		} while (s != "<Components>");
		getline(stream, s);
		this->schemeText.push_back(s);
		do											 // поиск модуля объектов
		{
			s = trim(s); //	очистка строки от лишних пробелов
			parse(s); // парсинг строки
			getline(stream, s);
			this->schemeText.push_back(s); // запись текстовой строки схемы
		} while (s != "</Components>");

		while (getline(stream, s))
		{
			this->schemeText.push_back(s);
		}

		return true;
	}
	else throw "error read from stream";
	return false;
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
}

void sch_parser_component::getPropertyList(std::vector<sch_parser_property>& list)
{
	list = Properties;
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