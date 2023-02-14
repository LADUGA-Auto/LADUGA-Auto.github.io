/***************************************************************************
                               component.cpp
                              ---------------
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

#include <stdlib.h>

//#include "components.h"
//#include "node.h"
#include "main.h"
//#include "qucs.h"
#include "schematic.h"
//#include "viewpainter.h"
#include "../pradis/PRADISComponent.h"
//#include <qdir.h>
//#include <qpen.h>
//#include <qpoint.h>
//#include <qstring.h>
//#include <qpainter.h>
//#include <qtabwidget.h>
#include <qmessagebox.h>

#include <math.h>



// ***********************************************************************
// **********                                                   **********
// **********                  class "Component"                **********
// **********                                                   **********
// ***********************************************************************
Component::Component()
{
  Type = isAnalogComponent;

  mirroredX = false;
  rotated = 0;
  isSelected = false;
  isActive = COMP_IS_ACTIVE;
  showName = true;

  cx = 0;
  cy = 0;
  tx = 0;
  ty = 0;

  Arcs.setAutoDelete(true);
  Lines.setAutoDelete(true);
  Rects.setAutoDelete(true);
  Ellips.setAutoDelete(true);
  Ports.setAutoDelete(true);
  Texts.setAutoDelete(true);
  Props.setAutoDelete(true);
}

// -------------------------------------------------------
Component* Component::newOne()
{
  return new Component();
}

// -------------------------------------------------------
void Component::setCenter(int x, int y, bool relative)
{
  if(relative) { cx += x;  cy += y; }
  else { cx = x;  cy = y; }
}

// -------------------------------------------------------
void Component::getCenter(int& x, int& y)
{
  x = cx;
  y = cy;
}



// -------------------------------------------------------
QString Component::save()
{
  QString s = "<" + Model;

  if(Name.isEmpty()) s += " * ";
  else s += " "+Name+" ";

  int i=0;
  if(!showName)
    i = 4;
  i |= isActive;
  s += QString::number(i);
  s += " "+QString::number(cx)+" "+QString::number(cy);
  s += " "+QString::number(tx)+" "+QString::number(ty);
  if(mirroredX) s += " 1";
  else s += " 0";
  s += " "+QString::number(rotated);

  // write all properties
  for(Property *p1 = Props.first(); p1 != 0; p1 = Props.next()) {
    if(p1->Description.isEmpty())
      s += " \""+p1->Name+"="+p1->Value+"\"";   // e.g. for equations
    else s += " \""+p1->Value+"\"";
    if(p1->display) s += " 1";
    else s += " 0";
  }

  return s+">";
}

// -------------------------------------------------------
bool Component::load(const QString& _s)
{
  bool ok;
  int  ttx, tty, tmp;
  QString s = _s;

  if(s.at(0) != '<') return false;
  if(s.at(s.length()-1) != '>') return false;
  s = s.mid(1, s.length()-2);   // cut off start and end character

  QString n;
  Name = s.section(' ',1,1);    // Name
  if(Name == "*") Name = "";

  n  = s.section(' ',2,2);      // isActive
  tmp = n.toInt(&ok);
  if(!ok) return false;
  isActive = tmp & 3;

  if(tmp & 4)
    showName = false;
  else
    showName = true;

  n  = s.section(' ',3,3);    // cx
  cx = n.toInt(&ok);
  if(!ok) return false;

  n  = s.section(' ',4,4);    // cy
  cy = n.toInt(&ok);
  if(!ok) return false;

  n  = s.section(' ',5,5);    // tx
  ttx = n.toInt(&ok);
  if(!ok) return false;

  n  = s.section(' ',6,6);    // ty
  tty = n.toInt(&ok);
  if(!ok) return false;

  if(Model.at(0) != '.') {  // is simulation component (dc, ac, ...) ?

    n  = s.section(' ',7,7);    // mirroredX
    //if(n.toInt(&ok) == 1) mirrorX();
    //if(!ok) return false;

    n  = s.section(' ',8,8);    // rotated
    //tmp = n.toInt(&ok);
    //if(!ok) return false;
    ///if(rotated > tmp)  // neccessary because of historical flaw in ...
    //  tmp += 4;        // ... components like "volt_dc"
    //for(int z=rotated; z<tmp; z++) rotate();

  }

  tx = ttx; ty = tty; // restore text position (was changed by rotate/mirror)

  unsigned int z=0, counts = s.contains('"');
  if(Model == "Sub")
    tmp = 2;   // first property (File) already exists
  else if(Model == "Lib")
    tmp = 3;
  else if(Model == "EDD")
    tmp = 5;
  else tmp = 0 + 1;    // "+1" because "counts" could be zero

  

  for(; tmp<=(int)counts/2; tmp++)
    Props.append(new Property("p", "", true, " "));

  // load all properties
  Property *p1;
  for(p1 = Props.first(); p1 != 0; p1 = Props.next()) {
    z++;
    n = s.section('"',z,z);    // property value
    z++;
    // not all properties have to be mentioned (backward compatible)
    if(z > counts) {
      if(p1->Description.isEmpty())
        Props.remove();    // remove if allocated in vain

      /*if(counts < 58)     // backward compatible
        if(Model == "Diode") {
          counts >>= 1;
          p1 = Props.at(counts-1);
          for(; p1 != 0; p1 = Props.current()) {
            if(counts-- < 19)
              break;

            n = Props.prev()->Value;
            p1->Value = n;
          }

          p1 = Props.at(17);
          p1->Value = Props.at(11)->Value;
          Props.current()->Value = "0";
        }*/

      return true;
    }

  /*  if(Model != "EDD")
    if(p1->Description.isEmpty()) {  // unknown number of properties ?
      p1->Name = n.section('=',0,0);
      n = n.section('=',1,1);
      // allocate memory for a new property (e.g. for equations)
      if(Props.count() < (counts>>1)) {
        Props.insert(z >> 1, new Property("y", "1", true));
        Props.prev();
      }
    }*/
    if(z == 6)  if(counts == 6)     // backward compatible
      if(Model == "R") {
        Props.getLast()->Value = n;
        return true;
      }
    p1->Value = n;

    n  = s.section('"',z,z);    // display
    p1->display = (n.at(1) == '1');
  }

	//PRADIS
	PRADISComponent::loadPRADISProperties(this,s,z);
	//PRADIS

  return true;
}

