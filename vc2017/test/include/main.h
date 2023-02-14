#pragma once
#include "PRADIS_scheme_parser.h"
#include <vector>
extern "C" bool load(std::vector<sch_parser_component>&list, std::string path);
extern "C" void close();