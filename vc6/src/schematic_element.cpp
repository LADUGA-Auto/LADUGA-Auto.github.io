/***************************************************************************
                            schematic_element.cpp
                           -----------------------
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

#include <stdlib.h>
#include <limits.h>

#include "schematic.h"
#include "node.h"
#include "wire.h"
#include "diagrams/diagram.h"
#include "paintings/painting.h"
#include "components/component.h"



/* *******************************************************************
   *****                                                         *****
   *****              Actions handling the nodes                 *****
   *****                                                         *****
   ******************************************************************* */

// Inserts a port into the schematic and connects it to another node if
// the coordinates are identical. The node is returned.
Node* Schematic::insertNode(int x, int y, Element *e)
{
  Node *pn;
  // check if new node lies upon existing node
  for(pn = Nodes->first(); pn != 0; pn = Nodes->next())  // check every node
    if(pn->cx == x) if(pn->cy == y) {
      pn->Connections.append(e);
      break;
    }

  if(pn == 0) { // create new node, if no existing one lies at this position
    pn = new Node(x, y);
    Nodes->append(pn);
    pn->Connections.append(e);  // connect schematic node to component node
  }
  else return pn;   // return, if node is not new

  // check if the new node lies upon an existing wire
  for(Wire *pw = Wires->first(); pw != 0; pw = Wires->next()) {
    if(pw->x1 == x) {
      if(pw->y1 > y) continue;
      if(pw->y2 < y) continue;
    }
    else if(pw->y1 == y) {
      if(pw->x1 > x) continue;
      if(pw->x2 < x) continue;
    }
    else continue;

    // split the wire into two wires
    splitWire(pw, pn);
    return pn;
  }

  return pn;
}


/* *******************************************************************
   *****                                                         *****
   *****              Actions handling the wires                 *****
   *****                                                         *****
   ******************************************************************* */

// Inserts a port into the schematic and connects it to another node if the
// coordinates are identical. If 0 is returned, no new wire is inserted.
// If 2 is returned, the wire line ended.
int Schematic::insertWireNode1(Wire *w)
{
  Node *pn;
  // check if new node lies upon an existing node
  for(pn = Nodes->first(); pn != 0; pn = Nodes->next()) // check every node
    if(pn->cx == w->x1) if(pn->cy == w->y1) break;

  if(pn != 0) {
    pn->Connections.append(w);
    w->Port1 = pn;
    return 2;   // node is not new
  }



  // check if the new node lies upon an existing wire
  for(Wire *ptr2 = Wires->first(); ptr2 != 0; ptr2 = Wires->next()) {
    if(ptr2->x1 == w->x1) {
      if(ptr2->y1 > w->y1) continue;
      if(ptr2->y2 < w->y1) continue;

      if(ptr2->isHorizontal() == w->isHorizontal()) // (ptr2-wire is vertical)
        if(ptr2->y2 >= w->y2) {
	  delete w;    // new wire lies within an existing wire
	  return 0; }
        else {
	  // one part of the wire lies within an existing wire
	  // the other part not
          if(ptr2->Port2->Connections.count() == 1) {
            w->y1 = ptr2->y1;
            w->Port1 = ptr2->Port1;
	    if(ptr2->Label) {
	      w->Label = ptr2->Label;
	      w->Label->pOwner = w;
	    }
            ptr2->Port1->Connections.removeRef(ptr2);  // two -> one wire
            ptr2->Port1->Connections.append(w);
            Nodes->removeRef(ptr2->Port2);
            Wires->removeRef(ptr2);
            return 2;
          }
          else {
            w->y1 = ptr2->y2;
            w->Port1 = ptr2->Port2;
            ptr2->Port2->Connections.append(w);   // shorten new wire
            return 2;
          }
        }
    }
    else if(ptr2->y1 == w->y1) {
      if(ptr2->x1 > w->x1) continue;
      if(ptr2->x2 < w->x1) continue;

      if(ptr2->isHorizontal() == w->isHorizontal()) // (ptr2-wire is horizontal)
        if(ptr2->x2 >= w->x2) {
          delete w;   // new wire lies within an existing wire
          return 0;
        }
        else {
	  // one part of the wire lies within an existing wire
	  // the other part not
          if(ptr2->Port2->Connections.count() == 1) {
            w->x1 = ptr2->x1;
            w->Port1 = ptr2->Port1;
	    if(ptr2->Label) {
	      w->Label = ptr2->Label;
	      w->Label->pOwner = w;
	    }
            ptr2->Port1->Connections.removeRef(ptr2); // two -> one wire
            ptr2->Port1->Connections.append(w);
            Nodes->removeRef(ptr2->Port2);
            Wires->removeRef(ptr2);
            return 2;
          }
          else {
            w->x1 = ptr2->x2;
            w->Port1 = ptr2->Port2;
            ptr2->Port2->Connections.append(w);   // shorten new wire
            return 2;
          }
        }
    }
    else continue;

    pn = new Node(w->x1, w->y1);   // create new node
    Nodes->append(pn);
    pn->Connections.append(w);  // connect schematic node to the new wire
    w->Port1 = pn;

    // split the wire into two wires
    splitWire(ptr2, pn);
    return 2;
  }

  pn = new Node(w->x1, w->y1);   // create new node
  Nodes->append(pn);
  pn->Connections.append(w);  // connect schematic node to the new wire
  w->Port1 = pn;
  return 1;
}

