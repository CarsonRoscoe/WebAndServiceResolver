/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: LocalConversions.cpp - An application that allows the user to input information and receive information
-- pertaining to either web domains or services.
--
-- PROGRAM: Assignment1
--
-- FUNCTIONS:
-- void ServiceToPort(std::string fromName, std::string fromProtocol) c
-- void PortToService(std::string fromPort, std::string fromProtocol)
--
-- DATE: January 14th, 2016
--
-- REVISIONS: January 14th, 2016: Started and finished the program.
--			  January 16th, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- NOTES:
-- This file contains the function calls used to resolve a service + protocol to a port number, or a port number + protocol
-- to a service. Called by WndProc in the Main.cpp file.
----------------------------------------------------------------------------------------------------------------------*/
#include "LocalConversions.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ServiceToPort
--
-- DATE: January 14th, 2016
--
-- REVISIONS: January 14th, 2016: Started and finished the program
--			  January 16th, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void ServiceToPort(std::string fromName, std::string fromProtocol)
--					string fromName: The name of the service to resolve a port from.
--					string fromProtocol: The name of the protocol the service uses.
--
-- RETURNS: void.
--
-- NOTES:
-- Takes in a services name. It then uses Winsock to get data regarding that service. It retrieves the port that service
-- is currently running on. It then sends messages to the corresponding Win32 UI controls to update themselves to display 
-- this new information.
-- 
-- On a failed resolve, it sends a message to the main label displaying an error message.
----------------------------------------------------------------------------------------------------------------------*/
void ServiceToPort(std::string fromName, std::string fromProtocol) {
	//Turn the name of the service into a char * for the c calls
	const char * serviceName = fromName.c_str();
	//Turn the name of the protocol into a char * for the c calls
	const char * protocol = fromProtocol.c_str();
	//Servant struct which will hold the data of the port/service
	struct servent *sv;
	//Used to determine which version of Winsock to use. In this case, version 2.2
	WORD wVersionRequested = MAKEWORD(2, 2);
	//Will hold data needed for WSAStartup. In our case, unused.
	WSADATA wsaData;

	//Open up a Winsock v2.2 session
	WSAStartup(wVersionRequested, &wsaData);

	//Get the data about the service by the name and protocol data passed in as parameters.
	sv = getservbyname(serviceName, protocol);

	//If the service is NULL, it failed to resolve get the service data. Display an error message.
	if (sv == NULL) {
		SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)ERRORPORT);
		return;
	}

	//Set the main labels text to be the name of the service resolved.
	SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)std::to_string(ntohs(sv->s_port)).c_str());

	//Mandatory cleanup code for the Winsock session.
	WSACleanup();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: PortToService
--
-- DATE: January 14th, 2016
--
-- REVISIONS: January 14th, 2016: Started and finished the program
--			  January 16th, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void PortToService(std::string fromPort, std::string fromProtocol)
--					string fromPort: The port in string form to resolve a service from.
--					string fromProtocol: The name of the protocol the service uses on the given port.
--
-- RETURNS: void.
--
-- NOTES:
-- Takes in a port number as a string. It then uses Winsock to get data regarding what service is using the given port.
-- It retrieves the service that uses the given port. It then sends messages to the corresponding Win32 UI controls to 
-- update themselves to display this new information.
--
-- On a failed resolve, it sends a message to the main label displaying an error message.
----------------------------------------------------------------------------------------------------------------------*/
void PortToService(std::string fromPort, std::string fromProtocol) {
	//Turn the name of the port into a char * for the c function calls.
	const char * port = fromPort.c_str();
	//Turn the name of the protocol into a char * for the c function calls.
	const char * protocol = fromProtocol.c_str();
	//Int that will hold the port number once it is resolved.
	int s_port;
	//Servant struct which will hold the data of the port/service
	struct servent *sv;
	//Used to determine which version of Winsock to use. In this case, version 2.2
	WORD wVersionRequested = MAKEWORD(2, 2);
	//Will hold data needed for WSAStartup. In our case, unused.
	WSADATA wsaData;

	//Open up a Winsock v2.2 session
	WSAStartup(wVersionRequested, &wsaData);

	//Store the port number into a temporary int
	s_port = atoi(port);

	//Store the service data into sv and resolve that service by the port number and the protocol
	sv = getservbyport(htons(s_port), protocol);

	//If sv is NULL, failed to resolve to a service. Display an error message.
	if (sv == NULL)
	{
		SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)ERRORSERVICE);
		return;
	}

	//Set the main labels text to be the name of the service resolved.
	SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)sv->s_name);

	//Mandatory cleanup code for the Winsock session.
	WSACleanup();
}