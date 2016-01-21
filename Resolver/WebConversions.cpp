/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: WebConversions.cpp - An application that allows the user to input information and receive information
-- pertaining to either web domains or services.
--
-- PROGRAM: Assignment1
--
-- FUNCTIONS:
-- void WebToIP(std::string toConvert)
-- void IPToWeb(std::string convertFrom)
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
-- This file contains the function calls used to either resolve a websites' hostname to a hostname, IP and aliases, or
-- to resolve a websites' IP to a hostname and aliases. These functions are used by WndProc in Main.cpp.
----------------------------------------------------------------------------------------------------------------------*/
#include "WebConversions.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WebToIP
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
-- INTERFACE: void WebToIP(std::string toConvert)
--					string toConvert: The hostname input by the user to be resolved to ip/hostname/aliases.
--
-- RETURNS: void.
--
-- NOTES:
-- Takes in a hostname. It then uses Winsock to get data regarding that hostname. The data it searches for is the
-- original hostname (as the name entered by the user may, in fact, be an aliase), all the IP addresses associated with
-- that hostname, and all aliases associated with that hostname. It then sends messages to the corresponding Win32 UI
-- controls to update themselves to display this new information.
-- 
-- On a failed resolve, it sends a message to the main label displaying an error message.
----------------------------------------------------------------------------------------------------------------------*/
void WebToIP(std::string toConvert) {
	//Will hold all the extra ips for the given hostname
	std::vector<std::string> ips;
	//Will hold the hostname of the main site
	std::vector<std::string> aliases;
	//String that holds the main websites hostname
	std::string mainSite;
	//Store the website name that will be resolved from into a char* to use with C functions
	const char * item;
	//Holds the data of the webhost
	struct hostent *hp;
	//Holds the information about the webhosts address
	struct in_addr my_addr, *addr_p;
	//Will be used to iterate through a loop of addresses in a list.
	char **addressPointer;
	//String for the IP address
	char ip_address[256];
	//Used to determine which version of Winsock to use. In this case, version 2.2
	WORD wVersionRequested = MAKEWORD(2, 2);
	//Will hold data needed for WSAStartup. In our case, unused.
	WSADATA wsaData;

	//If the user entered the define for local, it sets the item string to be empty.
	//The reason is because the default behaviour for the API used is an empty string
	//returns the local IP and information.
	if (toConvert == LOCAL) {
		item = "";
	}
	//In the case of the user leaving the string empty, error. 
	else if (toConvert == "") {
		SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)ERRORIP);
		return;
	}
	//In any other case, continue on as they entered either a valid or invalid hostname.
	else {
		item = &toConvert[0];
	}

	//Open up a Winsock v2.2 session
	WSAStartup(wVersionRequested, &wsaData);

	//Create a space of memory for the address pointer to use in the future.
	addr_p = (struct in_addr*)malloc(sizeof(struct in_addr));
	addr_p = &my_addr;

	//Gets the webhost data and stores it into hp. If hp is null, there was an error. Display the error.
	if ((hp = gethostbyname(item)) == NULL) {
		SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)ERRORIP);
		return;
	}

	//For each address in the list, put the IP into the ip vector and the aliases into the aliases vector.
	for (addressPointer = hp->h_addr_list; *addressPointer != 0; addressPointer++) {
		struct in_addr in;
		char **q;

		memcpy(&in.s_addr, *addressPointer, sizeof(in.s_addr));
		ips.push_back(inet_ntoa(in));
		for (q = hp->h_aliases; *q != 0; q++)
			aliases.push_back(*q);
	}

	//Store the main hostname in mainSite
	mainSite = hp->h_name;

	//Do the mandatory cleanup code
	WSACleanup();

	//Set the main center label to be the mainSite string
	SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)mainSite.c_str());

	//Add all the IPs to the IP dropdown and default it to the first one.
	for (auto ip : ips) {
		SendMessage(hDropDown3, CB_ADDSTRING, 0, (LPARAM)ip.c_str());
	}
	SendMessage(hDropDown3, CB_SETCURSEL, 0, (LPARAM)0);

	//Add all the aliases to the aliases dropdown and default it to the first one.
	for (auto aliase : aliases) {
		SendMessage(hDropDown2, CB_ADDSTRING, 0, (LPARAM)aliase.c_str());
	}
	SendMessage(hDropDown2, CB_SETCURSEL, 0, (LPARAM)0);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: IPToWeb
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
-- INTERFACE: void IPToWeb(std::string toConvert)
--					string toConvert: The dotted IP address input by the user to be resolved to hostname and aliases.
--
-- RETURNS: void.
--
-- NOTES:
-- Takes in a dotted IP address. It then uses Winsock to get data regarding that IP. The data it searches for is the
-- original hostname, and all aliases associated with that hostname. It then sends messages to the corresponding Win32 
-- UI controls to update themselves to display this new information.
--
-- On a failed resolve, it sends a message to the main label displaying an error message.
----------------------------------------------------------------------------------------------------------------------*/
void IPToWeb(std::string convertFrom) {
	//Will hold the hostnames of all aliases for the main site
	std::vector<std::string> aliases;
	//Will hold the hostname of the main site
	std::string mainSite;
	//Our dotted IP address in string form put into a char * to use with the C functions
	const char * item = convertFrom.c_str();
	//Holds the data of the webhost
	struct hostent *hp;
	//Holds the information about the webhosts address
	struct in_addr my_addr, *addr_p;
	//Will be used to iterate through a loop of addresses in a list.
	char **addressPointer;
	//String for the IP address
	char ip_address[256];
	//Used to determine which version of Winsock to use. In this case, version 2.2
	WORD wVersionRequested = MAKEWORD(2, 2);
	//Will hold data needed for WSAStartup. In our case, unused.
	WSADATA wsaData;

	//Open up a Winsock v2.2 session
	WSAStartup(wVersionRequested, &wsaData);

	//Create a space of memory for the address pointer to use in the future.
	addr_p = (struct in_addr*)malloc(sizeof(struct in_addr));
	addr_p = &my_addr;

	// Copy IP address  into ip_address
	for (size_t i = 0; i < 256; i++)
		ip_address[i] = item[i];
	//strcpy(ip_address, item);
	addr_p->s_addr = inet_addr(ip_address);

	//Gets the webhost data and stores it into hp. If hp is null, there was an error. Display the error.
	if ((hp = gethostbyaddr((char *)addr_p, PF_INET, sizeof(my_addr))) == NULL) {
		SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)ERRORWEBSITE);
		return;
	}

	//For each address in the list, put the IP into the ip vector and the aliases into the aliases vector.
	for (addressPointer = hp->h_addr_list; *addressPointer != 0; addressPointer++) {
		struct in_addr in;
		char **q;

		memcpy(&in.s_addr, *addressPointer, sizeof(in.s_addr));
		for (q = hp->h_aliases; *q != 0; q++)
			aliases.push_back(*q);
	}

	//Store the main webhost name into the char* mainSite.
	mainSite = hp->h_name;

	//Do the mandatory cleanup code for Winsock
	WSACleanup();

	//Add all the aliases to the aliases dropdown and default it to the first one.
	for (auto aliase : aliases) {
		SendMessage(hDropDown2, CB_ADDSTRING, 0, (LPARAM)aliase.c_str());
	}
	SendMessage(hDropDown2, CB_SETCURSEL, 0, (LPARAM)0);

	//Set the text of the main label to be the mainSite string
	SendMessage(hText1, WM_SETTEXT, 0, (LPARAM)mainSite.c_str());
}