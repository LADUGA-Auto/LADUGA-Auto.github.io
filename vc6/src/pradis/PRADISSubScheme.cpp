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

#include "PRADISSubScheme.h"
#include "schematic.h"
#include "main.h"

#include <qdir.h>
#include <qfileinfo.h>
#include <qregexp.h>

#include <math.h>
#include <limits.h>

extern QDir QucsWorkDir;


PRADISSubScheme::PRADISSubScheme()
{
  Type = isComponent;   // both analog and digital
  Description = QObject::tr("subcircuit");

  Props.append(new Property("Scheme_File", "", false,
		QObject::tr("name of qucs schematic file")));

  Model = "Sub";
  Name  = "SUB";

  // Do NOT call createSymbol() here. But create port to let it rotate.
  Ports.append(new Port(0, 0));
  
  type = 6;
	pElement=0;
}

// ---------------------------------------------------------------------
Component* PRADISSubScheme::newOne()
{
  PRADISSubScheme *p = new PRADISSubScheme();
  p->Props.getFirst()->Value = Props.getFirst()->Value;
  p->recreate(0);
  return p;
}
