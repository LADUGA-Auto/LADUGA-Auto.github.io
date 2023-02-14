#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <string>
#include "main.h"
int main()
{
	std::vector<sch_parser_component> componentList;
	std::vector<sch_parser_property> propertyList;


	if (load(componentList, "C:/Users/Danil/Desktop/pars/test_1.sch")) // loading components from .sch file to vector<sch_parser_components>
	{
		std::string componentName = "", componentModel = "", sValue = ""; // creating data buffers
		int iValue = 0;
		bool bValue = 0, active = 0;
		double dValue = 0;


		FILE* file;
		if (file = fopen("C:/Users/Danil/Desktop/pradiswork/test_pradis_parser/test.txt", "w")) // writing components info to .txt file
		{

			fprintf(file, "[ Module ]      |      [ Name ]       |      [ Active ]       |      [ Properties ]\n");

			for (size_t i = 0; i < componentList.size(); i++)
			{
				componentList[i].getName(componentName);
				componentList[i].getModel(componentModel);
				active = componentList[i].isActive();
				fprintf(file, "[ %s ]  |  [ %s ]  |  ", componentModel.c_str(), componentName.c_str());

				if (active) fprintf(file, "[ true ]  |  "); // active component or not?
				if (!active) fprintf(file, "[ false ]  |  ");


				componentList[i].getPropertyList(propertyList); // return propertyList for component

				for (size_t j = 0; j < propertyList.size(); j++)// check type value of property
				{
					if (propertyList[j].getStringValue(sValue)) fprintf(file, "[ %s : %s : string ]    ", propertyList[j].name.c_str(), sValue.c_str());
					else
					{
						if (propertyList[j].getIntValue(iValue)) fprintf(file, "[ %s : %d : integer ]    ", propertyList[j].name.c_str(), iValue);
						else
						{
							if (propertyList[j].getBoolValue(bValue)) fprintf(file, "[ %s : %d : boolean ]    ", propertyList[j].name.c_str(), bValue);
							else
							{
								if (propertyList[j].getDoubleValue(dValue)) fprintf(file, "[ %s : %lf : double ]    ", propertyList[j].name.c_str(), dValue);
							}
						}
					}
				}
				fprintf(file, "\n\n");
			}
			fclose(file);

			printf("Complete write file!\n");
		}
		else
		{
			printf("Error write file!\n");
			return -1;
		}

		return 0;
	}
	else {
		printf("Error load function!\n");
		return -1;
	}
}
