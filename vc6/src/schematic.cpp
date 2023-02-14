/***************************************************************************
                              schematic.cpp
                             ---------------
    begin                : Sat Mar 3 2006
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <limits.h>

//#include <qimage.h>
//#include <qaction.h>
//#include <qregexp.h>
//#include <qiconset.h>
//#include <qprinter.h>
//#include <qlineedit.h>
//#include <qfileinfo.h>
//#include <qiconview.h>
//#include <qtabwidget.h>
//#include <qdragobject.h>
//#include <qpaintdevicemetrics.h>
//#include <qdir.h>

#include <qmessagebox.h>

//#include "qucs.h"
#include "main.h"
//#include "node.h"
#include "schematic.h"
//#include "viewpainter.h"
//#include "mouseactions.h"
#include "diagrams/diagrams.h"
#include "paintings/paintings.h"
#include "pradis/PRADISGround.h"

// just dummies for empty lists
QPtrList<Wire>      SymbolWires;
QPtrList<Node>      SymbolNodes;
QPtrList<Diagram>   SymbolDiags;
QPtrList<Component> SymbolComps;


Schematic::Schematic(const QString& Name_)
    //: QucsDoc(App_, Name_)
{
//  symbolMode = false;
  DocName = Name_;
  Scale = 1.0;
  GridOn = true;


	QFileInfo Info(DocName);
	  if(!DocName.isEmpty()) {
		DocName = Info.absFilePath();

		DataSet = Info.baseName()+".dat";   // name of the default dataset
		if(Info.extension(false) != "dpl")
		  DataDisplay = Info.baseName()+".dpl"; // name of default data display
		else {
		  DataDisplay = Info.baseName()+".sch"; // name of default schematic
		  GridOn = false;     // data display without grid (per default)
		}
	  }


  // ...........................................................
  GridX  = GridY  = 10;
  ViewX1=ViewY1=0;
  ViewX2=ViewY2=800;
  UsedX1 = UsedY1 = INT_MAX;
  UsedX2 = UsedY2 = INT_MIN;


  DocComps.setAutoDelete(true);
  DocWires.setAutoDelete(true);
  DocNodes.setAutoDelete(true);
  DocDiags.setAutoDelete(true);
  DocPaints.setAutoDelete(true);
  SymbolPaints.setAutoDelete(true);

  isVerilog = false;
  creatingLib = false;
  SimOpenDpl = true;
}

Schematic::~Schematic()
{
}

// ---------------------------------------------------
// Loads this Qucs document.
bool Schematic::load()
{
  DocComps.clear();
  DocWires.clear();
  DocNodes.clear();
  DocDiags.clear();
  DocPaints.clear();
  SymbolPaints.clear();


  if(!loadDocument()) return false;
  
  return true;
}

// ---------------------------------------------------
// Saves this Qucs document. Returns the number of subcircuit ports.
int Schematic::save()
{
  /*int result = adjustPortNumbers();// same port number for schematic and symbol
  if(saveDocument() < 0)
     return -1;
  lastSaved = QDateTime::currentDateTime();
  if(result >= 0) {
    setChanged(false);
    QString *p, *ps = UndoStack.current();
    for(p = UndoStack.first(); p != 0; p = UndoStack.next())
      p->at(1) = ' ';  // state of being changed
    ps->at(1) = 'i';   // state of being unchanged
    UndoStack.findRef(ps);  // back to current

    ps = UndoSymbol.current();
    for(p = UndoSymbol.first(); p != 0; p = UndoSymbol.next())
      p->at(1) = ' ';  // state of being changed
    ps->at(1) = 'i';   // state of being unchanged
    UndoSymbol.findRef(ps);  // back to current
  }*/
  return 0;//result;
}

