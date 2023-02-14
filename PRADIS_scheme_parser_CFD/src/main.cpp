#define _CRT_SECURE_NO_WARNINGS

#include "PRADIS_scheme_parser.h"
#include "CoSimulation.h"
#include "Optimization.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
/// function for print CFD scheme
int printScheme(CFDScheme& scheme)
{
	cout << "Printing CFD scheme...\n";
	for (int i = 0; i < scheme.portList.size(); i++)
	{
		std::cout << scheme.portList[i].Model << " " << scheme.portList[i].Name << " " << scheme.portList[i].Active << " ";
		for (int j = 0; j < scheme.portList[i].Properties.size(); j++) {
			std::cout << scheme.portList[i].Properties[j].name << " : " << scheme.portList[i].Properties[j].value << ", ";
		}
		std::cout << std::endl;
	}
	printf("portList is done!\n\n");
	for (int i = 0; i < scheme.fluidDomainList.size(); i++)
	{
		std::cout << scheme.fluidDomainList[i].Model << " " << scheme.fluidDomainList[i].Name << " " << scheme.fluidDomainList[i].Active << " ";
		for (int j = 0; j < scheme.fluidDomainList[i].Properties.size(); j++) {
			std::cout << scheme.fluidDomainList[i].Properties[j].name << " : " << scheme.fluidDomainList[i].Properties[j].value << ", ";
		}
		std::cout << std::endl;
	}
	printf("fluidDomainList is done!\n\n");
	for (int i = 0; i < scheme.solidDomainList.size(); i++)
	{
		std::cout << scheme.solidDomainList[i].Model << " " << scheme.solidDomainList[i].Name << " " << scheme.solidDomainList[i].Active << " ";
		for (int j = 0; j < scheme.solidDomainList[i].Properties.size(); j++) {
			std::cout << scheme.solidDomainList[i].Properties[j].name << " : " << scheme.solidDomainList[i].Properties[j].value << ", ";
		}
		std::cout << std::endl;
	}
	printf("solidDomainList is done!\n\n");

	std::cout << scheme.solver.Model << " " << scheme.solver.Name << " " << scheme.solver.Active << " ";
	for (int i = 0; i < scheme.solver.Properties.size(); i++)
	{
		std::cout << scheme.solver.Properties[i].name << " : " << scheme.solver.Properties[i].value << ", ";
	}
	std::cout << std::endl;

	printf("solver is done!\n\n");

	std::cout << scheme.task.Model << " " << scheme.task.Name << " " << scheme.task.Active << " ";
	for (int i = 0; i < scheme.task.Properties.size(); i++)
	{
		std::cout << scheme.task.Properties[i].name << " : " << scheme.task.Properties[i].value << ", ";
	}
	std::cout << std::endl;

	printf("task is done!\n\n");
	return 0;
}
/// function for print optimization scheme
int printScheme(OPTScheme& scheme)
{
	cout << "Printing OPT scheme...\n";
	for (int i = 0; i < scheme.variableList.size(); i++)
	{
		std::cout << scheme.variableList[i].Model << " " << scheme.variableList[i].Name << " " << scheme.variableList[i].Active << " ";
		for (int j = 0; j < scheme.variableList[i].Properties.size(); j++) {
			std::cout << scheme.variableList[i].Properties[j].name << " : " << scheme.variableList[i].Properties[j].value << ", ";
		}
		std::cout << std::endl;
	}
	printf("variableList is done!\n\n");

	for (int i = 0; i < scheme.dataList.size(); i++)
	{
		std::cout << scheme.dataList[i].Model << " " << scheme.dataList[i].Name << " " << scheme.dataList[i].Active << " ";
		for (int j = 0; j < scheme.dataList[i].Properties.size(); j++) {
			std::cout << scheme.dataList[i].Properties[j].name << " : " << scheme.dataList[i].Properties[j].value << ", ";
		}
		std::cout << std::endl;
	}
	printf("dataList is done!\n\n");

	for (int i = 0; i < scheme.loadcaseList.size(); i++)
	{
		std::cout << scheme.loadcaseList[i].Model << " " << scheme.loadcaseList[i].Name << " " << scheme.loadcaseList[i].Active << " ";
		for (int j = 0; j < scheme.loadcaseList[i].Properties.size(); j++) {
			std::cout << scheme.loadcaseList[i].Properties[j].name << " : " << scheme.loadcaseList[i].Properties[j].value << ", ";
		}
		std::cout << std::endl;
	}
	printf("loadcaseList is done!\n\n");

	for (int i = 0; i < scheme.optimizationList.size(); i++)
	{
		std::cout << scheme.optimizationList[i].Model << " " << scheme.optimizationList[i].Name << " " << scheme.optimizationList[i].Active << " ";
		for (int j = 0; j < scheme.optimizationList[i].Properties.size(); j++) {
			std::cout << scheme.optimizationList[i].Properties[j].name << " : " << scheme.optimizationList[i].Properties[j].value << ", ";
		}
		std::cout << std::endl;
	}
	printf("optimizationList is done!\n\n");
	return 0;
}


/// function for write result parsing to file
/*int writeFile(std::string outputFile, std::vector<sch_parser_component> componentList)
{
	std::vector<sch_parser_property> propertyList;
	ofstream file(outputFile, 'w');
	if (file) // writing components info to file
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
		printf("Complete write file!\n");
	}
	else
	{
		printf("Error write file!\n");
		return -1;
	}
	return 0;
}	 */


int main()
{
	setlocale(LC_ALL, ".UTF8");	 // для корректного отображения кириллицы
	const std::string inputFileCFD = "tests/FEM/test1.sch", inputFileOPT = "tests/optimization/optimization.sch", outputFileCFD = inputFileCFD.substr(0, inputFileCFD.rfind('.')) + ".out", outputFileOPT = inputFileOPT.substr(0, inputFileOPT.rfind('.')) + ".out";

	PRADIS_scheme_parser parser;
	ifstream file;
	file.open(inputFileCFD);
	if (file.is_open())
	{
		CFDScheme CfdScheme;
		parser.load(CfdScheme, file);
		parser.writeFile(outputFileCFD);

		printScheme(CfdScheme);
	}
	file.close();


	file.open(inputFileOPT);
	if (file)
	{
		OPTScheme OptScheme;
		parser.load(OptScheme, file); 
		parser.writeFile(outputFileOPT);

		printScheme(OptScheme);
	}
	file.close();

	return 0;
}
