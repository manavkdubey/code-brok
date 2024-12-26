#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include "crow/json.h"

crow::json::wvalue errorToJson(const std::string& errstring);

#endif // UTILITIES_H
