# CSIPv6Manager

This program adds external IPv6 support to Counter Strike 1.6.

## Introduction

Counter Strike 1.6 supports by default only IPv4.
Due to this limitation, DS-Lite Internet users cannot host Counter Strike servers.
This limitation is circumvented with this program by transmitting the network packets over the Internet using the IPv6.
To do so, this program is started on the server and all client PC's. 
After that, the individual programs are linked to each other via the server IPv6 address.
The client PCs can then start Counter Strike 1.6 and connect to the server via the IP address 127.0.0.1.

## Installation

1. Clone this repository
2. Open CSIPv6Manager.pro in Qt Creator
3. Compile the project
4. Start CSIPv6Manager

OR

Download release binaries:
[Link to binaries](https://github.com/johbey/CSIPv6Manager/releases)

## How To

### Connect To Server

1. Open CSIPv6Manager
2. Paste server IPv6 Address to the text field
3. Click 'Start'
4. Open Counter Strike 1.6
5. Add Server with IP Address '127.0.0.1' to your favorites
6. Connect to the Server

![Client Page](https://github.com/johbey/CSIPv6Manager/blob/main/images/client.png?raw=true)

Care: 
You can change the IPv4 and IPv6 ports if its necessary.
Errors will be displayed at the bottom.

### Configure Server

1. Open hlds.exe
2. Start Counter Strike Server
3. Open CSIPv6Manager
4. Switch to the Server page (left top menu)
5. Click 'Start Server'
6. Send IPv6 Address to your clients
7. Open Port 37015 UDP

![Server Page](https://github.com/johbey/CSIPv6Manager/blob/main/images/server.png?raw=true)

Care: 
The connected client IP's will be listed in this page.
You can change the IPv4 and IPv6 ports if its necessary.
Errors will be displayed at the bottom.

## Attributions:
Icon https://www.flaticon.com/authors/smashicons<br>
Qt https://doc.qt.io/<br>
Classes Client and Server: [NGE] Kotnascher
