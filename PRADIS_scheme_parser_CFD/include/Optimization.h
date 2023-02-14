#pragma once
#include "PRADIS_scheme_parser.h"
#include <string>

using namespace std;

/// Class for describing the PRADIS "Variable" object
class Variable : public sch_parser_component   
{
public:
	Variable();

	/// data is copied from pc
	Variable(sch_parser_component* pc);	  
};


/// Class for describing the PRADIS "Data" object
class Data : public sch_parser_component    
{
public:
	Data();

	/// data is copied from pc
	Data(sch_parser_component* pc);	  
};




/// Class for describing the PRADIS "Loadcase" object
class Loadcase : public sch_parser_component   
{
public:
	Loadcase();

	/// data is copied from pc
	Loadcase(sch_parser_component* pc);	  
};


/// Class for describing the PRADIS "Optimization" object
class Optimization : public sch_parser_component    
{
public:
	Optimization();

	/// data is copied from pc
	Optimization(sch_parser_component* pc);	  
};

/// <summary>
/// Structure for describing the optimization scheme
/// </summary>
struct OPTScheme  
{	///PRADIS "Variable" storage list
	std::vector<Variable> variableList;  

	///PRADIS "data" storage list 
	std::vector<Data> dataList;   

	///PRADIS "Loadcase" storage list
	std::vector<Loadcase> loadcaseList;   

	///PRADIS "Optimization" storage list
	std::vector<Optimization> optimizationList;   
};

/// Predicate for defining objects 
bool isVariable(std::string Model);

/// Predicate for defining objects 
bool isData(std::string Model);

/// Predicate for defining objects 
bool isLoadcase(std::string Model);

/// Predicate for defining objects 
bool isOptimization(std::string Model);

/*!
Parses the optimization scheme
\param componentList List of components read from the schematic
\param scheme filling structure for describing the optimization scheme
\return 0 if success
*/
int parseOptimization(std::vector<sch_parser_component> componentList, OPTScheme& scheme);







