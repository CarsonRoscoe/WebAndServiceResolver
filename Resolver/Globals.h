#pragma once

//Hookup ws2_32.dll for the Winsock (Windows Sockets) API
#pragma comment (lib,"ws2_32.lib")

#include <windows.h>
#include <windowsx.h>
#include <string>
#include <vector>
#include "resource.h"

//Get rid of all warning
#define _CRT_SECURE_NO_WARNINGS

//Constants
#define DROPDOWNSIZE 4
#define DROPDOWNITEMLENGTH 16
#define BUFFERSIZE 1024

//UI Control Names
#define ALIASES "Aliases"
#define IPS "IPs"
#define WEB "Website"
#define IP "IP"
#define SERVICE "Service"
#define PORT "Port"
#define PROTOCOL "Protocol"

//Drop down selection list
static const TCHAR messages[DROPDOWNSIZE][DROPDOWNITEMLENGTH]{
	"Website To IP", "IP To Website", "Service To Port", "Port To Service"
};

//Enum that defines which mode the application is in.
enum Conversion {
	toWeb, toIP, toPort, toService
};

//Global variable that dictates which mode our application is currently in
extern Conversion conversion;

//Handler for our Dialog, essentially for our window.
extern HWND hDlg;

//////Handler for all of our dialogs controls//////
//Buttons//
extern HWND hButton;
//Edit texts//
extern HWND hConvertFrom;
extern HWND hProtocol;
//Statics (Labels)//
extern HWND hText1;
extern HWND hText2;
extern HWND hText3;
extern HWND hText4;
extern HWND hText5;
//Dropdown Boxes(Combo Boxes)//
extern HWND hDropDown1;
extern HWND hDropDown2;
extern HWND hDropDown3;