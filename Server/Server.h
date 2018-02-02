#ifndef __Server_H__
#define __Server_H__

#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <conio.h>
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")
#include <string>

using namespace std;


class Server
{
	SOCKET MainSocket; // gniazdo g��wne
	SOCKET AcceptSocket; // gniazdo tymczasowe
	WORD wVersionRequested; // zmienna przydatna do inicjacji Winsocka
	WSADATA wsaData;
	sockaddr_in service; // zmienna potrzebna do ustalenia adresu
	HANDLE hOut; // Zmienna potrzebna do ustalenia koloru napis�w konsoli

public:
	Server(); // Konstruktor klasy Server
	~Server(); // Destruktor klasy Server

	void Initiation(); // Metoda inicjuj�ca Winsocka
	void CreateSocket(); // Metoda tworz�ca Gniazdo
	void GetAdress(); // Metoda ustalaj�ca adres IP oraz Port
	void WaitingForConnection(); // Metoda nas�uchuj�ca na po��czenie
	string currentDateTime();
	void Sending(); // Metoda wysy�aj�ca dane
	void Receiving(); // Metoda odbieraj�ca dane

	void loadChatHistory();

	void RunThread(int); // Metoda tworz�ca nowy w�tek

	void SetCursor(int, int);
	void GetCursor(int&, int&);

	void ReadFile();
	int SendAll(int MainSocket, char* buffor, int* length);
};

#endif
