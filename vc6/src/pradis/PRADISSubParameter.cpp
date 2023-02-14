/***************************************************************************
                               subcirport.cpp
                              ----------------
    begin                : Sat Aug 23 2003
    copyright            : (C) 2003 by Michael Margraf
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

#include "PRADISSubParameter.h"
#include "node.h"
#include "schematic.h"


PRADISSubParameter::PRADISSubParameter()
{
  Type = isComponent;   // both analog and digital
  Description = QObject::tr("parameter of a subcircuit");

  // This property must be the first one !
  Props.append(new Property("Num", "1", true,
		QObject::tr("number of the port within the subcircuit")));
  // This property must be the second one !
  Props.append(new Property("Type", "analog", false,
		QObject::tr("type of the port (for digital simulation only)")
		+" [analog, in, out, inout]"));

  createSymbol();
  tx = x1+4;
  ty = y2+4;
  Model = "Port";
  Name  = "P";
  
  type = 7;  
	pElement=0;
}

// -------------------------------------------------------
void PRADISSubcirPort::createSymbol()
{
  x1 = -27; y1 = -8;
  x2 =   0; y2 =  8;

  if(Props.at(1)->Value.at(0) == 'a') {
    Arcs.append(new Arc(-25, -3, 6, 6,  0, 16*360,QPen(QPen::darkBlue,2)));
    Lines.append(new Line(-18,  0,  0,  0,QPen(QPen::darkBlue,2)));

//	EArc -25 -3 6 6  0 5760 #000080 2 1:
  
//  Line -18 0 13 0  #000080 2 1

  }
  else {
    Lines.append(new Line( -9,  0,  0,  0,QPen(QPen::darkBlue,2)));
    if(Props.at(1)->Value == "out") {
      Lines.append(new Line(-20, -5,-25,  0,QPen(QPen::red,2)));
      Lines.append(new Line(-20,  5,-25,  0,QPen(QPen::red,2)));
      Lines.append(new Line(-20, -5, -9, -5,QPen(QPen::red,2)));
      Lines.append(new Line(-20,  5, -9,  5,QPen(QPen::red,2)));
      Lines.append(new Line( -9, -5, -9,  5,QPen(QPen::red,2)));
    }
    else {
      Lines.append(new Line(-14, -5, -9,  0,QPen(QPen::darkGreen,2)));
      Lines.append(new Line(-14,  5, -9,  0,QPen(QPen::darkGreen,2)));
      if(Props.at(1)->Value == "in") {
        Lines.append(new Line(-25, -5,-14, -5,QPen(QPen::darkGreen,2)));
        Lines.append(new Line(-25,  5,-14,  5,QPen(QPen::darkGreen,2)));
        Lines.append(new Line(-25, -5,-25,  5,QPen(QPen::darkGreen,2)));
      }
      else {
        x1 = -30;
        Lines.append(new Line(-18, -5,-14, -5,QPen(QPen::darkGreen,2)));
        Lines.append(new Line(-18,  5,-14,  5,QPen(QPen::darkGreen,2)));
        Lines.append(new Line(-23, -5,-28,  0,QPen(QPen::red,2)));
        Lines.append(new Line(-23,  5,-28,  0,QPen(QPen::red,2)));
        Lines.append(new Line(-23, -5,-18, -5,QPen(QPen::red,2)));
        Lines.append(new Line(-23,  5,-18,  5,QPen(QPen::red,2)));
      }
    }
  }

  Ports.append(new Port(  0,  0));
}

// -------------------------------------------------------
Component* PRADISSubcirPort::newOne()
{
  return new PRADISSubcirPort();
}

// -------------------------------------------------------
Element* PRADISSubcirPort::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Subcircuit Port");
  BitmapFile = "subport";

  if(getNewOne)  return new PRADISSubcirPort();
  return 0;
}

// -------------------------------------------------------
QString PRADISSubcirPort::netlist()
{
  return QString("");
}

// -------------------------------------------------------
QString PRADISSubcirPort::vhdlCode(int)
{
  return QString("");
}

// -------------------------------------------------------
QString PRADISSubcirPort::verilogCode(int)
{
  return QString("");
}
