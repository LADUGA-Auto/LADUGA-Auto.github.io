/***************************************************************************
                          wirelabel.cpp  -  description
                             -------------------
    begin                : Sun February 29 2004
    copyright            : (C) 2004 by Michael Margraf
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

#include "wirelabel.h"
#include "wire.h"
#include "main.h"


WireLabel::WireLabel(const QString& _Name, int _cx, int _cy,
                     int _x1, int _y1, int _Type)
{
  cx = _cx;
  cy = _cy;
  x1 = _x1;
  y1 = _y1;
  setName(_Name);
  initValue = "";

  Type = _Type;
  isSelected = false;
}

WireLabel::~WireLabel()
{
}

// ----------------------------------------------------------------
void WireLabel::setCenter(int x_, int y_, bool relative)
{
  switch(Type) {
    case isMovingLabel:
      if(relative) {
        x1 += x_;  cx += x_;
        y1 += y_;  cy += y_;
      }
      else {
        x1 = x_;  cx = x_;
        y1 = y_;  cy = y_;
      }
      break;
    case isHMovingLabel:
      if(relative) { x1 += x_;  cx += x_; }
      else { x1 = x_;  cx = x_; }
      break;
    case isVMovingLabel:
      if(relative) { y1 += y_;  cy += y_; }
      else { y1 = y_;  cy = y_; }
      break;
    default:
      if(relative) {
        x1 += x_;  y1 += y_; // moving cx/cy is done by owner (wire, node)
      }
      else { x1 = x_; y1 = y_; }
  }
}

// ----------------------------------------------------------------
void WireLabel::setName(const QString& Name_)
{
  /*Name = Name_;

  QFontMetrics  metrics(QucsSettings.font);    // get size of text
  QSize r = metrics.size(0, Name);
  x2 = r.width();
  y2 = r.height()-2; */   // remember size of text
}

// ----------------------------------------------------------------
// Converts all necessary data of the wire into a string. This can be used to
// save it to an ASCII file or to transport it via the clipboard.
// Wire labels use the same format like wires, but with length zero.
QString WireLabel::save()
{
  QString s("<");
	s += QString::number(cx)+" "+QString::number(cy)+" "
	  +  QString::number(cx)+" "+QString::number(cy)
	  +  " \""+Name +"\" "
	  +  QString::number(x1)+" "+QString::number(y1)+" 0 \""
	  +  initValue+"\">";
  return s;
}
