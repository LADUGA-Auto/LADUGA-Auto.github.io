// dllmain.cpp : Определяет точку входа для приложения DLL.                 
#include <windows.h>
#include <vector>
#include "PRADIS_scheme_parser.h"

sch_parser* parser = nullptr;

BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    parser = new sch_parser;
    return TRUE;
}


extern "C" __declspec (dllexport) bool load(std::vector<sch_parser_component>&list, std::string path) ///////
{                                                    

    parser->parse(path); // fill componentList and propertyList

    parser->getComponentList(list);

    return true;

}

extern "C" __declspec (dllexport) void close()
{

    if (NULL != parser)
    {
        delete parser;
        parser = NULL;
    }

}   

