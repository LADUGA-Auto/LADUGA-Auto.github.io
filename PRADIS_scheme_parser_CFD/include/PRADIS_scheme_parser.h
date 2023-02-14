#pragma once
#include <string>
#include <vector>
#include <iostream>		

struct CFDScheme;
struct OPTScheme;

/// Set of possible data types
enum Types
{
	String = 0, 	///< Data type is string
	Integer,	///< Data type is integer
	Double, 	///< Data type is double
	Boolean 	///< Data type is boolean
};



/*!
	\brief Class sch_parser_property

	This class that stores the name, value, and type of the property
*/
class sch_parser_property
{
public:
	Types type;   ///  Value type
	std::string name = "";   /// Property name
	std::string value;   /// Property value

	sch_parser_property();
	~sch_parser_property();
	/*!
	Returns type of property value
	\param[out] type Link to the variable where the data should be returned
	*/
	void getType(int& type);
	//	void getName(stl::string& propertyName);	 /// later
	/*!
	Returns value if type of property value	is string
	\param[out] type Link to the variable where the data should be returned
	\return true if success, false if fail
	*/
	bool getStringValue(std::string& value);
	/*!
	Returns value if type of property value	is integer
	\param[out] type Link to the variable where the data should be returned
	\return true if success, false if fail
	*/
	bool getIntValue(int& value);
	/*!
	Returns value if type of property value	is boolean
	\param[out] type Link to the variable where the data should be returned
	\return true if success, false if fail
	*/
	bool getBoolValue(bool& value);
	/*!
	Returns value if type of property value	is double
	\param[out] type Link to the variable where the data should be returned	
	 \return true if success, false if fail
	*/
	bool getDoubleValue(double& value);

};


/*!
	\brief Class sch_parser_component

	This class that stores the component model, name, activity, and property list 
*/
class sch_parser_component
{
public:
	std::string Model, Name, Active;	   
	std::vector<sch_parser_property> Properties;   /// property list for component	
	
	sch_parser_component();
	~sch_parser_component();
	/*!
	Returns component model
	\param[out] type Link to the variable where the data should be returned
	*/
	void getModel(std::string& model_name);
	/*!
	Returns component name
	\param[out] type Link to the variable where the data should be returned
	*/
	void getName(std::string& componentName);
	/*!
	Returns component activity
	\return true if component active, else return false
	*/
	bool isActive(); 
	/*!
	Returns property list 
	\param[out] type Link to the variable where the data should be returned
	*/
	void getPropertyList(std::vector<sch_parser_property>& list);

	//	void getNodesList(vector<sch_parser_node> &list);		// later			   


}; 



/*!
	\brief Hidden class for working with the scheme

	This class reads, parses and stores the received data in its fields
*/
class sch_parser
{
public:
	std::vector<sch_parser_component> listComp;	/// list of read components
	std::vector<std::string> schemeText; /// list to store the read schema

	sch_parser();
	sch_parser(std::vector<sch_parser_component>& list, std::string path);
	~sch_parser();
	/*!
	Parses the read line on components and components propeties
	\param[in] line Line read from file
	\return true if success
	*/
	bool parse(std::string& line);
	//void getSchemeText(std::string& scheme_text); // later
	//int parse(std::string& scheme_text); // later
	/*!
	Read the .sch scheme by file name, fills field "schemeText"	
	Calls parse only for Components of schema
	\param[in] file_name Scheme file name 
	\throw char const* If unable to read from the file
	\throw const exception& If the file could not be opened
	\return true if success, false if fail
	*/
	bool read(const std::string& file_name);		
	/*!
	Read the .sch scheme by std::stream&, fills field "schemeText"
	Calls parse only for Components of schema
	\param[in] stream Stream open for reading
	\throw char const* If unable to read from the stream
	\throw const exception& If the stream could not be opened
	\return true if success, false if fail
	*/
	bool read(std::istream& stream);	
	/*!
	Returns a list of components
	\param[out] list Link to the list where the data should be returned
	*/
	void getComponentList(std::vector<sch_parser_component>& list);
};

/*!
	\brief Public class that allows you to read data from the schema

	This class allows you to read the scheme both by file name and from the stream
*/
class PRADIS_scheme_parser
{
private:
	sch_parser parser;
public:
	PRADIS_scheme_parser();

	/*!
	Read the .sch scheme by file name
	\param[out] scheme Scheme with components reads from the file
	\param[in] path Path and file name to read
	\throw char const* If unable to read from the file
	\throw const exception& If the file could not be opened
	\return true if success, false if fail
	*/
	bool load(CFDScheme& scheme, std::istream& stream);

	/*!
	Read the .sch scheme by file name
	\param[out] scheme Scheme with components reads from the file
	\param[in] path Path and file name to read
	\throw char const* If unable to read from the file
	\throw const exception& If the file could not be opened
	\return true if success, false if fail
	*/
	bool load(OPTScheme& scheme, std::istream& stream);
	
	/*!
	Write .out file as result of parsing
	\param std::string path result file
	\throw char const* If unable to read from the stream
	\throw const exception& If the stream could not be opened
	\return true if success, false if fail
	*/
	bool writeFile(std::string path);
	//bool close();
};

/*
class sch_parser_node
{
	sch_parser_node();
	~sch_parser_node();

	void getType(int &type);
	void getName(stl::string& nodeName);

}	  */

/*!
	Helper function to write to file
	\param std::string path result file
	\param componentList is list component for writing in file
	\throw char const* If the file could not be opened
	\return 0 if success, -1 if fail
	*/
int writeOutFile(std::string outputFile, std::vector<sch_parser_component> componentList);