// Returns the number of subcircuit ports.
int Schematic::saveDocument()
{
  /*QFile file(DocName);
  if(!file.open(IO_WriteOnly)) {
    QMessageBox::critical(0, QObject::tr("Error"),
				QObject::tr("Cannot save document!"));
    return -1;
  }

  QTextStream stream(&file);

  stream << "<Qucs Schematic " << PACKAGE_VERSION << ">\n";

  stream << "<Properties>\n";
  if(symbolMode) {
    stream << "  <View=" << tmpViewX1<<","<<tmpViewY1<<","
			 << tmpViewX2<<","<<tmpViewY2<< ",";
    stream <<tmpScale<<","<<tmpPosX<<","<<tmpPosY << ">\n";
  }
  else {
    stream << "  <View=" << ViewX1<<","<<ViewY1<<","
			 << ViewX2<<","<<ViewY2<< ",";
    stream << Scale <<","<<contentsX()<<","<<contentsY() << ">\n";
  }
  stream << "  <Grid=" << GridX<<","<<GridY<<","
			<< GridOn << ">\n";
  stream << "  <DataSet=" << DataSet << ">\n";
  stream << "  <DataDisplay=" << DataDisplay << ">\n";
  stream << "  <OpenDisplay=" << SimOpenDpl << ">\n";
  stream << "  <showFrame=" << showFrame << ">\n";

  QString t;
  convert2ASCII(t = Frame_Text0);
  stream << "  <FrameText0=" << t << ">\n";
  convert2ASCII(t = Frame_Text1);
  stream << "  <FrameText1=" << t << ">\n";
  convert2ASCII(t = Frame_Text2);
  stream << "  <FrameText2=" << t << ">\n";
  convert2ASCII(t = Frame_Text3);
  stream << "  <FrameText3=" << t << ">\n";
  stream << "</Properties>\n";

  Painting *pp;
  stream << "<Symbol>\n";     // save all paintings for symbol
  for(pp = SymbolPaints.first(); pp != 0; pp = SymbolPaints.next())
    stream << "  <" << pp->save() << ">\n";
  stream << "</Symbol>\n";

  stream << "<Components>\n";    // save all components
  for(Component *pc = DocComps.first(); pc != 0; pc = DocComps.next())
    stream << "  " << pc->save() << "\n";
  stream << "</Components>\n";

  stream << "<Wires>\n";    // save all wires
  for(Wire *pw = DocWires.first(); pw != 0; pw = DocWires.next())
    stream << "  " << pw->save() << "\n";

  // save all labeled nodes as wires
  for(Node *pn = DocNodes.first(); pn != 0; pn = DocNodes.next())
    if(pn->Label) stream << "  " << pn->Label->save() << "\n";
  stream << "</Wires>\n";

  stream << "<Diagrams>\n";    // save all diagrams
  for(Diagram *pd = DocDiags.first(); pd != 0; pd = DocDiags.next())
    stream << "  " << pd->save() << "\n";
  stream << "</Diagrams>\n";

  stream << "<Paintings>\n";     // save all paintings
  for(pp = DocPaints.first(); pp != 0; pp = DocPaints.next())
    stream << "  <" << pp->save() << ">\n";
  stream << "</Paintings>\n";

  file.close();*/
  return 0;
}

// -------------------------------------------------------------
bool Schematic::loadProperties(QTextStream *stream)
{
  bool ok = true;
  QString Line, cstr, nstr;
  while(!stream->atEnd()) {
    Line = stream->readLine();
    if(Line.at(0) == '<') if(Line.at(1) == '/') return true;  // field end ?
    Line = Line.stripWhiteSpace();
    if(Line.isEmpty()) continue;

    if(Line.at(0) != '<') {
      QMessageBox::critical(0, QObject::tr("Error"),
		QObject::tr("Format Error:\nWrong property field limiter!"));
      return false;
    }
    if(Line.at(Line.length()-1) != '>') {
      QMessageBox::critical(0, QObject::tr("Error"),
		QObject::tr("Format Error:\nWrong property field limiter!"));
      return false;
    }
    Line = Line.mid(1, Line.length()-2);   // cut off start and end character

    cstr = Line.section('=',0,0);    // property type
    nstr = Line.section('=',1,1);    // property value
         if(cstr == "View") {
		ViewX1 = nstr.section(',',0,0).toInt(&ok); if(ok) {
		ViewY1 = nstr.section(',',1,1).toInt(&ok); if(ok) {
		ViewX2 = nstr.section(',',2,2).toInt(&ok); if(ok) {
		ViewY2 = nstr.section(',',3,3).toInt(&ok); if(ok) {
		Scale  = nstr.section(',',4,4).toDouble(&ok); if(ok) {
		tmpViewX1 = nstr.section(',',5,5).toInt(&ok); if(ok)
		tmpViewY1 = nstr.section(',',6,6).toInt(&ok); }}}}} }
    else if(cstr == "Grid") {
		GridX = nstr.section(',',0,0).toInt(&ok); if(ok) {
		GridY = nstr.section(',',1,1).toInt(&ok); if(ok) {
		if(nstr.section(',',2,2).toInt(&ok) == 0) GridOn = false;
		else GridOn = true; }} }
    else if(cstr == "DataSet") DataSet = nstr;
    else if(cstr == "DataDisplay") DataDisplay = nstr;
    else if(cstr == "OpenDisplay")
		if(nstr.toInt(&ok) == 0) SimOpenDpl = false;
		else SimOpenDpl = true;
    else if(cstr == "showFrame")
		showFrame = nstr.at(0).latin1() - '0';
    else if(cstr == "FrameText0") convert2Unicode(Frame_Text0 = nstr);
    else if(cstr == "FrameText1") convert2Unicode(Frame_Text1 = nstr);
    else if(cstr == "FrameText2") convert2Unicode(Frame_Text2 = nstr);
    else if(cstr == "FrameText3") convert2Unicode(Frame_Text3 = nstr);
    else {
      QMessageBox::critical(0, QObject::tr("Error"),
	   QObject::tr("Format Error:\nUnknown property: ")+cstr);
      return false;
    }
    if(!ok) {
      QMessageBox::critical(0, QObject::tr("Error"),
	   QObject::tr("Format Error:\nNumber expected in property field!"));
      return false;
    }
  }

  QMessageBox::critical(0, QObject::tr("Error"),
               QObject::tr("Format Error:\n'Property' field is not closed!"));
  return false;
}

