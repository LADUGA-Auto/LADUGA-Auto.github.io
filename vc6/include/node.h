/***************************************************************************
                                 node.h
                                --------
    begin                : Sat Sep 20 2003
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

#ifndef NODE_H
#define NODE_H

#include "element.h"

#include <qptrlist.h>

class ViewPainter;
class WireLabel;
class QPainter;


class Node : public Conductor {
public:
  Node(int, int);
 ~Node();

  void  paint(ViewPainter*);// must be deleted
  bool  getSelected(int, int);// must be deleted

  QPtrList<Element> Connections;
  QString Name;   // node name used by creation of netlist
  int State;	// remember some things during some operations
};

#endif
