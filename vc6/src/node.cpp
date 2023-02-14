/***************************************************************************
                          node.cpp  -  description
                             -------------------
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

#include "node.h"

#include "wirelabel.h"

Node::Node(int _x, int _y)
{
  Label = 0;
  Type  = isNode;
  State = 0;

  cx = _x;
  cy = _y;
}

Node::~Node()
{
}
// ----------------------------------------------------------------
bool Node::getSelected(int x_, int y_)
{
  if(cx-5 <= x_) if(cx+5 >= x_) if(cy-5 <= y_) if(cy+5 >= y_)
    return true;

  return false;
}

// ----------------------------------------------------------------
