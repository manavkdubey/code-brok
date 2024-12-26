#ifndef DB_H
#define DB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libsql.h"  
#ifdef __cplusplus
}
#endif

#include <string>



void setupDatabase();
std::string insertUser(const std::string& name);
std::string getAllUsers();

#endif // DB_H
