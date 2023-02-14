#pragma once
#include <string>
#include <vector>


enum Types
{
	String = 0,
	Integer,
	Double,
	Boolean
};


class sch_parser_property
{
public:
	Types type;			// valueType
	std::string name = "";
	std::string value;

	sch_parser_property();
	~sch_parser_property();

	void getType(int& type);
	//	void getName(stl::string& propertyName);
	bool getStringValue(std::string& value);
	bool getIntValue(int& value);
	bool getBoolValue(bool& value);
	bool getDoubleValue(double& value);

};

class sch_parser_component
{
public:
	std::string Model, Name, Active;
	std::vector<sch_parser_property> props;

	sch_parser_component();
	~sch_parser_component();

	void getModel(std::string& model_name);
	void getName(std::string& componentName);
	bool isActive();
	void getPropertyList(std::vector<sch_parser_property>& list);
	//	void getNodesList(vector<sch_parser_node> &list);


};

class sch_parser
{
public:
	std::vector<sch_parser_component> listComp;
						 

	sch_parser();
	~sch_parser();

	int read(std::string file_name);
	//void getSchemeText(std::string& scheme_text); // later
	//int parse(std::string& scheme_text); // later
	int parse(const std::string& file_name);
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