// ---------------------------------------------------
// Inserts a component without performing logic for wire optimization.
void Schematic::simpleInsertComponent(Component *c)
{
  Node *pn;
  int x, y;
  // connect every node of component
  for(Port *pp = c->Ports.first(); pp != 0; pp = c->Ports.next()) {
    x = pp->x+c->cx;
    y = pp->y+c->cy;

    // check if new node lies upon existing node
    for(pn = DocNodes.first(); pn != 0; pn = DocNodes.next())
      if(pn->cx == x) if(pn->cy == y)  break;

    if(pn == 0) { // create new node, if no existing one lies at this position
      pn = new Node(x, y);
      DocNodes.append(pn);
    }
    pn->Connections.append(c);  // connect schematic node to component node

    pp->Connection = pn;  // connect component node to schematic node
  }

  DocComps.append(c);
}

// -------------------------------------------------------------
bool Schematic::loadComponents(QTextStream *stream, QPtrList<Component> *List)
{
  QString Line, cstr;
  Component *c;
  while(!stream->atEnd()) {
    Line = stream->readLine();
    if(Line.at(0) == '<') if(Line.at(1) == '/') return true;
    Line = Line.stripWhiteSpace();
    if(Line.isEmpty()) continue;

    c = getComponentFromName(Line);
//>PRA-0
//    if(!c) return false;
    if(!c) continue;

    if(List) {  // "paste" ?
      int z;
      for(z=c->Name.length()-1; z>=0; z--) // cut off number of component name
        if(!c->Name.at(z).isDigit()) break;
      c->Name = c->Name.left(z+1);
      List->append(c);
    }
    else  simpleInsertComponent(c);
  }

  QMessageBox::critical(0, QObject::tr("Error"),
	   QObject::tr("Format Error:\n'Component' field is not closed!"));
  return false;
}