// *******************************************************************
// ***  The following functions are used to load the schematic symbol
// ***  from file. (e.g. subcircuit, library component)

int Component::analyseLine(const QString& Row, int numProps)
{
  /*QPen Pen;
  QBrush Brush;
  QColor Color;
  QString s;
  int i1, i2, i3, i4, i5, i6;

  s = Row.section(' ',0,0);    // component type
  if((s == "PortSym") || (s == ".PortSym")) {  // backward compatible
    if(!getIntegers(Row, &i1, &i2, &i3))
      return -1;
    for(i6 = Ports.count(); i6<i3; i6++)  // if ports not in numerical order
      Ports.append(new Port(0, 0));

    Ports.at(i3-1)->x  = i1;
    Ports.current()->y = i2;

    if(i1 < x1)  x1 = i1;  // keep track of component boundings
    if(i1 > x2)  x2 = i1;
    if(i2 < y1)  y1 = i2;
    if(i2 > y2)  y2 = i2;
    return 0;   // do not count Ports
  }
  else if(s == "Line") {
    if(!getIntegers(Row, &i1, &i2, &i3, &i4))  return -1;
    if(!getPen(Row, Pen, 5))  return -1;
    i3 += i1;
    i4 += i2;
    Lines.append(new Line(i1, i2, i3, i4, Pen));

    if(i1 < x1)  x1 = i1;  // keep track of component boundings
    if(i1 > x2)  x2 = i1;
    if(i2 < y1)  y1 = i2;
    if(i2 > y2)  y2 = i2;
    if(i3 < x1)  x1 = i3;
    if(i3 > x2)  x2 = i3;
    if(i4 < y1)  y1 = i4;
    if(i4 > y2)  y2 = i4;
    return 1;
  }
  else if(s == "EArc") {
    if(!getIntegers(Row, &i1, &i2, &i3, &i4, &i5, &i6))
      return -1;
    if(!getPen(Row, Pen, 7))  return -1;
    Arcs.append(new struct Arc(i1, i2, i3, i4, i5, i6, Pen));
/*		int k=0;
		int s;
		if (i6<0) s =-1; else s = 1;
		for (int alfa = i5; alfa* s<(i5+i6)* s; alfa = alfa + 160 * s)
		{
			k++;
			if (k>100) break;
			int x = i3 * cos (3.1415926*alfa/(16.0*180.0)) /2 + i1+i3/2;	
			int y = i4 * sin (3.1415926*alfa/(16.0*180.0)) /2 + i2+i4/2;
			if (x<x1) x1=x;
			if (x>x2) x2=x;
			if (y<y1) y1=y;
			if (y>y2) y2=y;
					
		}
*/

    /*if(i1 < x1)  x1 = i1;  // keep track of component boundings
    if(i1+i3 > x2)  x2 = i1+i3;
    if(i2 < y1)  y1 = i2;
    if(i2+i4 > y2)  y2 = i2+i4;



    return 1;
  }
  else if(s == ".ID") {
    if(!getIntegers(Row, &i1, &i2))  return -1;
    tx = i1;
    ty = i2;
    Name = Row.section(' ',3,3);
    if(Name.isEmpty())  Name = "SUB";

    i1 = 1;
    Property *pp = Props.at(numProps-1);
    for(;;) {
      s = Row.section('"', i1,i1);
      if(s.isEmpty())  break;

      pp = Props.next();
      if(pp == 0) {
        pp = new Property();
        Props.append(pp);

        pp->display = (s.at(0) == '1');
        pp->Value = s.section('=', 2,2);
      }

      pp->Name  = s.section('=', 1,1);
      pp->Description = s.section('=', 3,3);
      if(pp->Description.isEmpty())
        pp->Description = " ";

      i1 += 2;
    }

    while(pp != Props.last())
      Props.remove();
    return 0;   // do not count IDs
  }
  else if(s == "Arrow") {
    if(!getIntegers(Row, &i1, &i2, &i3, &i4, &i5, &i6))  return -1;
    if(!getPen(Row, Pen, 7))  return -1;

    double beta   = atan2(double(i6), double(i5));
    double phi    = atan2(double(i4), double(i3));
    double Length = sqrt(double(i6*i6 + i5*i5));

    i3 += i1;
    i4 += i2;
    if(i1 < x1)  x1 = i1;  // keep track of component boundings
    if(i1 > x2)  x2 = i1;
    if(i3 < x1)  x1 = i3;
    if(i3 > x2)  x2 = i3;
    if(i2 < y1)  y1 = i2;
    if(i2 > y2)  y2 = i2;
    if(i4 < y1)  y1 = i4;
    if(i4 > y2)  y2 = i4;

    Lines.append(new Line(i1, i2, i3, i4, Pen));   // base line

    double w = beta+phi;
    i5 = i3-int(Length*cos(w));
    i6 = i4-int(Length*sin(w));
    Lines.append(new Line(i3, i4, i5, i6, Pen)); // arrow head
    if(i5 < x1)  x1 = i5;  // keep track of component boundings
    if(i5 > x2)  x2 = i5;
    if(i6 < y1)  y1 = i6;
    if(i6 > y2)  y2 = i6;

    w = phi-beta;
    i5 = i3-int(Length*cos(w));
    i6 = i4-int(Length*sin(w));
    Lines.append(new Line(i3, i4, i5, i6, Pen));
    if(i5 < x1)  x1 = i5;  // keep track of component boundings
    if(i5 > x2)  x2 = i5;
    if(i6 < y1)  y1 = i6;
    if(i6 > y2)  y2 = i6;

    return 1;
  }
  else if(s == "Ellipse") {
    if(!getIntegers(Row, &i1, &i2, &i3, &i4))  return -1;
    if(!getPen(Row, Pen, 5))  return -1;
    if(!getBrush(Row, Brush, 8))  return -1;
    Ellips.append(new Area(i1, i2, i3, i4, Pen, Brush));

    if(i1 < x1)  x1 = i1;  // keep track of component boundings
    if(i1 > x2)  x2 = i1;
    if(i2 < y1)  y1 = i2;
    if(i2 > y2)  y2 = i2;
    if(i1+i3 < x1)  x1 = i1+i3;
    if(i1+i3 > x2)  x2 = i1+i3;
    if(i2+i4 < y1)  y1 = i2+i4;
    if(i2+i4 > y2)  y2 = i2+i4;
    return 1;
  }
  else if(s == "Rectangle") {
    if(!getIntegers(Row, &i1, &i2, &i3, &i4))  return -1;
    if(!getPen(Row, Pen, 5))  return -1;
    if(!getBrush(Row, Brush, 8))  return -1;
    Rects.append(new Area(i1, i2, i3, i4, Pen, Brush));

    if(i1 < x1)  x1 = i1;  // keep track of component boundings
    if(i1 > x2)  x2 = i1;
    if(i2 < y1)  y1 = i2;
    if(i2 > y2)  y2 = i2;
    if(i1+i3 < x1)  x1 = i1+i3;
    if(i1+i3 > x2)  x2 = i1+i3;
    if(i2+i4 < y1)  y1 = i2+i4;
    if(i2+i4 > y2)  y2 = i2+i4;
    return 1;
  }
  else if(s == "Text") {  // must be last in order to reuse "s" *********
    if(!getIntegers(Row, &i1, &i2, &i3, 0, &i4))  return -1;
    Color.setNamedColor(Row.section(' ',4,4));
    if(!Color.isValid()) return -1;

    s = Row.mid(Row.find('"')+1);    // Text (can contain " !!!)
    s = s.left(s.length()-1);
    if(s.isEmpty()) return -1;
    convert2Unicode(s);

    Texts.append(new Text(i1, i2, s, Color, float(i3),
                          float(cos(float(i4)*M_PI/180.0)),
                          float(sin(float(i4)*M_PI/180.0))));

    QFont Font(QucsSettings.font);
    Font.setPointSizeFloat(float(i3));
    QFontMetrics  metrics(Font);
    QSize r = metrics.size(0, s);    // get size of text
    i3 = i1 + int(float(r.width())  * Texts.current()->mCos)
            + int(float(r.height()) * Texts.current()->mSin);
    i4 = i2 + int(float(r.width())  * -Texts.current()->mSin)
            + int(float(r.height()) * Texts.current()->mCos);

    if(i1 < x1)  x1 = i1;  // keep track of component boundings
    if(i2 < y1)  y1 = i2;
    if(i1 > x2)  x2 = i1;
    if(i2 > y2)  y2 = i2;
    
    if(i3 < x1)  x1 = i3;
    if(i4 < y1)  y1 = i4;
    if(i3 > x2)  x2 = i3;
    if(i4 > y2)  y2 = i4;
    return 1;
  }
*/
  return 0;
}

