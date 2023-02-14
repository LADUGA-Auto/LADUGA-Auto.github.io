#pragma once
#include "PRADIS_scheme_parser.h"
#include <string>
#include <vector>

/// <summary>
///  Class for describing the PRADIS "Port" object
/// </summary>
class Port : public sch_parser_component 
{
public:
	Port();

	/// data is copied from pc
	Port(sch_parser_component* pc);	   
};


/// <summary>
/// Class for describing the PRADIS "Fluid domain" object
/// </summary>
class FluidDomain : public sch_parser_component  
{
public:
	FluidDomain();

	/// data is copied from pc
	FluidDomain(sch_parser_component* pc);	   
};

/// <summary>
/// Class for describing the PRADIS "Solid domain" object
/// </summary>
class SolidDomain : public sch_parser_component  
{
public:
	SolidDomain();

	/// data is copied from pc
	SolidDomain(sch_parser_component* pc);	  

};

/// <summary>
/// Class for describing the PRADIS "Solver" object
/// </summary>
class Solver : public sch_parser_component  
{
public:
	Solver();

	/// data is copied from pc
	Solver(sch_parser_component* pc); 	  
};
	
/// <summary>
/// Class for describing the PRADIS "Task" object
/// </summary>
class Task : public sch_parser_component  
{
public:
	Task();

	/// data is copied from pc
	Task(sch_parser_component* pc);	   
};
/// <summary>
/// Structure for describing the CFD scheme
/// </summary>
struct CFDScheme	
{
	///PRADIS "Port" storage list 
	std::vector<Port> portList;   

	///PRADIS "Fluid domain" storage list
	std::vector<FluidDomain> fluidDomainList; 

	///PRADIS "Solid domain" storage list
	std::vector<SolidDomain> solidDomainList; 

	/// PRADIS "Solver" storage field
	Solver solver;	

	/// PRADIS "Task" storage field
	Task task;   
};

/// Predicate for defining objects 
bool isCFDPort(std::string Name);

/// Predicate for defining objects 
bool isFluidDomain(std::string Name);

/// Predicate for defining objects 
bool isSolidDomain(std::string Name);

/// Predicate for defining objects 
bool isSolver(std::string Name);

/// Predicate for defining objects 
bool isTask(std::string Name);

/*!
Parses the CoSimulation scheme
\param componentList List of components read from the schematic 
\param scheme filling structure for describing the CFD scheme 
\return 0 if success
*/
int parseCoSimulation(std::vector<sch_parser_component>& componentList, CFDScheme& scheme);