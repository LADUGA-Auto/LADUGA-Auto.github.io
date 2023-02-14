/***************************************************************************
                               PRADISComponent.cpp
                              ---------------
    begin                : Sun August 12 2007
    copyright            : (C) 2007 by LADUGA Ltd.
    email                : laduga@laduga.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "PRADISComponent.h"
#include "../node.h"
#include "../schematic.h"
#include "../BaseElement.h"
#include "../element.h"
#include "../main.h"


#include <qptrlist.h>
#include <qmessagebox.h>

const QString PRADISComponent::RangePrefix = "Range:";
const QString PRADISComponent::ImagePrefix = "Image:";
const QString PRADISComponent::ValuePrefix = "Value:";

 int     PRADISComponent::isCreateImage = 0;


PRADISComponent::PRADISComponent()
{
	type = 0;
	pElement=0;
};

PRADISComponent::PRADISComponent(PRADISComponent& pc)
{
	type = pc.type;
	moduleName=pc.moduleName;
	pElement=pc.pElement;
};


	
void PRADISComponent::setdata()
{
	/*pradis::sysarm::BaseElement* be=pElement;
		
	Description = pElement->GetRuDescription();
	Model = pElement->GetName();
  Name  = Model;

	copyImage2(be);
/*
  x1 = be->Image2.x1;
  y1 = be->Image2.y1;
  x2 = be->Image2.x2;
  y2 = be->Image2.y2;

  tx = x1+ be->Image2.tx;
  ty = y2+ be->Image2.ty;
*/  
  /*int np = 0;
  
  QString module;
  
	Model = pElement->GetName();
	np = pElement->ParameterNames.size();
	module = pElement->GetModule();  

  Name  = Model;
//>PRA-10
//	if (module=="Passport" && Name == "Port")
//	{
//		Model = Name;
//
//	}else
	{
		Model = module+"."+Name;
	}

//<PRA-10

	for (int i=0;i<np;i++)
	{
		QString parname, pardesc,pardefault;
			switch(type)
			{
			case 1:case 2:case 3:	case 4:
								parname = pElement->ParameterNames[i];
								pardesc = pElement->ParameterRuDescription[i];
								pardefault = pElement->ParameterDefaultValue[i];
								break;
				default:;
			}

	  Props.append(new Property(parname, pardefault, false, pardesc));
	}
	if(QucsMain!=0)
		setDefaultImage(QucsMain->SysArm);
	if (IsOVP())
		setDefaultOutValues();	*/
}
//TODO: Добавить в описание параметра значение по умолчанию и флаг отображения
//TODO: Добавить корректную обработку для разных значений паспорта объекта
//TODO: Отредатктировать системный каталог, а именно - описания параметров


Component* PRADISComponent::newOne()
{
  PRADISComponent* pc =  new PRADISComponent();
//  *pc=*this;
	pc->type = type;
	pc->moduleName=moduleName;
	pc->pElement=pElement;
  pc->setdata();
	//pc->setDefaultImage(QucsMain->SysArm);
  
  return pc;
}


void PRADISComponent::recreate(Schematic* Doc)
{
	Component::recreate(Doc);

	if (!(type==7  || type==6 || type==8))
	{
		return;
	}
	//from MultiView
  if(Doc) {
    Doc->Components->setAutoDelete(false);
//    Doc->deleteComp(this);
  }

  Ellips.clear();
  Texts.clear();
  Ports.clear();
  Lines.clear();
  Rects.clear();
  Arcs.clear();
//  createSymbol();
  
  bool mmir = mirroredX;
  int  rrot = rotated;
  //if(mmir)  mirrorX();   // mirror
  //for(int z=0; z<rrot; z++)  rotate(); // rotate

  rotated = rrot;   // restore properties (were changed by rotate/mirror)
  mirroredX = mmir;

  if(Doc) {
    Doc->insertRawComponent(this);
    Doc->Components->setAutoDelete(true);
  }

};

void PRADISComponent::copyComponent(Component* comp)
{
	Component::copyComponent(comp);
	type = ((PRADISComponent*)comp)->type;
	moduleName =( (PRADISComponent*)comp)->moduleName;
	pElement=((PRADISComponent*)comp)->pElement;
	
};	

void PRADISComponent::loadPRADISProperties(Component *Comp, const QString &s, int z)
{
	QString n;
//	if (Comp->Model=="base.DISP" || Comp->Model=="base.SHTERM" || Comp->Model=="base.NEWMARK" )
	{		
	  do
	  {
		  z++;
		  n = s.section('"',z,z);    // property value
		  z++;
			if(n.startsWith(RangePrefix))
			{
				Comp->Props.append( new Property(n.section('=',0,0), n.section('=',1)));
			}else
			if( n.startsWith(ImagePrefix))
			{
				Property* pr;
				pr = ((PRADISComponent*)Comp)->getProperty(ImagePrefix);
				if(pr==0)
				{
					Comp->Props.append( new Property(ImagePrefix,n.section('=',1)));
				}else
				{
					pr->Value = n.section('=',1);
				}
			}
		}while(n!="");
	}
}



Property* PRADISComponent::getProperty(const QString& name)
{
	Property* pr;
	for (pr=Props.first(); pr!=0; pr=Props.next())
	{
		if (pr->Name==name)
		{
			return pr;
		}
	}
	return 0;	
}

void PRADISComponent::setProperty(const QString name, const QString value, const QString description)
{
/*	Property* pr;
	for (pr=Props.first(); pr!=0; pr=Props.next())
	{
		if (pr->Name==name)
		{                
			pr->Value = value;
			return;
		}
	}         
	Props.append( new Property(name, value, false, description));*/
	return ;	
}




QString PRADISComponent::convert2uc(const QString& str)
{
	QString value = str;
	value = value.replace("\n", "\\n");
	value = value.replace("<", "%lt;");
	value = value.replace(">", "%gt;");
	value = value.replace("\"", "%ap;");
	value = value.replace(":", "%dp;");
	value = value.replace("&", "%amp;");
	return value;
};

QString PRADISComponent::convert2int(const QString& str)
{
	QString value = str;
	value = value.replace("\\n", "\n");
	value = value.replace("%lt;", "<");
	value = value.replace("%gt;", ">");
	value = value.replace("%ap;", "\"");
	value = value.replace("%dp;", ":");
	value = value.replace("%amp;", "&");
	
	return value;
};
