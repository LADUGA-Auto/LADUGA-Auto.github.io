/***************************************************************************
                               schematic.h
                              -------------
    begin                : Sat Mar 11 2006
    copyright            : (C) 2006 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include "wire.h"
#include "diagrams/diagram.h"
#include "paintings/painting.h"

class Schematic{
  
public: 
  Schematic(const QString&);
 ~Schematic();

  bool    load();
  int     save();
  QString DocName;
  QString DataSet;     // name of the default dataset
  QString DataDisplay;
  float Scale;
  bool GridOn;
   bool SimOpenDpl; 

  // The pointers points to the current lists, either to the schematic
  // elements "Doc..." or to the symbol elements "SymbolPaints".
  QPtrList<Wire>      *Wires, DocWires;
  QPtrList<Node>      *Nodes, DocNodes;
  QPtrList<Diagram>   *Diagrams, DocDiags;
  QPtrList<Painting>  *Paintings, DocPaints;
  QPtrList<Component> *Components, DocComps;

  QPtrList<Painting>  SymbolPaints;  // symbol definition for subcircuit

  int GridX, GridY;
  int ViewX1, ViewY1, ViewX2, ViewY2;  // size of the document area
  int UsedX1, UsedY1, UsedX2, UsedY2;  // document area used by elements

  int showFrame;
  QString Frame_Text0, Frame_Text1, Frame_Text2, Frame_Text3;

  // Two of those data sets are needed for Schematic and for symbol.
  // Which one is in "tmp..." depends on "symbolMode".
  float tmpScale;
  int tmpViewX1, tmpViewY1, tmpViewX2, tmpViewY2;
  int tmpUsedX1, tmpUsedY1, tmpUsedX2, tmpUsedY2;


/* ********************************************************************
   *****  The following methods are in the file                   *****
   *****  "schematic_element.cpp". They only access the QPtrList  *****
   *****  pointers "Wires", "Nodes", "Diagrams", "Paintings" and  *****
   *****  "Components".                                           *****
   ******************************************************************** */

public:
  Node* insertNode(int, int, Element*);

  int   insertWireNode1(Wire*);
  int   insertWireNode2(Wire*);
  int   insertWire(Wire*);
  Wire* splitWire(Wire*, Node*);
  bool  oneTwoWires(Node*);
  void  deleteWire(Wire*);


  void     newMovingWires(QPtrList<Element>*, Node*, int);

  void       setComponentNumber(Component*);
  void       insertRawComponent(Component*, bool noOptimize=true);
  void       recreateComponent(Component*);
  void       insertComponent(Component*);
  void       setCompPorts(Component*);
  Component* searchSelSubcircuit();

  void     oneLabel(Node*);
  int      placeNodeLabel(WireLabel*);
  Element* getWireLabel(Node*);
 

private:
  void insertComponentNodes(Component*, bool);


/* ********************************************************************
   *****  The following methods are in the file                   *****
   *****  "schematic_file.cpp". They only access the QPtrLists    *****
   *****  and their pointers. ("DocComps", "Components" etc.)     *****
   ******************************************************************** */

public:

  bool loadDocument();/////////ne trogat

private:
  int  saveDocument();///////// ne trogat

  bool loadProperties(QTextStream*);
  void simpleInsertComponent(Component*);
  bool loadComponents(QTextStream*, QPtrList<Component> *List=0);
  void simpleInsertWire(Wire*);
  bool loadWires(QTextStream*, QPtrList<Element> *List=0);
  bool loadDiagrams(QTextStream*, QPtrList<Diagram>*);
  bool loadPaintings(QTextStream*, QPtrList<Painting>*);
public:
  bool isVerilog;
  bool creatingLib;
  
//PRADIS methods
	//PPL Ppl;
//PRA-10


  //////////////////////////////////////////////////////////////////////////////////////
	bool isPassport;
};

#endif
