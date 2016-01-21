# Webhost And Service Resolver

A small GUI program built in C++ using Win32 API, built with the intention of learning to use windows sockets. It uses the windows sockets to gather information about webhosts, IP addresses, services and open ports.

The program allows the user to:
* Enter a webhost name(or alias) and resolve the webhosts name, all aliases and all IP addresses associated with that domain name.
* Enter an IP address and resolve the webhost name associated with that IP address.
* Enter a service name & protocol then resolve it to the port that the service listed is using.
* Enter a port number & protocol then resolve it to the service name currently using that port.

# How To Run

After cloning or downloading, run the Resolver.exe application.

# How To Compile

Clone or download the files onto your Windows system.

Open up the Resolver.sln file in Visual Studio and run the program. Everything should already be linked.

# Design Document

The design document for the program is the DesignDocument.pdf. That is where you will see state diagrams regarding how the program was planned to be structured, along with pseudocode.

# Testing Document

The testing document for the program is the TestingDocument.pdf. This is where all the test cases were run with test summaries at the top and more detailed test breakdowns (with screenshots) in the pages proceeding the summary.
