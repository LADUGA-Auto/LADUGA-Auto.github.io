/***************************************************************************
                               PRADISComponent.h
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

#ifndef PRADISCOMPONENT_H
#define PRADISCOMPONENT_H 



#include "../components/component.h"
#include "../BaseElement.h"

class PRADISComponent : public Component {
  
public:
	static const QString RangePrefix;
	static const QString ImagePrefix;
	static const QString ValuePrefix;
	static int isCreateImage;

  PRADISComponent();
  PRADISComponent(PRADISComponent& pc);
 ~PRADISComponent(){};


	
  Component* newOne();
  
	inline int getType(){return type;};
	inline bool IsModel(){return type==1;};
	inline bool IsOVP(){return type==2;};
	inline bool IsImage(){return type==3;};
	Property* getProperty(const QString& name);   
	void setProperty(const QString name, const QString value, const QString description="");


  void recreate(Schematic*);
  
	void setDefaultOutValues();

  static void loadPRADISProperties(Component *Comp, const QString &s, int z);
	static QString convert2uc(const QString& str);
	static QString convert2int(const QString& str);
  

protected:
	void setdata();
	void copyImage2(pradis::sysarm::BaseElement* be);
	void copyComponent(Component*);	
	
	int type;
	QString moduleName;
	pradis::sysarm::BaseElement *pElement;
};
#endif
