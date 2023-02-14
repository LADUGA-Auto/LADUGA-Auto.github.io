/***************************************************************************
                               subcircuit.cpp
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "PSLSubScheme.h"
#include "schematic.h"
#include "main.h"

#include <qdir.h>
#include <qfileinfo.h>
#include <qregexp.h>

#include <math.h>
#include <limits.h>

extern QDir QucsWorkDir;


PSLSubScheme::PSLSubScheme()
{
  Type = isComponent;   // both analog and digital
  Description = QObject::tr("pslsubcircuit");

  Props.append(new Property("Name", "", true,
		QObject::tr("Name of PSL subscheme")));
  Props.append(new Property("NodesNumber", "0", false,
		QObject::tr("Number of nodes of PSL subscheme")));
/*	Props.append(new Property("ParametersNumber", "0", false,
		QObject::tr("Number of parameters of PSL subscheme")));
*/	
	Props.append(new Property("Par_1", "", false,		""));

  Model = "PSLSub";
  Name  = "PSLSub";

  // Do NOT call createSymbol() here. But create port to let it rotate.
  Ports.append(new Port(0, 0));
  
  type = 8;
	pElement=0;
}

// ---------------------------------------------------------------------
Component* PSLSubScheme::newOne()
{
  PSLSubScheme *p = new PSLSubScheme();
	Property*		it;
  	p->Props.clear();
  for (it = Props.first();it!=0;it = Props.next())
  {
  	p->Props.append(new Property(it->Name, it->Value, it->display, it->Description));
//  	p->Props.getFirst()->Value = Props.getFirst()->Value;
  }
  p->recreate(0);
  return p;
}

