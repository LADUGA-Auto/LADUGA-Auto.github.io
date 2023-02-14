/***************************************************************************
                                 main.cpp
                                ----------
    begin                : Thu Aug 28 2003
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

#include <stdlib.h>
#include <math.h>
#include <locale.h>

#include <qapplication.h>
#include <qmessagebox.h>
#include <qregexp.h>

//#include "qucs.h"
#include "main.h"
#include "schematic.h"
#include "pradis/PRADISComponent.h"
#include "schParser.h"
#include <string>


//tQucsSettings QucsSettings;

//QFont savingFont;    // to remember which font to save in "qucsrc"

//QucsApp *QucsMain;  // the Qucs application itself

//PRA-1
//QString lastDir;    // to remember last directory for several dialogs

// #########################################################################
// Loads the settings file and stores the settings.

// #########################################################################
// Saves the settings in the settings file.
/*bool saveApplSettings(QucsApp *qucs)
{
//PRA-2	
//  QFile file(QucsHomeDir.filePath("qucsrc"));
	QDir home(QDir::homeDirPath()+QDir::convertSeparators ("/.qucs"));
	QFile file(home.filePath("qucsrc"));

  if(!file.open(IO_WriteOnly)) {    // settings file cannot be created
    QMessageBox::warning(0, QObject::tr("Warning"),
			QObject::tr("Cannot save settings !"));
    return false;
  }

  QString Line;
  QTextStream stream(&file);


  stream << "Settings file, Qucs " PACKAGE_VERSION "\n"
    << "Position=" << qucs->x() << "," << qucs->y() << "\n"
    << "Size=" << qucs->width() << "," << qucs->height() << "\n"
    << "Font=" << savingFont.toString() << "\n"
    << "Language=" << QucsSettings.Language << "\n"
    << "BGColor=" << QucsSettings.BGColor.name() << "\n"
    << "maxUndo=" << QucsSettings.maxUndo << "\n"
    << "Editor=" << QucsSettings.Editor << "\n"
    << "SyntaxColor="
    << QucsSettings.VHDL_Comment.name() << ","
    << QucsSettings.VHDL_String.name() << ","
    << QucsSettings.VHDL_Integer.name() << ","
    << QucsSettings.VHDL_Real.name() << ","
    << QucsSettings.VHDL_Character.name() << ","
    << QucsSettings.VHDL_Types.name() << ","
    << QucsSettings.VHDL_Attributes.name() << "\n"
    << "NodeWiring=" << QucsSettings.NodeWiring << "\n"
//PRA-1
    << "LastFolder=" << lastDir << "\n"
//PRA-1
    << "HomeFolder=" << QucsSettings.HomeDir << "\n";

  QStringList::Iterator it = QucsSettings.FileTypes.begin();
  while(it != QucsSettings.FileTypes.end())
    stream << "FileType=" << (*(it++)) << "\n";

  file.close();
  return true;
}*/

// #########################################################################
QString complexRect(double real, double imag, int Precision)
{
  QString Text;
  if(fabs(imag) < 1e-250) Text = QString::number(real,'g',Precision);
  else {
    Text = QString::number(imag,'g',Precision);
    if(Text.at(0) == '-') {
      Text.at(0) = 'j';
      Text = '-'+Text;
    }
    else  Text = "+j"+Text;
    Text = QString::number(real,'g',Precision) + Text;
  }
  return Text;
}

QString complexDeg(double real, double imag, int Precision)
{
  QString Text;
  if(fabs(imag) < 1e-250) Text = QString::number(real,'g',Precision);
  else {
    Text  = QString::number(sqrt(real*real+imag*imag),'g',Precision) + " / ";
    Text += QString::number(180.0/M_PI*atan2(imag,real),'g',Precision) + '°';
  }
  return Text;
}

QString complexRad (double real, double imag, int Precision)
{
  QString Text;
  if(fabs(imag) < 1e-250) Text = QString::number(real,'g',Precision);
  else {
    Text  = QString::number(sqrt(real*real+imag*imag),'g',Precision);
    Text += " / " + QString::number(atan2(imag,real),'g',Precision) + "rad";
  }
  return Text;
}

// #########################################################################
QString StringNum(double num, char form, int Precision)
{
  int a = 0;
  char *p, Buffer[512], Format[6] = "%.00g";

  if(Precision < 0) {
    Format[1]  = form;
    Format[2]  = 0;
  }
  else {
    Format[4]  = form;
    Format[2] += Precision / 10;
    Format[3] += Precision % 10;
  }
  sprintf(Buffer, Format, num);
  p = strchr(Buffer, 'e');
  if(p) {
    p++;
    if(*(p++) == '+') { a = 1; }   // remove '+' of exponent
    if(*p == '0') { a++; p++; }    // remove leading zeros of exponent
    if(a > 0)
      do {
        *(p-a) = *p;
      } while(*(p++) != 0);    // override characters not needed
  }

  return QString(Buffer);
}

