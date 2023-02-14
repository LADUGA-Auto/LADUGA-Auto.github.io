
#include "BaseElement.h"
//#include "Image2d.h"

namespace pradis { namespace sysarm
{

void WriteDescription (QTextStream& stream, const QString& ru, const QString& en, const QString& tabs);

//bool WriteImage2d(QTextStream& stream);


BaseElement::BaseElement ()
{
	Name = RuDescription = EnDescription = Alias="";

	NodeTypes.clear();
	NodeNames.clear();

	ParameterTypes.clear();
	ParameterNames.clear();
	
	NodeRuDescription.clear();
	NodeEnDescription.clear();
	ParameterRuDescription.clear();
	ParameterEnDescription.clear();
	
///	strcpy(Image2.icon, "");
//	Image2.Symbol =  "";
	Priority = -1;
};


void BaseElement::SetPriority(int priority)
{
	Priority = priority;
};

void BaseElement::SetName (QString name)
{
	Name = name;
}

void BaseElement::SetAlias (QString name)
{
	Alias = name;
}

void BaseElement::SetRuDescription (QString description)
{
	RuDescription = description;
}

void BaseElement::SetEnDescription (QString description)
{
	EnDescription = description;
}

QString BaseElement::GetName ()
{
	return Name;
}

QString BaseElement::GetAlias ()
{
	return Alias;
}

QString BaseElement::GetRuDescription ()
{
	return RuDescription;
}

QString BaseElement::GetEnDescription ()
{
	return EnDescription;
}

void BaseElement::SetModule(QString module)
{
	Module = module;
}

QString BaseElement::GetModule()
{
	return Module;
}

int BaseElement::GetPriority()
{
	return Priority;	
};





bool BaseElement::write (QTextStream& stream)
{
	QString str;

	
	WriteDescription (stream, GetRuDescription(), GetEnDescription(), "\t");
	stream << "\t<nodelist>\n";
	
	int i;
	for (i = 0; i < NodeNames.size(); i++)
	{
		stream << ("\t\t<node name=\"") << NodeNames[i] <<
					"\" type=\"" << NodeTypes[i] << "\">\n";

		WriteDescription (stream, NodeRuDescription[i], NodeEnDescription[i], "\t\t\t");
		
		stream << ("\t\t</node>\n");
	}
	stream <<  "\t</nodelist>\n";
	stream <<  "\t<parameterlist>\n";

	for (i = 0; i < ParameterNames.size(); i++)
	{
		stream << ("\t\t<parameter name=\"") <<  ParameterNames[i] <<
					"\" type=\"" <<ParameterTypes[i] << 
					"\" default=\"" <<ParameterDefaultValue[i] <<
					"\">\n";

		WriteDescription (stream, ParameterRuDescription[i], ParameterEnDescription[i], "\t\t\t");
		
		stream << ("\t\t</parameter>\n");
	}
	stream << "\t</parameterlist>\n";

///	stream << ("\t<image2d icon = \"")<<Image2.icon<<
	//	"\" symbol = \""<<Image2.Symbol<<"\"/>\n";
		
	return true;
};

void WriteDescription (QTextStream& stream, const QString& ru, const QString& en, const QString& tabs)
{
	stream << tabs << "<description>\n"
	<< tabs << "\t<russian>" << ru << "</russian>\n"
	<< tabs << "\t<english>" << en << "</english>\n"
	<< tabs << "</description>\n";
}


}}