// ---------------------------------------------------
// if possible, connect two horizontal wires to one

// ---------------------------------------------------
// Inserts a port into the schematic and connects it to another node if the
// coordinates are identical. If 0 is returned, no new wire is inserted.
// If 2 is returned, the wire line ended.
int Schematic::insertWireNode2(Wire *w)
{
  Node *pn;
  // check if new node lies upon an existing node
  for(pn = Nodes->first(); pn != 0; pn = Nodes->next())  // check every node
    if(pn->cx == w->x2) if(pn->cy == w->y2) break;

  if(pn != 0) {
    pn->Connections.append(w);
    w->Port2 = pn;
    return 2;   // node is not new
  }



  // check if the new node lies upon an existing wire
  for(Wire *ptr2 = Wires->first(); ptr2 != 0; ptr2 = Wires->next()) {
    if(ptr2->x1 == w->x2) {
      if(ptr2->y1 > w->y2) continue;
      if(ptr2->y2 < w->y2) continue;

    // (if new wire lies within an existing wire, was already check before)
      if(ptr2->isHorizontal() == w->isHorizontal()) // ptr2-wire is vertical
          // one part of the wire lies within an existing wire
          // the other part not
          if(ptr2->Port1->Connections.count() == 1) {
	    if(ptr2->Label) {
	      w->Label = ptr2->Label;
	      w->Label->pOwner = w;
	    }
            w->y2 = ptr2->y2;
            w->Port2 = ptr2->Port2;
            ptr2->Port2->Connections.removeRef(ptr2);  // two -> one wire
            ptr2->Port2->Connections.append(w);
            Nodes->removeRef(ptr2->Port1);
            Wires->removeRef(ptr2);
            return 2;
          }
          else {
            w->y2 = ptr2->y1;
            w->Port2 = ptr2->Port1;
            ptr2->Port1->Connections.append(w);   // shorten new wire
            return 2;
          }
    }
    else if(ptr2->y1 == w->y2) {
      if(ptr2->x1 > w->x2) continue;
      if(ptr2->x2 < w->x2) continue;

    // (if new wire lies within an existing wire, was already check before)
      if(ptr2->isHorizontal() == w->isHorizontal()) // ptr2-wire is horizontal
          // one part of the wire lies within an existing wire
          // the other part not
          if(ptr2->Port1->Connections.count() == 1) {
	    if(ptr2->Label) {
	      w->Label = ptr2->Label;
	      w->Label->pOwner = w;
	    }
            w->x2 = ptr2->x2;
            w->Port2 = ptr2->Port2;
            ptr2->Port2->Connections.removeRef(ptr2);  // two -> one wire
            ptr2->Port2->Connections.append(w);
            Nodes->removeRef(ptr2->Port1);
            Wires->removeRef(ptr2);
            return 2;
          }
          else {
            w->x2 = ptr2->x1;
            w->Port2 = ptr2->Port1;
            ptr2->Port1->Connections.append(w);   // shorten new wire
            return 2;
          }
    }
    else continue;

    pn = new Node(w->x2, w->y2);   // create new node
    Nodes->append(pn);
    pn->Connections.append(w);  // connect schematic node to the new wire
    w->Port2 = pn;

    // split the wire into two wires
    splitWire(ptr2, pn);
    return 2;
  }

  pn = new Node(w->x2, w->y2);   // create new node
  Nodes->append(pn);
  pn->Connections.append(w);  // connect schematic node to the new wire
  w->Port2 = pn;
  return 1;
}