// #########################################################################
QString StringNiceNum(double num)
{
  char Format[6] = "%.8e";
  if(fabs(num) < 1e-250)  return QString("0");  // avoid many problems
  if(fabs(log10(fabs(num))) < 3.0)  Format[3] = 'g';

  int a = 0;
  char *p, *pe, Buffer[512];

  sprintf(Buffer, Format, num);
  p = pe = strchr(Buffer, 'e');
  if(p) {
    if(*(++p) == '+') { a = 1; }    // remove '+' of exponent
    if(*(++p) == '0') { a++; p++; } // remove leading zeros of exponent
    if(a > 0)
      do {
        *(p-a) = *p;
      } while(*(p++) != 0);  // override characters not needed

    // In 'g' format, trailing zeros are already cut off !!!
    p = strchr(Buffer, '.');
    if(p) {
      if(!pe)  pe = Buffer + strlen(Buffer);
      p = pe-1;
      while(*p == '0')   // looking for unneccessary zero characters
        if((--p) <= Buffer)  break;
      if(*p != '.')  p++;  // no digit after decimal point ?
      while( (*(p++) = *(pe++)) != 0 ) ;  // overwrite zero characters
    }
  }

  return QString(Buffer);
}

// #########################################################################
void str2num(const QString& s_, double& Number, QString& Unit, double& Factor)
{
  QString str = s_.stripWhiteSpace();

/*  int i=0;
  bool neg = false;
  if(str[0] == '-') {      // check sign
    neg = true;
    i++;
  }
  else if(str[0] == '+')  i++;

  double num = 0.0;
  for(;;) {
    if(str[i] >= '0')  if(str[i] <= '9') {
      num = 10.0*num + double(str[i]-'0');
    }
  }*/

  QRegExp Expr( QRegExp("[^0-9\\x2E\\x2D\\x2B]") );
  int i = str.find( Expr );
  if(i >= 0)
    if((str.at(i).latin1() | 0x20) == 'e') {
      int j = str.find( Expr , ++i);
      if(j == i)  j--;
      i = j;
    }

  Number = str.left(i).toDouble();
  Unit   = str.mid(i).stripWhiteSpace();

  switch(Unit.at(0).latin1()) {
    case 'T': Factor = 1e12;  break;
    case 'G': Factor = 1e9;   break;
    case 'M': Factor = 1e6;   break;
    case 'k': Factor = 1e3;   break;
    case 'c': Factor = 1e-2;  break;
    case 'm': Factor = 1e-3;  break;
    case 'u': Factor = 1e-6;  break;
    case 'n': Factor = 1e-9;  break;
    case 'p': Factor = 1e-12; break;
    case 'f': Factor = 1e-15; break;
//    case 'd':
    default:  Factor = 1.0;
  }

  return;
}

// #########################################################################
QString num2str(double Num)
{
  char c = 0;
  double cal = fabs(Num);
  if(cal > 1e-20) {
    cal = log10(cal) / 3.0;
    if(cal < -0.2)  cal -= 0.98;
    int Expo = int(cal);

    if(Expo >= -5) if(Expo <= 4)
      switch(Expo) {
        case -5: c = 'f'; break;
        case -4: c = 'p'; break;
        case -3: c = 'n'; break;
        case -2: c = 'u'; break;
        case -1: c = 'm'; break;
        case  1: c = 'k'; break;
        case  2: c = 'M'; break;
        case  3: c = 'G'; break;
        case  4: c = 'T'; break;
      }

    if(c)  Num /= pow(10.0, double(3*Expo));
  }

  QString Str = QString::number(Num);
  if(c)  Str += c;
  
  return Str;
}

// #########################################################################
void convert2Unicode(QString& Text)
{
  bool ok;
  int i = 0;
  QString n;
  unsigned short ch;
  while((i=Text.find("\\x", i)) >= 0) {
    n = Text.mid(i, 6);
    ch = n.mid(2).toUShort(&ok, 16);
    if(ok)  Text.replace(n, QChar(ch));
    i++;
  }
  Text.replace("\\n", "\n");
  Text.replace("\\\\", "\\");
}

