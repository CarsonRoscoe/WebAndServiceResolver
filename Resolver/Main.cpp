/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Main.cpp - An application that allows the user to input information and receive information
-- pertaining to either web domains or services. 
--
-- PROGRAM: Assignment1
--
-- FUNCTIONS:
-- int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
-- INT_PTR CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
-- The program will take in information from the user regarding web domains, IP addresses, ports and/or services
-- and display information resolved from the input to the UI.
-- 
-- If the user enters a dotted IP address, they receive information pertaining to that IP's host name and any aliases. 
-- If the user enters a host name (or aliase), it receives information pertaining to that host name, IPs and aliases. 
-- If the user enters a service name and protocol, it returns which port the service is being run on.
-- If the user enters a port and a protocol, it returns which service is using that port.
--
-- The UI is done using basic Win32 calls.
----------------------------------------------------------------------------------------------------------------------*/
#include "Globals.h"
#include "WinMain.h"
#include "LocalConversions.h"
#include "WebConversions.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: wWinMain
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
-- INTERFACE: int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
--					HINSTANCE hInstance: Handle to this programs instance.
--					HINSTANCE: Handle to previous instance of program. (Unused)
--					PWSTR pCmdLine: Pointer to command line arguments. (Unused)
--					int nCmdShow: Controls how the window being built will be displayed
--
-- RETURNS: int regarding the exit code of the program
--
-- NOTES:
-- wWinMain is the entry point of our program. It creates the dialog, which is the effective UI of our program,
-- and then displays that dialog. It then gets the handle for all items in the dialog box to be manipulated later and
-- then assigns which UI elements will be visible/which wont be for the starting of our program. It lays out the UI
-- in the way that we would the Webhost to IP selection would, as that is the default mode of the program.
--
-- It then assigns the four modes (Webhost to IP, IP to Webhost, Service name to port and port to service name) to the
-- corresponding dropdown box.
--
-- Finally it goes into a while(GetMessage) loop, which is the messaging system that allows this program to function.
----------------------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR pCmdLine, int nCmdShow) {
	//Create our dialog, which is essentia4lly our pre-set UI window.
	hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, WndProc, 0);
	
	//Show the dialog so it is visible
	ShowWindow(hDlg, nCmdShow);

	//Get the handles for all of the UI controls in the dialog
	hButton = GetDlgItem(hDlg, IDC_BUTTON1);
	hConvertFrom = GetDlgItem(hDlg, IDC_EDIT1);
	hProtocol = GetDlgItem(hDlg, IDC_EDIT2);
	hText1 = GetDlgItem(hDlg, IDC_STATIC1);
	hText2 = GetDlgItem(hDlg, IDC_STATIC2);
	hText3 = GetDlgItem(hDlg, IDC_STATIC3);
	hText4 = GetDlgItem(hDlg, IDC_STATIC4);
	hText5 = GetDlgItem(hDlg, IDC_STATIC5);
	hDropDown1 = GetDlgItem(hDlg, IDC_COMBO1);
	hDropDown2 = GetDlgItem(hDlg, IDC_COMBO2);
	hDropDown3 = GetDlgItem(hDlg, IDC_COMBO3);

	//Notify all the UI controls how to lay themselves out for our default setup.
	//Our default setup is "Webhost to IP" mode.
	SetWindowText(hText2, WEB);
	SetWindowText(hText4, ALIASES);
	SetWindowText(hText5, IPS);
	ShowWindow(hText3, SW_HIDE);
	ShowWindow(hText4, SW_SHOW);
	ShowWindow(hText5, SW_SHOW);
	ShowWindow(hProtocol, SW_HIDE);
	ShowWindow(hDropDown2, SW_SHOW);
	ShowWindow(hDropDown3, SW_SHOW);

	//Set our default mode to toIP, which is short for WebhostToIP.
	conversion = toIP;

	//Foreach item in our list of modes, add it to our dropdown list
	for (size_t i = 0; i < DROPDOWNSIZE; i++)
		SendMessage(hDropDown1, CB_ADDSTRING, 0, (LPARAM)messages[i]);
	//And set the dropdown to default select the first one in the list.
	SendMessage(hDropDown1, CB_SETCURSEL, 0, (LPARAM)0);

	// Run the message loop.
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
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
-- INTERFACE: INT_PTR CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
--					HWND hwnd: The handle to the window that the message is being sent from.
--					UINT uMsg: The message invoked.
--					WPARAM wParam: Additional messaging information 
--					LPARAM lParam: Additional messaging information
--
-- RETURNS: int regarding the reason the function returned.
--
-- NOTES:
-- Any time a message is sent, whether from keyboard keys being pressed or a UI element being used, WndProc is
-- notified of this message. The function is essentially used by our program to allow the UI elements to communicate
-- with the backend logic.
--
-- If the Resolve button is pressed, WndProc is notified via the messages in the parameters and allows us to
-- call the appropriate conversion function depending on what our conversion enum is currently set to.
--
-- If our dropdown combobox has the selected item changed, this is invoked and we modify the UI to re-layout it
-- based on what our new selection was.
--
-- If the exit button is clicked, we exit the program here.
--
-- If the message says to paint the screen, we repaint it here.
----------------------------------------------------------------------------------------------------------------------*/
INT_PTR CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		//In the case of the command being from our UI
		case WM_COMMAND:
			//If the UI control was our button and that button was clicked
			if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
				//Get the message from the first edit box and store it into bufferToString1.
				//The name is generic as the box is multipurposed.
				TCHAR buffer1[BUFFERSIZE];
				GetWindowText(hConvertFrom, buffer1, BUFFERSIZE);
				std::string bufferToString1(&buffer1[0]);

				//Get the message from the second edit box and store it into protocolString.
				TCHAR buffer2[BUFFERSIZE];
				GetWindowText(hProtocol, buffer2, BUFFERSIZE);
				std::string protocolString(&buffer2[0]);

				//Tell the aliases and IP's comboboxes to reset their content, as they are
				//either no longer going to be displayed, or their information is outdated now anyway.
				SendDlgItemMessage(hDlg, IDC_COMBO2, CB_RESETCONTENT, 0, 0);
				SendDlgItemMessage(hDlg, IDC_COMBO3, CB_RESETCONTENT, 0, 0);

				//Depending on which conversion our mode is set to, we will pass in the strings above
				//into the appropriate message.
				switch (conversion) {
					//In the case of Webhost to IP, call WebToIP.
					case toIP: 
						WebToIP(bufferToString1);
						break;
					//In the case of IP to Webhost, call IPToWeb
					case toWeb:
						IPToWeb(bufferToString1);
						break;
					//In the case of Port To Service Name, call PortToService
					case toService:
						PortToService(bufferToString1, protocolString);
						break;
					//In the case of Service Name To Port, call ServiceToPort
					case toPort:
						ServiceToPort(bufferToString1, protocolString);
						break;
				}
				return 0;
			}
			//If the UI control was the dropdown box and the action was the selection changed
			//i.e. someone changed modes
			if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == IDC_COMBO1) {
				//Depending on which mode it was changed to, re-layout the screen.
				switch (ComboBox_GetCurSel(hDropDown1)) {
					//In the case of an error, default to the Webhost to IP mode and lay the UI out accordingly.
					case CB_ERR: 
						SendMessage(hDropDown1, CB_SETCURSEL, 0, (LPARAM)0);
						SetWindowText(hText2, WEB);
						SetWindowText(hText4, ALIASES);
						SetWindowText(hText5, IPS);
						ShowWindow(hText3, SW_HIDE);
						ShowWindow(hText4, SW_SHOW);
						ShowWindow(hText5, SW_SHOW);
						ShowWindow(hProtocol, SW_HIDE);
						ShowWindow(hDropDown2, SW_SHOW);
						ShowWindow(hDropDown3, SW_SHOW);
						conversion = toIP;
						return 0;
					//In the case of webhost to IP mode, lay it out accordingly.
					case 0:
						SetWindowText(hText2, WEB);
						SetWindowText(hText4, ALIASES);
						SetWindowText(hText5, IPS);
						ShowWindow(hText1, SW_SHOW);
						ShowWindow(hText2, SW_SHOW);
						ShowWindow(hText3, SW_HIDE);
						ShowWindow(hText4, SW_SHOW);
						ShowWindow(hText5, SW_SHOW);
						ShowWindow(hProtocol, SW_HIDE);
						ShowWindow(hDropDown2, SW_SHOW);
						ShowWindow(hDropDown3, SW_SHOW);
						//And set our conversion mode to Webhost to IP
						conversion = toIP;
						return 0;
					//In the case of IP to web mode, lay it out accordingly
					case 1:
						SetWindowText(hText2, IP);
						SetWindowText(hText3, ALIASES);
						ShowWindow(hText1, SW_SHOW);
						ShowWindow(hText2, SW_SHOW);
						ShowWindow(hText3, SW_HIDE);
						ShowWindow(hText4, SW_HIDE);
						ShowWindow(hText5, SW_HIDE);
						ShowWindow(hProtocol, SW_HIDE);
						ShowWindow(hDropDown2, SW_HIDE);
						ShowWindow(hDropDown3, SW_HIDE);
						//And set our conversion mode to IP to Webhost
						conversion = toWeb;
						return 0;
					case 2:
					//In the case of service name to port mode, lay it out accordingly
						SetWindowText(hText2, SERVICE);
						SetWindowText(hText3, PROTOCOL);
						ShowWindow(hDropDown2, SW_HIDE);
						ShowWindow(hDropDown3, SW_HIDE);
						ShowWindow(hText1, SW_SHOW);
						ShowWindow(hText2, SW_SHOW);
						ShowWindow(hText3, SW_SHOW);
						ShowWindow(hText4, SW_HIDE);
						ShowWindow(hText5, SW_HIDE);
						ShowWindow(hProtocol, SW_SHOW);
						//And set our conversion mode to service name to port.
						conversion = toPort;
						return 0;
					//In the case of port to service name, lay it out accordingly
					case 3:
						SetWindowText(hText2, PORT);
						SetWindowText(hText3, PROTOCOL);
						ShowWindow(hDropDown2, SW_HIDE);
						ShowWindow(hDropDown3, SW_HIDE);
						ShowWindow(hText1, SW_SHOW);
						ShowWindow(hText2, SW_SHOW);
						ShowWindow(hText3, SW_SHOW);
						ShowWindow(hText4, SW_HIDE);
						ShowWindow(hText5, SW_HIDE);
						ShowWindow(hProtocol, SW_SHOW);
						//And set the conversion mode to port to service name.
						conversion = toService;
						return 0;
				}
			}
			return 0;
		//In the case of the exit button, exit the program.
		case WM_DESTROY:
			DestroyWindow(hDlg);
			PostQuitMessage(0);
			return 0;
		//In the case of repainting the screen, repaint the screen.
		case WM_PAINT: {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

				EndPaint(hwnd, &ps);
			}
			return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