// ---------------------------------------------------


// ---------------------------------------------------
// Inserts a vertical or horizontal wire into the schematic and connects
// the ports that hit together. Returns whether the beginning and ending
// (the ports of the wire) are connected or not.
int Schematic::insertWire(Wire *w)
{
  int  tmp, con = 0;

  // change coordinates if necessary (port 1 coordinates must be less
  // port 2 coordinates)
  if(w->x1 > w->x2) { tmp = w->x1; w->x1 = w->x2; w->x2 = tmp; }
  else
  if(w->y1 > w->y2) { tmp = w->y1; w->y1 = w->y2; w->y2 = tmp; }
  else con = 0x100;



  tmp = insertWireNode1(w);
  if(tmp == 0) return 3;  // no new wire and no open connection
  if(tmp > 1) con |= 2;   // insert node and remember if it remains open

  


  
  tmp = insertWireNode2(w);
  if(tmp == 0) return 3;  // no new wire and no open connection
  if(tmp > 1) con |= 1;   // insert node and remember if it remains open



  

  // change node 1 and 2
  if(con > 255) con = ((con >> 1) & 1) | ((con << 1) & 2);

  Wires->append(w);    // add wire to the schematic




  int  n1, n2;
  Wire *pw, *nw;
  Node *pn, *pn2;
  Element *pe;
  // ................................................................
  // Check if the new line covers existing nodes.
  // In order to also check new appearing wires -> use "for"-loop
  for(pw = Wires->current(); pw != 0; pw = Wires->next())
    for(pn = Nodes->first(); pn != 0; ) {  // check every node
      if(pn->cx == pw->x1) {
        if(pn->cy <= pw->y1) { pn = Nodes->next(); continue; }
        if(pn->cy >= pw->y2) { pn = Nodes->next(); continue; }
      }
      else if(pn->cy == pw->y1) {
        if(pn->cx <= pw->x1) { pn = Nodes->next(); continue; }
        if(pn->cx >= pw->x2) { pn = Nodes->next(); continue; }
      }
      else { pn = Nodes->next(); continue; }

      n1 = 2; n2 = 3;
      pn2 = pn;
      // check all connections of the current node
      for(pe=pn->Connections.first(); pe!=0; pe=pn->Connections.next()) {
        if(pe->Type != isWire) continue;
        nw = (Wire*)pe;
	// wire lies within the new ?
	if(pw->isHorizontal() != nw->isHorizontal()) continue;

        pn  = nw->Port1;
        pn2 = nw->Port2;
        n1  = pn->Connections.count();
        n2  = pn2->Connections.count();
        if(n1 == 1) {
          Nodes->removeRef(pn);     // delete node 1 if open
          pn2->Connections.removeRef(nw);   // remove connection
          pn = pn2;
        }

        if(n2 == 1) {
          pn->Connections.removeRef(nw);   // remove connection
          Nodes->removeRef(pn2);     // delete node 2 if open
          pn2 = pn;
        }

        if(pn == pn2) {
	  if(nw->Label) {
	    pw->Label = nw->Label;
	    pw->Label->pOwner = pw;
	  }
          Wires->removeRef(nw);    // delete wire
          Wires->findRef(pw);      // set back to current wire
        }
        break;
      }
      if(n1 == 1) if(n2 == 1) continue;

      // split wire into two wires
      if((pw->x1 != pn->cx) || (pw->y1 != pn->cy)) {
        nw = new Wire(pw->x1, pw->y1, pn->cx, pn->cy, pw->Port1, pn);
        pn->Connections.append(nw);
        Wires->append(nw);
        Wires->findRef(pw);
        pw->Port1->Connections.append(nw);
      }
      pw->Port1->Connections.removeRef(pw);
      pw->x1 = pn2->cx;
      pw->y1 = pn2->cy;
      pw->Port1 = pn2;
      pn2->Connections.append(pw);

      pn = Nodes->next();
    }

  if (Wires->containsRef (w))  // if two wire lines with different labels ...
    oneLabel(w->Port1);       // ... are connected, delete one label
  return con | 0x0200;   // sent also end flag
}

