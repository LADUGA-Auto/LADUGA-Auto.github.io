#include <string>
#include<vector>
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
	Types type;			  // type of value
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
	std::string Model, Name;
	bool Active;
	std::vector<sch_parser_property> props;

	sch_parser_component();
	~sch_parser_component();

	void getModel(std::string& model_name);  
	void getName(std::string& componentName); 
	bool isActive(); 
	void getPropertyList(std::vector<sch_parser_property>& list); 
//	void getNodesList(vector<sch_parser_node> &list);


};