// #########################################################################
void convert2ASCII(QString& Text)
{
  Text.replace('\\', "\\\\");
  Text.replace('\n', "\\n");

  int i = 0;
  QChar ch;
  char Str[8];
  while((ch=Text.at(i++)) != QChar(0)) {  // convert special characters
    if(ch > QChar(0x7F)) {
      sprintf(Str, "\\x%04X", ch.unicode());
      Text.replace(ch, Str);
    }
  }
}

// #########################################################################
QString properAbsFileName(const QString& Name)
{
  QString s = Name;
  QFileInfo Info(s);
  //if(Info.isRelative()) s = QucsWorkDir.filePath(s);
  return QDir::cleanDirPath(s);
}

// #########################################################################
QString properFileName(const QString& Name)
{
  QFileInfo Info(Name);
  return Info.fileName();
}

// #########################################################################
// Takes a file name (with path) and replaces all special characters.
QString properName(const QString& Name)
{
  QString s = Name;
  QFileInfo Info(s);
  if(Info.extension() == "sch")
    s = s.left(s.length()-4);
  if(s.at(0) <= '9') if(s.at(0) >= '0')
    s = 'n' + s;
  s.replace(QRegExp("\\W"), "_"); // none [a-zA-Z0-9] into "_"
  s.replace("__", "_");  // '__' not allowed in VHDL
  if(s.at(0) == '_')
    s = 'n' + s;
  return s;
}

// #########################################################################
bool checkVersion(QString& Line)
{
  QStringList sl = QStringList::split('.',PACKAGE_VERSION);
  QStringList ll = QStringList::split('.',Line);
  if (ll.count() != 3 || sl.count() != 3)
    return false;
  int sv = (*sl.at(1)).toInt() * 10000 + (*sl.at(2)).toInt() * 100 +
    (*sl.at(3)).toInt();
  int lv = (*ll.at(1)).toInt() * 10000 + (*ll.at(2)).toInt() * 100 +
    (*ll.at(3)).toInt();
  if(lv > sv) // wrong version number ? (only backward compatible)
    return false;
  return true;
}


// #########################################################################
// ##########                                                     ##########
// ##########                  Program Start                      ##########
// ##########                                                     ##########
// #########################################################################
/*
int main(int argc, char *argv[])
{
  
  QApplication a(argc, argv);//must be deleted
//setlocale (LC_NUMERIC, "C");

	
  std::vector<sch_parser_component> vec; // create componentList
  std::vector<sch_parser_property> propList; // create propertyList

  sch_parser par; // create object of parser


  par.read("C:/DINAMA/examples/Qucs/swing.sch"); // read scheme for path

  par.parse(); // fill componentList and propertyList

  par.getComponentList(vec); // return componentList


  std::string componentName = "";
  std::string componentModel = "";

  std::string sValue;
  int iValue;
  bool bValue;
  double dValue;

  bool act = 0;

  FILE* file;
  if (file = fopen("C:/DINAMA/examples/Qucs/swing.txt", "w"))
  {
  
	  fprintf(file, "[ Module ]      |      [ Name ]       |      [ Active ]       |      [ Properties ]\n");




  for (size_t i = 0; i<vec.size(); i++)
  { 

	  vec[i].getName(componentName);
	  vec[i].getModel(componentModel);
	  act = vec[i].isActive();
	  fprintf(file, "[ %s ]  |  [ %s ]  |  ", componentModel.c_str(), componentName.c_str());
	  
	  if(act == true) fprintf(file, "[ true ]  |  " ); // active component or not?
	  if(act == false) fprintf(file, "[ false ]  |  " );
	  

	  //std::vector<sch_parser_property> propList;
	  vec[i].getPropertyList(propList); // return propertyList for component

	  for (size_t j = 0; j < propList.size(); j++)// check type value of property
	  { 
		  if(propList[j].getStringValue(sValue)) fprintf(file, "[ %s : string ]    ", sValue.c_str());
		  else
		  {
			  if(propList[j].getIntValue(iValue)) fprintf(file, "[ %d : integer ]    ", iValue );
			  else
			  {
				  if(propList[j].getBoolValue(bValue)) fprintf(file, "[ %d : boolean ]    ", bValue );
				  else
				  {
					  if(propList[j].getDoubleValue(dValue)) fprintf(file, "[ %lf : double ]    ", dValue );
				  }
			  }
		  }
	  }
  fprintf(file, "\n\n");
  }
  fclose(file);
	
	QMessageBox::information(0, "writing", "complete");
  }
  else
  {
		QMessageBox::information(0, "writing", "fail");
  }
  return 0;
}
*/