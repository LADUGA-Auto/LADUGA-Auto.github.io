#pragma once
#include <string>
#include <vector>
#include "schematic.h"
#include "../adm/parserComponent.h"


class sch_parser
{
public:
	std::vector<sch_parser_component> listComp;
	Schematic* scheme;

	sch_parser();
	~sch_parser();

	int read(const std::string& file_name); 
	//void getSchemeText(std::string& scheme_text); // later
	//int parse(std::string& scheme_text); // later
	int parse();
	void getComponentList(std::vector<sch_parser_component>& list); 
};

/*
class sch_parser_node
{
	sch_parser_node();
	~sch_parser_node();

	void getType(int &type);
	void getName(stl::string& nodeName);

}	  */