// ---------------------------------------------------
// Splits the wire "*pw" into two pieces by the node "*pn".
Wire* Schematic::splitWire(Wire *pw, Node *pn)
{
  Wire *newWire = new Wire(pn->cx, pn->cy, pw->x2, pw->y2, pn, pw->Port2);
  newWire->isSelected = pw->isSelected;

  pw->x2 = pn->cx;
  pw->y2 = pn->cy;
  pw->Port2 = pn;

  newWire->Port2->Connections.prepend(newWire);
  pn->Connections.prepend(pw);
  pn->Connections.prepend(newWire);
  newWire->Port2->Connections.removeRef(pw);
  Wires->append(newWire);

  if(pw->Label)
    if((pw->Label->cx > pn->cx) || (pw->Label->cy > pn->cy)) {
      newWire->Label = pw->Label;   // label goes to the new wire
      pw->Label = 0;
      newWire->Label->pOwner = newWire;
    }

  return newWire;
}

// ---------------------------------------------------
// If possible, make one wire out of two wires.
bool Schematic::oneTwoWires(Node *n)
{
  Wire *e3;
  Wire *e1 = (Wire*)n->Connections.getFirst();  // two wires -> one wire
  Wire *e2 = (Wire*)n->Connections.getLast();

  if(e1->Type == isWire) if(e2->Type == isWire)
    if(e1->isHorizontal() == e2->isHorizontal()) {
      if(e1->x1 == e2->x2) if(e1->y1 == e2->y2) {
        e3 = e1; e1 = e2; e2 = e3;    // e1 must have lesser coordinates
      }
      if(e2->Label) {   // take over the node name label ?
        e1->Label = e2->Label;
	e1->Label->pOwner = e1;
      }
      else if(n->Label) {
             e1->Label = n->Label;
	     e1->Label->pOwner = e1;
	     if(e1->isHorizontal())
	       e1->Label->Type = isHWireLabel;
	     else
	       e1->Label->Type = isVWireLabel;
	   }

      e1->x2 = e2->x2;
      e1->y2 = e2->y2;
      e1->Port2 = e2->Port2;
      Nodes->removeRef(n);    // delete node (is auto delete)
      e1->Port2->Connections.removeRef(e2);
      e1->Port2->Connections.append(e1);
      Wires->removeRef(e2);
      return true;
    }
  return false;
}

// ---------------------------------------------------
// Deletes the wire 'w'.
void Schematic::deleteWire(Wire *w)
{
  if(w->Port1->Connections.count() == 1) {
    if(w->Port1->Label) delete w->Port1->Label;
    Nodes->removeRef(w->Port1);     // delete node 1 if open
  }
  else {
    w->Port1->Connections.removeRef(w);   // remove connection
    if(w->Port1->Connections.count() == 2)
      oneTwoWires(w->Port1);  // two wires -> one wire
  }

  if(w->Port2->Connections.count() == 1) {
    if(w->Port2->Label) delete w->Port2->Label;
    Nodes->removeRef(w->Port2);     // delete node 2 if open
  }
  else {
    w->Port2->Connections.removeRef(w);   // remove connection
    if(w->Port2->Connections.count() == 2)
      oneTwoWires(w->Port2);  // two wires -> one wire
  }

  if(w->Label) {
    delete w->Label;
    w->Label = 0;
  }
  Wires->removeRef(w);
}

