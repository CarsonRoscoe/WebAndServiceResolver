#pragma once

#include "Globals.h"
#include <string>

//Prototype for our Service Name+Protocol to Port resolution
void ServiceToPort(std::string item, std::string fromProtocol);
//Prototype for our Port+Protocol to Service Name resolution
void PortToService(std::string item, std::string fromProtocol);

//Error Messages
#define ERRORSERVICE "<ERROR: Could not resolve to a service>"
#define ERRORPORT "<ERROR: Could not resolve to a port>"