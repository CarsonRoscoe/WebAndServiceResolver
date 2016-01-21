#pragma once

#include <Windows.h>
#include <string>

//Prototype for WndProc
INT_PTR CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Global variable that dictates which mode our application is currently in
Conversion conversion;

//Handler for our Dialog, essentially for our window.
HWND hDlg;

//////Handler for all of our dialogs controls//////
//Buttons//
HWND hButton;
//Edit texts//
HWND hConvertFrom;
HWND hProtocol;
//Statics (Labels)//
HWND hText1;
HWND hText2;
HWND hText3;
HWND hText4;
HWND hText5;
//Dropdown Boxes(Combo Boxes)//
HWND hDropDown1;
HWND hDropDown2;
HWND hDropDown3;