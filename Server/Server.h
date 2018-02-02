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
	SOCKET MainSocket; // gniazdo g³ówne
	SOCKET AcceptSocket; // gniazdo tymczasowe
	WORD wVersionRequested; // zmienna przydatna do inicjacji Winsocka
	WSADATA wsaData;
	sockaddr_in service; // zmienna potrzebna do ustalenia adresu
	HANDLE hOut; // Zmienna potrzebna do ustalenia koloru napisów konsoli

public:
	Server(); // Konstruktor klasy Server
	~Server(); // Destruktor klasy Server

	void Initiation(); // Metoda inicjuj¹ca Winsocka
	void CreateSocket(); // Metoda tworz¹ca Gniazdo
	void GetAdress(); // Metoda ustalaj¹ca adres IP oraz Port
	void WaitingForConnection(); // Metoda nas³uchuj¹ca na po³¹czenie
	string currentDateTime();
	void Sending(); // Metoda wysy³aj¹ca dane
	void Receiving(); // Metoda odbieraj¹ca dane

	void loadChatHistory();

	void RunThread(int); // Metoda tworz¹ca nowy w¹tek

	void SetCursor(int, int);
	void GetCursor(int&, int&);

	void ReadFile();
	int SendAll(int MainSocket, char* buffor, int* length);
};

#endif
