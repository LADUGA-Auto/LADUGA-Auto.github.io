#ifndef _SYSARM_BASEELEMENT_
#define _SYSARM_BASEELEMENT_

#include <qvaluevector.h>
#include <qstring.h>
#include <QString.h>

namespace pradis { namespace sysarm
{

	class BaseElement
	{

	public:
		BaseElement ();
		virtual ~BaseElement (){};

		QString Name;
		QString Alias;
		QString Module;
		QString RuDescription;
		QString EnDescription;
		int     Priority;
		
	public:
		void SetName (QString);
		void SetAlias(const QString);
		void SetRuDescription (QString);
		void SetEnDescription (QString);
		void SetPriority(int priority);
		QString GetName ();
		QString GetAlias ();
		QString GetRuDescription ();
		QString GetEnDescription ();
		void SetModule(QString);
		QString GetModule();
		int GetPriority();
		
		bool write (QTextStream& stream);
//		bool WriteImage2d(QTextStream& stream);

		virtual int GetType() = 0;

		
		QValueVector <QString> NodeTypes;
		QValueVector <QString> NodeNames;

		QValueVector <QString> ParameterTypes;
		QValueVector <QString> ParameterNames;
		QValueVector <QString> ParameterDefaultValue;

		QValueVector <QString> NodeRuDescription;
		QValueVector <QString> NodeEnDescription;
		
		QValueVector <QString> ParameterRuDescription;
		QValueVector <QString> ParameterEnDescription;

//		Image2d		             Image2;
	};
	
}}

#endif