// ---------------------------------------------------------------------
bool Component::getIntegers(const QString& s, int *i1, int *i2, int *i3,
			     int *i4, int *i5, int *i6)
{
  bool ok;
  QString n;

  if(!i1) return true;
  n  = s.section(' ',1,1);
  *i1 = n.toInt(&ok);
  if(!ok) return false;

  if(!i2) return true;
  n  = s.section(' ',2,2);
  *i2 = n.toInt(&ok);
  if(!ok) return false;

  if(!i3) return true;
  n  = s.section(' ',3,3);
  *i3 = n.toInt(&ok);
  if(!ok) return false;

  if(i4) {
    n  = s.section(' ',4,4);
    *i4 = n.toInt(&ok);
    if(!ok) return false;
  }

  if(!i5) return true;
  n  = s.section(' ',5,5);
  *i5 = n.toInt(&ok);
  if(!ok) return false;

  if(!i6) return true;
  n  = s.section(' ',6,6);
  *i6 = n.toInt(&ok);
  if(!ok) return false;

  return true;
}


// ---------------------------------------------------------------------
void Component::copyComponent(Component *pc)
{
  Type = pc->Type;
  x1 = pc->x1;
  y1 = pc->y1;
  x2 = pc->x2;
  y2 = pc->y2;

  Model = pc->Model;
  Name  = pc->Name;
  showName = pc->showName;
  Description = pc->Description;

  isActive = pc->isActive;
  rotated  = pc->rotated;
  mirroredX = pc->mirroredX;
  tx = pc->tx;
  ty = pc->ty;

  Props  = pc->Props;
  Ports  = pc->Ports;
  Lines  = pc->Lines;
  Arcs   = pc->Arcs;
  Rects  = pc->Rects;
  Ellips = pc->Ellips;
  Texts  = pc->Texts;
}