// ---------------------------------------------------



/* *******************************************************************
   *****                                                         *****
   *****                  Actions with markers                   *****
   *****                                                         *****
   ******************************************************************* */





/* *******************************************************************
   *****                                                         *****
   *****               Actions with all elements                 *****
   *****                                                         *****
   ******************************************************************* */

// Selects the element that contains the coordinates x/y.
// Returns the pointer to the element.
// If 'flag' is true, the element can be deselected.





// ---------------------------------------------------


// ---------------------------------------------------
// For moving elements: If the moving element is connected to a not
// moving element, insert two wires. If the connected element is already
// a wire, use this wire. Otherwise create new wire.
void Schematic::newMovingWires(QPtrList<Element> *p, Node *pn, int pos)
{
  Element *pe;

  if(pn->State & 8)  // Were new wires already inserted ?
    return;
  pn->State |= 8;

  for (;;) {
    if(pn->State & 16)  // node was already worked on
      break;

    pe = pn->Connections.getFirst();
    if(pe == 0)  return;

    if(pn->Connections.count() > 1)
      break;
    if(pe->Type != isWire)  // is it connected to exactly one wire ?
      break;

    // .................................................
    long  mask = 1, invMask = 3;
    Wire *pw2=0, *pw = (Wire*)pe;

    Node *pn2 = pw->Port1;
    if(pn2 == pn) pn2 = pw->Port2;

    if(pn2->Connections.count() == 2) // two existing wires connected ?
      if((pn2->State & (8+4)) == 0) {
        Element *pe2 = pn2->Connections.getFirst();
        if(pe2 == pe) pe2 = pn2->Connections.getLast();
        // connected wire connected to exactly one wire ?
        if(pe2->Type == isWire)
          pw2  = (Wire*)pe2;
      }

    // .................................................
    // reuse one wire
    p->insert(pos, pw);
    pw->Port1->Connections.removeRef(pw);   // remove connection 1
    pw->Port1->State |= 16+4;
    pw->Port2->Connections.removeRef(pw);   // remove connection 2
    pw->Port2->State |= 16+4;
    Wires->take(Wires->findRef(pw));

    if(pw->isHorizontal()) mask = 2;

    if(pw2 == 0) {  // place new wire between component and old wire
      pn = pn2;
      mask ^= 3;
      invMask = 0;
    }

    if(pw->Port1 != pn) {
      pw->Port1->State |= mask;
      pw->Port1 = (Node*)mask;
      pw->Port2->State |= invMask;
      pw->Port2 = (Node*)invMask;  // move port 2 completely
    }
    else {
      pw->Port1->State |= invMask;
      pw->Port1 = (Node*)invMask;
      pw->Port2->State |= mask;
      pw->Port2 = (Node*)mask;
    }

    invMask ^= 3;
    // .................................................
    // create new wire ?
    if(pw2 == 0) {
      if(pw->Port1 != (Node*)mask)
        p->insert(pos,
          new Wire(pw->x2, pw->y2, pw->x2, pw->y2, (Node*)mask, (Node*)invMask));
      else
        p->insert(pos,
          new Wire(pw->x1, pw->y1, pw->x1, pw->y1, (Node*)mask, (Node*)invMask));
      return;
    }


    // .................................................
    // reuse a second wire
    p->insert(pos, pw2);
    pw2->Port1->Connections.removeRef(pw2);   // remove connection 1
    pw2->Port1->State |= 16+4;
    pw2->Port2->Connections.removeRef(pw2);   // remove connection 2
    pw2->Port2->State |= 16+4;
    Wires->take(Wires->findRef(pw2));

    if(pw2->Port1 != pn2) {
      pw2->Port1 = (Node*)0;
      pw2->Port2->State |= mask;
      pw2->Port2 = (Node*)mask;
    }
    else {
      pw2->Port1->State |= mask;
      pw2->Port1 = (Node*)mask;
      pw2->Port2 = (Node*)0;
    }
    return;
  }

  // only x2 moving
  p->insert(pos, new Wire(pn->cx, pn->cy, pn->cx, pn->cy, (Node*)0, (Node*)1));
  // x1, x2, y2 moving
  p->insert(pos, new Wire(pn->cx, pn->cy, pn->cx, pn->cy, (Node*)1, (Node*)3));
}


