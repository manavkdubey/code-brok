#include "utilities.h"

crow::json::wvalue errorToJson(const std::string& errstring) {
    crow::json::wvalue response;
    response["error"] = errstring;
    return response;
}