// ***********************************************************************
// ********                                                       ********
// ********          Functions of class MultiViewComponent        ********
// ********                                                       ********
// ***********************************************************************
void MultiViewComponent::recreate(Schematic *Doc)
{
  if(Doc) {
    Doc->Components->setAutoDelete(false);
  }

  Ellips.clear();
  Texts.clear();
  Ports.clear();
  Lines.clear();
  Rects.clear();
  Arcs.clear();
  createSymbol();
  
  bool mmir = mirroredX;
  int  rrot = rotated;

  rotated = rrot;   // restore properties (were changed by rotate/mirror)
  mirroredX = mmir;

  if(Doc) {
    Doc->insertRawComponent(this);
    Doc->Components->setAutoDelete(true);
  }
}




// ***********************************************************************
// ********                                                       ********
// ******** The following function does not below to any class.   ********
// ******** It creates a component by getting the identification  ********
// ******** string used in the schematic file and for copy/paste. ********
// ********                                                       ********
// ***********************************************************************
/*
Component* getComponentFromName(QString& Line)
{
  Component *c = 0;

  Line = Line.stripWhiteSpace();
  if(Line.at(0) != '<') {
    QMessageBox::critical(0, QObject::tr("Error"),
			QObject::tr("Format Error:\nWrong line start!"));
    return 0;
  }

  QString cstr = Line.section(' ',0,0); // component type
  char first = Line.at(1).latin1();     // first letter of component name
  cstr.remove(0,2);    // remove leading "<" and first letter

  // to speed up the string comparision, they are ordered by the first
  // letter of their name
  switch(first) {
  case 'R' : if(cstr.isEmpty()) c = new Resistor();
	else if(cstr == "us") c = new Resistor(false);  // backward capatible
	else if(cstr == "SFF") c = new RS_FlipFlop();
	else if(cstr == "elais") c = new Relais();
	break;
  case 'C' : if(cstr.isEmpty()) c = new Capacitor();
	else if(cstr == "CCS") c = new CCCS();
	else if(cstr == "CVS") c = new CCVS();
	else if(cstr == "irculator") c = new Circulator();
	else if(cstr == "oupler") c = new Coupler();
	else if(cstr == "LIN") c = new Coplanar();
	else if(cstr == "OPEN") c = new CPWopen();
	else if(cstr == "SHORT") c = new CPWshort();
	else if(cstr == "GAP") c = new CPWgap();
	else if(cstr == "STEP") c = new CPWstep();
	else if(cstr == "OAX") c = new CoaxialLine();
	break;
  case 'L' : if(cstr.isEmpty()) c = new Inductor();
	else if(cstr == "ib") c = new LibComp();
	break;
  case 'G' : if(cstr == "ND") c = new Ground();
        else if(cstr == "yrator") c = new Gyrator();
        break;
  case 'I' : if(cstr == "Probe") c = new iProbe();
        else if(cstr == "dc") c = new Ampere_dc();
        else if(cstr == "ac") c = new Ampere_ac();
        else if(cstr == "noise") c = new Ampere_noise();
        else if(cstr == "solator") c = new Isolator();
        else if(cstr == "pulse") c = new iPulse();
        else if(cstr == "rect") c = new iRect();
        else if(cstr == "Inoise") c = new Noise_ii();
        else if(cstr == "Vnoise") c = new Noise_iv();
        else if(cstr == "nv") c = new Logical_Inv();
        else if(cstr == "exp") c = new iExp();
        break;
  case 'J' : if(cstr == "FET") c = new JFET();
	else if(cstr == "KFF") c = new JK_FlipFlop();
        break;
  case 'V' : if(cstr == "dc") c = new Volt_dc();
        else if(cstr == "ac") c = new Volt_ac();
        else if(cstr == "CCS") c = new VCCS();
        else if(cstr == "CVS") c = new VCVS();
        else if(cstr == "Probe") c = new vProbe();
        else if(cstr == "noise") c = new Volt_noise();
        else if(cstr == "pulse") c = new vPulse();
        else if(cstr == "rect") c = new vRect();
        else if(cstr == "Vnoise") c = new Noise_vv();
        else if(cstr == "HDL") c = new VHDL_File();
        else if(cstr == "erilog") c = new Verilog_File();
        else if(cstr == "exp") c = new vExp();
        break;
  case 'T' : if(cstr == "r") c = new Transformer();
        else if(cstr == "LIN") c = new TLine();
        else if(cstr == "LIN4P") c = new TLine_4Port();
        else if(cstr == "WIST") c = new TwistedPair();
        break;
  case 's' : if(cstr == "Tr") c = new symTrafo();
        break;
  case 'P' : if(cstr == "ac") c = new Source_ac();
        else if(cstr == "ort") c = new SubCirPort();
        else if(cstr == "Shift") c = new Phaseshifter();
        else if(cstr == "M_Mod") c = new PM_Modulator();
        break;
  case 'S' : if(cstr == "Pfile") c = new SParamFile();
        else if(cstr.left(5) == "Pfile") {  // backward compatible
          c = new SParamFile();
          c->Props.getLast()->Value = cstr.mid(5); }
        else if(cstr == "ub")   c = new Subcircuit();
        else if(cstr == "UBST") c = new Substrate();
        else if(cstr == "PICE") c = new SpiceFile();
        else if(cstr == "witch") c = new Switch();
        break;
  case 'D' : if(cstr == "CBlock") c = new dcBlock();
	else if(cstr == "CFeed") c = new dcFeed();
	else if(cstr == "iode") c = new Diode();
	else if(cstr == "igiSource") c = new Digi_Source();
	else if(cstr == "FF") c = new D_FlipFlop();
	break;
  case 'B' : if(cstr == "iasT") c = new BiasT();
        else if(cstr == "JT") c = new BJTsub();
        else if(cstr == "OND") c = new BondWire();
        break;
  case 'A' : if(cstr == "ttenuator") c = new Attenuator();
        else if(cstr == "mp") c = new Amplifier();
        else if(cstr == "ND") c = new Logical_AND();
        else if(cstr == "M_Mod") c = new AM_Modulator();
        break;
  case 'M' : if(cstr == "UT") c = new Mutual();
	else if(cstr == "UT2") c = new Mutual2();
	else if(cstr == "LIN") c = new MSline();
	else if(cstr == "OSFET") c = new MOSFET_sub();
	else if(cstr == "STEP") c = new MSstep();
	else if(cstr == "CORN") c = new MScorner();
	else if(cstr == "TEE") c = new MStee();
	else if(cstr == "CROSS") c = new MScross();
	else if(cstr == "MBEND") c = new MSmbend();
	else if(cstr == "OPEN") c = new MSopen();
	else if(cstr == "GAP") c = new MSgap();
	else if(cstr == "COUPLED") c = new MScoupled();
	else if(cstr == "VIA") c = new MSvia();
	break;
  case 'E' : if(cstr == "qn") c = new Equation();
	else if(cstr == "DD") c = new EqnDefined();
        break;
  case 'O' : if(cstr == "pAmp") c = new OpAmp();
        else if(cstr == "R") c = new Logical_OR();
        break;
  case 'N' : if(cstr == "OR") c = new Logical_NOR();
        else if(cstr == "AND") c = new Logical_NAND();
        break;
  case '.' : if(cstr == "DC") c = new DC_Sim();
        else if(cstr == "AC") c = new AC_Sim();
        else if(cstr == "TR") c = new TR_Sim();
        else if(cstr == "SP") c = new SP_Sim();
        else if(cstr == "HB") c = new HB_Sim();
        else if(cstr == "SW") c = new Param_Sweep();
        else if(cstr == "Digi") c = new Digi_Sim();
        else if(cstr == "Opt") c = new Optimize_Sim();
        break;
  case '_' : if(cstr == "BJT") c = new BJT();
	else if(cstr == "MOSFET") c = new MOSFET();
        break;
  case 'X' : if(cstr == "OR") c = new Logical_XOR();
        else if(cstr == "NOR") c = new Logical_XNOR();
        break;
  case 'h' : if(cstr == "icumL2p1") c = new hicumL2p1();
        break;
  case 'H' : if(cstr == "BT_X") c = new HBT_X();
        break;
  }
  if(!c) {
    QMessageBox::critical(0, QObject::tr("Error"),
	QObject::tr("Format Error:\nUnknown component!"));
    return 0;
  }

  if(!c->load(Line)) {
    QMessageBox::critical(0, QObject::tr("Error"),
	QObject::tr("Format Error:\nWrong 'component' line format!"));
    delete c;
    return 0;
  }

  cstr = c->Name;   // is perhaps changed in "recreate" (e.g. subcircuit)
  int x = c->tx, y = c->ty;
  c->recreate(0);
  c->Name = cstr;
  c->tx = x;  c->ty = y;
  return c;
}
*/