/* *******************************************************************
   *****                                                         *****
   *****                Actions with components                  *****
   *****                                                         *****
   ******************************************************************* */

// Finds the correct number for power sources, subcircuit ports and
// digital sources and sets them accordingly.
void Schematic::setComponentNumber(Component *c)
{
  Property *pp = c->Props.getFirst();
  if(!pp) return;
  if(pp->Name != "Num") return;

  int n=1;
  QString s = pp->Value;
  QString cSign = c->Model;
  Component *pc;
  // First look, if the port number already exists.
  for(pc = Components->first(); pc != 0; pc = Components->next())
    if(pc->Model == cSign)
      if(pc->Props.getFirst()->Value == s) break;
  if(!pc) return;   // was port number not yet in use ?

  // Find the first free number.
  do {
    s  = QString::number(n);
    // look for existing ports and their numbers
    for(pc = Components->first(); pc != 0; pc = Components->next())
      if(pc->Model == cSign)
        if(pc->Props.getFirst()->Value == s) break;

    n++;
  } while(pc);   // found not used component number
  pp->Value = s; // set new number
}

// ---------------------------------------------------
void Schematic::insertComponentNodes(Component *c, bool noOptimize)
{
  Port *pp;
  // connect every node of the component to corresponding schematic node
  for(pp = c->Ports.first(); pp != 0; pp = c->Ports.next())
    pp->Connection = insertNode(pp->x+c->cx, pp->y+c->cy, c);

  if(noOptimize)  return;

  Node    *pn;
  Element *pe, *pe1;
  QPtrList<Element> *pL;
  // if component over wire then delete this wire
  c->Ports.first();  // omit the first element
  for(pp = c->Ports.next(); pp != 0; pp = c->Ports.next()) {
    pn = pp->Connection;
    for(pe = pn->Connections.first(); pe!=0; pe = pn->Connections.next())
      if(pe->Type == isWire) {
	if(((Wire*)pe)->Port1 == pn)  pL = &(((Wire*)pe)->Port2->Connections);
	else  pL = &(((Wire*)pe)->Port1->Connections);

        for(pe1 = pL->first(); pe1!=0; pe1 = pL->next())
	  if(pe1 == c) {
	    deleteWire((Wire*)pe);
	    break;
	  }
      }
  }
}

// ---------------------------------------------------
// Used for example in moving components.
void Schematic::insertRawComponent(Component *c, bool noOptimize)
{
  // connect every node of component to corresponding schematic node
  insertComponentNodes(c, noOptimize);
  Components->append(c);

  // a ground symbol erases an existing label on the wire line
  if(c->Model == "GND") {
    c->Model = "x";    // prevent that this ground is found as label
    Element *pe = getWireLabel(c->Ports.getFirst()->Connection);
    if(pe) if((pe->Type & isComponent) == 0) {
      delete ((Conductor*)pe)->Label;
      ((Conductor*)pe)->Label = 0;
    }
    c->Model = "GND";    // rebuild component model
  }
}

