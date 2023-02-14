/***************************************************************************
                                ground.cpp
                               ------------
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

#include "PRADISData.h"


PRADISData::PRADISData()
{
  Type = isComponent;   // both analog and digital
  Description = QObject::tr("Data description");
  type=5;

  Lines.append(new Line( -35,  -4,  35, -4,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(38, -1, 38, 10,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-38, 10, -38, -1,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( -35, 13,  35, 13,QPen(QPen::darkBlue,2)));

  Arcs.append(new Arc(31, -4, 7, 7, 0, 1440,QPen(QPen::darkBlue,2)));
  Arcs.append(new Arc(-38, -4, 7, 7, 1440, 1440 ,QPen(QPen::darkBlue,2)));
  Arcs.append(new Arc(-38, 6, 7, 7, 2880, 1440 ,QPen(QPen::darkBlue,2)));
  Arcs.append(new Arc(31, 6, 7, 7, 4320 , 1440 ,QPen(QPen::darkBlue,2)));

  Texts.append(new Text( -15,-3, "Data"));
  
  pElement=0;
  

//  Ports.append(new Port(  0,  0));
  x1 = -42; y1 = -8;
  x2 =  42; y2 = 17;

  tx = x1+4;
  ty = y2+4;
  Model = "Data";
  Name  = "Data";
  
  text = new Property ("Text", "", false,"Text2");
  Props.append(text);
}

PRADISData::~PRADISData()
{
}

Component* PRADISData::newOne()
{
  return new PRADISData();
}



void PRADISData::setData(const QString& str)
{
	QString value = str;
	value = convert2uc(value);
	text->Value = value;
};

QString PRADISData::getData()
{
	QString value = text->Value ;
	value = convert2int(value);
	
	return value;
};
