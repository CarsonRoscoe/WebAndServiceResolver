#pragma once

#include "Globals.h"
#include <string>

//Prototype for the IP to Webhost+Aliases resolution
void IPToWeb(std::string item);
//Prototype for the Webhost to Webhost+Aliases+IPs resolution
void WebToIP(std::string item);

//Used as the keyword for wanting to get the users local IP address.
#define LOCAL "local"

//Error Messages
#define ERRORIP "<ERROR: Could not resolve to an IP>"
#define ERRORWEBSITE "<ERROR: Could not resolve to a host name>"