// ---------------------------------------------------
void Schematic::recreateComponent(Component *Comp)
{
  Port *pp;
  WireLabel **plMem=0, **pl;
  int PortCount = Comp->Ports.count();

  if(PortCount > 0) {
    // Save the labels whose node is not connected to somewhere else.
    // Otherwise the label would be deleted.
    pl = plMem = (WireLabel**)malloc(PortCount * sizeof(WireLabel*));
    for(pp = Comp->Ports.first(); pp != 0; pp = Comp->Ports.next())
      if(pp->Connection->Connections.count() < 2) {
        *(pl++) = pp->Connection->Label;
        pp->Connection->Label = 0;
      }
      else  *(pl++) = 0;
  }


  int x = Comp->tx, y = Comp->ty;
  int x1 = Comp->x1, x2 = Comp->x2, y1 = Comp->y1, y2 = Comp->y2;
  QString tmp = Comp->Name;    // is sometimes changed by "recreate"
  Comp->recreate(this);   // to apply changes to the schematic symbol
  Comp->Name = tmp;
  if(x < x1)
    x += Comp->x1 - x1;
  else if(x > x2)
    x += Comp->x2 - x2;
  if(y < y1)
    y += Comp->y1 - y1;
  else if(y > y2)
    y += Comp->y2 - y2;
  Comp->tx = x;  Comp->ty = y;


  if(PortCount > 0) {
    // restore node labels
    pl = plMem;
    for(pp = Comp->Ports.first(); pp != 0; pp = Comp->Ports.next()) {
      if(*pl != 0) {
        (*pl)->cx = pp->Connection->cx;
        (*pl)->cy = pp->Connection->cy;
        placeNodeLabel(*pl);
      }
      pl++;
      if((--PortCount) < 1)  break;
    }
    for( ; PortCount > 0; PortCount--) {
      delete (*pl);  // delete not needed labels
      pl++;
    }
    free(plMem);
  }
}

// ---------------------------------------------------
void Schematic::insertComponent(Component *c)
{
  // connect every node of component to corresponding schematic node
  insertComponentNodes(c, false);

  bool ok;
  QString s;
  int  max=1, len = c->Name.length(), z;
  if(c->Name.isEmpty()) {
    // a ground symbol erases an existing label on the wire line
    if(c->Model == "GND") {
      c->Model = "x";    // prevent that this ground is found as label
      Element *pe = getWireLabel(c->Ports.getFirst()->Connection);
      if(pe) if((pe->Type & isComponent) == 0) {
        delete ((Conductor*)pe)->Label;
        ((Conductor*)pe)->Label = 0;
      }
      c->Model = "GND";    // rebuild component model
    }
  }
  else {
    // determines the name by looking for names with the same
    // prefix and increment the number
    for(Component *pc = Components->first(); pc != 0; pc = Components->next())
      if(pc->Name.left(len) == c->Name) {
        s = pc->Name.right(pc->Name.length()-len);
        z = s.toInt(&ok);
        if(ok) if(z >= max) max = z + 1;
      }
    c->Name += QString::number(max);  // create name with new number
  }

  setComponentNumber(c); // important for power sources and subcircuit ports
  Components->append(c);
}





// ---------------------------------------------------
// Sets the component ports anew. Used after rotate, mirror etc.
void Schematic::setCompPorts(Component *pc)
{
  Port *pp;
  WireLabel *pl;
  QPtrList<WireLabel> LabelCache;

  for(pp = pc->Ports.first(); pp!=0; pp = pc->Ports.next()) {
    pp->Connection->Connections.removeRef((Element*)pc);// delete connections
    switch(pp->Connection->Connections.count()) {
      case 0:
        pl = pp->Connection->Label;
        if(pl) {
          LabelCache.append(pl);
          pl->cx = pp->x + pc->cx;
          pl->cy = pp->y + pc->cy;
        }
        Nodes->removeRef(pp->Connection);
        break;
      case 2:
        oneTwoWires(pp->Connection); // try to connect two wires to one
      default: ;
    }
  }

  // Re-connect component node to schematic node. This must be done completely
  // after the first loop in order to avoid problems with node labels.
  for(pp = pc->Ports.first(); pp!=0; pp = pc->Ports.next())
    pp->Connection = insertNode(pp->x+pc->cx, pp->y+pc->cy, pc);

}

// ---------------------------------------------------


// ---------------------------------------------------
Component* Schematic::searchSelSubcircuit()
{
  Component *sub=0;
  // test all components
  for(Component *pc = Components->first(); pc != 0; pc = Components->next()) {
    if(!pc->isSelected) continue;
    if(pc->Model != "Sub")
      if(pc->Model != "VHDL") continue;

    if(sub != 0) return 0;    // more than one subcircuit selected
    sub = pc;
  }
  return sub;
}