Component* getPRADISComponentFromName(QString& model)
{	
	PRADISComponent* pc = new PRADISComponent;
	pc->Model = model;
	return pc;
}




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
//  char first = Line.at(1).latin1();     // first letter of component name
  cstr.remove(0,1);    // remove leading "<" and first letter
	
	c = getPRADISComponentFromName(cstr);
	
	
	
	if(c)
	{
		c = c->newOne();
	}	
	if(cstr=="GND")
	{
		c = new PRADISGround();
	}

	c->Model = cstr;
	
	//QMessageBox::information(0, "info", c->Model);



  if(!c) {
    QMessageBox::critical(0, QObject::tr("Error"),
	QObject::tr("Format Error:\nUnknown component!")+"\n"+cstr);
    return 0;
  }

  if(!c->load(Line)) {
    QMessageBox::critical(0, QObject::tr("Error"),
	QObject::tr("Format Error:\nWrong 'component' line format!")+"\n"+cstr);
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


// -------------------------------------------------------------
// Inserts a wire without performing logic for optimizing.
void Schematic::simpleInsertWire(Wire *pw)
{
  Node *pn;
  // check if first wire node lies upon existing node
  for(pn = DocNodes.first(); pn != 0; pn = DocNodes.next())
    if(pn->cx == pw->x1) if(pn->cy == pw->y1) break;

  if(!pn) {   // create new node, if no existing one lies at this position
    pn = new Node(pw->x1, pw->y1);
    DocNodes.append(pn);
  }

  if(pw->x1 == pw->x2) if(pw->y1 == pw->y2) {
    pn->Label = pw->Label;   // wire with length zero are just node labels
    if (pn->Label) {
      pn->Label->Type = isNodeLabel;
      pn->Label->pOwner = pn;
    }
    delete pw;           // delete wire because this is not a wire
    return;
  }
  pn->Connections.append(pw);  // connect schematic node to component node
  pw->Port1 = pn;

  // check if second wire node lies upon existing node
  for(pn = DocNodes.first(); pn != 0; pn = DocNodes.next())
    if(pn->cx == pw->x2) if(pn->cy == pw->y2) break;

  if(!pn) {   // create new node, if no existing one lies at this position
    pn = new Node(pw->x2, pw->y2);
    DocNodes.append(pn);
  }
  pn->Connections.append(pw);  // connect schematic node to component node
  pw->Port2 = pn;

  DocWires.append(pw);
}

// -------------------------------------------------------------
bool Schematic::loadWires(QTextStream *stream, QPtrList<Element> *List)
{
  Wire *w;
  QString Line;
  while(!stream->atEnd()) {
    Line = stream->readLine();
    if(Line.at(0) == '<') if(Line.at(1) == '/') return true;
    Line = Line.stripWhiteSpace();
    if(Line.isEmpty()) continue;

    // (Node*)4 =  move all ports (later on)
    w = new Wire(0,0,0,0, (Node*)4,(Node*)4);
    if(!w->load(Line)) {
      QMessageBox::critical(0, QObject::tr("Error"),
		QObject::tr("Format Error:\nWrong 'wire' line format!"));
      delete w;
      return false;
    }
    if(List) {
      if(w->x1 == w->x2) if(w->y1 == w->y2) if(w->Label) {
	w->Label->Type = isMovingLabel;
	List->append(w->Label);
	delete w;
	continue;
      }
      List->append(w);
      if(w->Label)  List->append(w->Label);
    }
    else simpleInsertWire(w);
  }

  QMessageBox::critical(0, QObject::tr("Error"),
		QObject::tr("Format Error:\n'Wire' field is not closed!"));
  return false;
}

// -------------------------------------------------------------
bool Schematic::loadDiagrams(QTextStream *stream, QPtrList<Diagram> *List)
{
  Diagram *d;
  QString Line, cstr;
  while(!stream->atEnd()) {
    Line = stream->readLine();
    if(Line.at(0) == '<') if(Line.at(1) == '/') return true;
    Line = Line.stripWhiteSpace();
    if(Line.isEmpty()) continue;

    cstr = Line.section(' ',0,0);    // diagram type
         if(cstr == "<Rect") d = new RectDiagram();
    else if(cstr == "<Polar") d = new PolarDiagram();
    else if(cstr == "<Tab") d = new TabDiagram();
    else if(cstr == "<Smith") d = new SmithDiagram();
    else if(cstr == "<ySmith") d = new SmithDiagram(0,0,false);
    else if(cstr == "<PS") d = new PSDiagram();
    else if(cstr == "<SP") d = new PSDiagram(0,0,false);
    else if(cstr == "<Rect3D") d = new Rect3DDiagram();
    else if(cstr == "<Curve") d = new CurveDiagram();
    else if(cstr == "<Time") d = new TimingDiagram();
    else if(cstr == "<Truth") d = new TruthDiagram();
    else {
      QMessageBox::critical(0, QObject::tr("Error"),
		   QObject::tr("Format Error:\nUnknown diagram!"));
      return false;
    }

    if(!d->load(Line, stream)) {
      QMessageBox::critical(0, QObject::tr("Error"),
		QObject::tr("Format Error:\nWrong 'diagram' line format!"));
      delete d;
      return false;
    }
    List->append(d);
  }

  QMessageBox::critical(0, QObject::tr("Error"),
	       QObject::tr("Format Error:\n'Diagram' field is not closed!"));
  return false;
}

// -------------------------------------------------------------
bool Schematic::loadPaintings(QTextStream *stream, QPtrList<Painting> *List)
{
  Painting *p=0;
  QString Line, cstr;
  while(!stream->atEnd()) {
    Line = stream->readLine();
    if(Line.at(0) == '<') if(Line.at(1) == '/') return true;

    Line = Line.stripWhiteSpace();
    if(Line.isEmpty()) continue;
    if( (Line.at(0) != '<') || (Line.at(Line.length()-1) != '>')) {
      QMessageBox::critical(0, QObject::tr("Error"),
	QObject::tr("Format Error:\nWrong 'painting' line delimiter!"));
      return false;
    }
    Line = Line.mid(1, Line.length()-2);  // cut off start and end character

    cstr = Line.section(' ',0,0);    // painting type
         if(cstr == "Line") p = new GraphicLine();
    else if(cstr == "EArc") p = new EllipseArc();
    else if(cstr == ".PortSym") p = new PortSymbol();
    else if(cstr == ".ID") p = new ID_Text();
    else if(cstr == "Text") p = new GraphicText();
    else if(cstr == "Rectangle") p = new Rectangle();
    else if(cstr == "Arrow") p = new Arrow();
    else if(cstr == "Ellipse") p = new Ellipse();
    else {
      QMessageBox::critical(0, QObject::tr("Error"),
		QObject::tr("Format Error:\nUnknown painting!"));
      return false;
    }

    if(!p->load(Line)) {
      QMessageBox::critical(0, QObject::tr("Error"),
	QObject::tr("Format Error:\nWrong 'painting' line format!"));
      delete p;
      return false;
    }
    List->append(p);
  }

  QMessageBox::critical(0, QObject::tr("Error"),
	QObject::tr("Format Error:\n'Painting' field is not closed!"));
  return false;
}

// -------------------------------------------------------------
bool Schematic::loadDocument()
{
  QFile file(DocName);
  if(!file.open(IO_ReadOnly)) {
    QMessageBox::critical(0, QObject::tr("Error"),
                 QObject::tr("Cannot load document: ")+DocName);
    return false;
  }

  QString Line;
  QTextStream stream(&file);



  // read header **************************
  do {
    if(stream.atEnd()) {
      file.close();
      return true;
    }

    Line = stream.readLine();
  } while(Line.isEmpty());



  if(Line.left(16) != "<Qucs Schematic ") {  // wrong file type ?
    file.close();
    QMessageBox::critical(0, QObject::tr("Error"),
 		 QObject::tr("Wrong document type: ")+DocName);
    return false;
  }

  Line = Line.mid(16, Line.length()-17);
  if(!checkVersion(Line)) { // wrong version number ?
    file.close();
    QMessageBox::critical(0, QObject::tr("Error"),
		 QObject::tr("Wrong document version: ")+Line);
    return false;
  }



  // read content *************************
  while(!stream.atEnd()) {
    Line = stream.readLine();
    Line = Line.stripWhiteSpace();
    if(Line.isEmpty()) continue;
	
	//QMessageBox::information(0, "Line is", Line);
		

    if(Line == "<Symbol>") {
			try{
      if(!loadPaintings(&stream, &SymbolPaints)) {
	file.close();
	return false;
      }
			}catch(...){}
    }
    else
    if(Line == "<Properties>") {
			try{
      if(!loadProperties(&stream)) { file.close(); return false; } 
			}catch(...){}
			}
    else
    if(Line == "<Components>") {
			try{
      if(!loadComponents(&stream)) { file.close(); return false; } 
		}catch(...){}
		}
    else
    if(Line == "<Wires>") {
			try{
      if(!loadWires(&stream)) { file.close(); return false; } 
			}catch(...){}
			}
    else
    if(Line == "<Diagrams>") {
      if(!loadDiagrams(&stream, &DocDiags)) { file.close(); return false; } 
		}
			
    else
    if(Line == "<Paintings>") {
      if(!loadPaintings(&stream, &DocPaints)) { file.close(); return false; } 
			}
    else {
      QMessageBox::critical(0, QObject::tr("Error"),
		   QObject::tr("File Format Error:\nUnknown field!"));
      file.close();
      return false;
    }
  }



  file.close();
  return true;
}