/* *******************************************************************
   *****                                                         *****
   *****                  Actions with labels                    *****
   *****                                                         *****
   ******************************************************************* */

// Test, if wire connects wire line with more than one label and delete
// all further labels. Also delete all labels if wire line is grounded.
void Schematic::oneLabel(Node *n1)
{
  Wire *pw;
  Node *pn, *pNode;
  Element *pe;
  WireLabel *pl = 0;
  bool named=false;   // wire line already named ?
  QPtrList<Node> Cons;

  for(pn = Nodes->first(); pn!=0; pn = Nodes->next())
    pn->y1 = 0;   // mark all nodes as not checked

  Cons.append(n1);
  n1->y1 = 1;  // mark Node as already checked
  for(pn = Cons.first(); pn!=0; pn = Cons.next()) {
    if(pn->Label) {
      if(named) {
        delete pn->Label;
        pn->Label = 0;    // erase double names
      }
      else {
	named = true;
	pl = pn->Label;
      }
    }

    for(pe = pn->Connections.first(); pe!=0; pe = pn->Connections.next()) {
      if(pe->Type != isWire) {
        if(((Component*)pe)->isActive == COMP_IS_ACTIVE)
	  if(((Component*)pe)->Model == "GND") {
	    named = true;
	    if(pl) {
	      pl->pOwner->Label = 0;
	      delete pl;
	    }
	    pl = 0;
	  }
	continue;
      }
      pw = (Wire*)pe;

      if(pn != pw->Port1) pNode = pw->Port1;
      else pNode = pw->Port2;

      if(pNode->y1) continue;
      pNode->y1 = 1;  // mark Node as already checked
      Cons.append(pNode);
      Cons.findRef(pn);

      if(pw->Label) {
        if(named) {
          delete pw->Label;
          pw->Label = 0;    // erase double names
        }
        else {
	  named = true;
	  pl = pw->Label;
	}
      }
    }
  }
}

// ---------------------------------------------------
int Schematic::placeNodeLabel(WireLabel *pl)
{
  Node *pn;
  int x = pl->cx;
  int y = pl->cy;

  // check if new node lies upon an existing node
  for(pn = Nodes->first(); pn != 0; pn = Nodes->next())
    if(pn->cx == x) if(pn->cy == y) break;

  if(!pn)  return -1;

  Element *pe = getWireLabel(pn);
  if(pe) {    // name found ?
    if(pe->Type & isComponent)  return -2;  // ground potential

    delete ((Conductor*)pe)->Label;
    ((Conductor*)pe)->Label = 0;
  }

  pn->Label = pl;   // insert node label
  return 0;
}

// ---------------------------------------------------
// Test, if wire line is already labeled and returns a pointer to the
// labeled element.
Element* Schematic::getWireLabel(Node *pn_)
{
  Wire *pw;
  Node *pn, *pNode;
  Element *pe;
  QPtrList<Node> Cons;

  for(pn = Nodes->first(); pn!=0; pn = Nodes->next())
    pn->y1 = 0;   // mark all nodes as not checked

  Cons.append(pn_);
  pn_->y1 = 1;  // mark Node as already checked
  for(pn = Cons.first(); pn!=0; pn = Cons.next())
    if(pn->Label) return pn;
    else
      for(pe = pn->Connections.first(); pe!=0; pe = pn->Connections.next()) {
        if(pe->Type != isWire) {
	  if(((Component*)pe)->isActive == COMP_IS_ACTIVE)
	    if(((Component*)pe)->Model == "GND") return pe;
          continue;
        }

        pw = (Wire*)pe;
        if(pw->Label) return pw;

        if(pn != pw->Port1) pNode = pw->Port1;
        else pNode = pw->Port2;

        if(pNode->y1) continue;
        pNode->y1 = 1;  // mark Node as already checked
        Cons.append(pNode);
        Cons.findRef(pn);
      }
  return 0;   // no wire label found
}

// ---------------------------------------------------
