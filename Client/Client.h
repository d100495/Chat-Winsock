#ifndef __Client_H__
#define __Client_H__

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")
#include <vector>
#include <string>

using namespace std;


class Client
{
	SOCKET MainSocket; // gniazdo g��wne
	SOCKET AcceptSocket; // gniazdo tymczasowe
	WORD wVersionRequested; // zmienna przydatna do inicjacji Winsocka
	WSADATA wsaData;
	sockaddr_in service; // zmienna potrzebna do ustalenia adresu
	HANDLE hOut; // Zmienna potrzebna do ustalenia koloru napis�w konsoli

public:
	Client(); // Konstruktor klasy Client
	~Client(); // Destruktor klasy Client

	void Initiation(); // Metoda inicjuj�ca Winsocka
	void CreateSocket(); // Metoda tworz�ca Gniazdo
	void GetAdress(); // Metoda ustalaj�ca adres IP oraz Port
	bool Connect(); // Metoda pozwalaj�ca na po��czenie si� z serwerem
	void Sending(); // Metoda wysy�aj�ca dane
	void Receiving(); // Metoda odbieraj�ca dane

	void loadChatHistory();

	void RunThread(int); // Metoda tworz�ca nowy w�tek

	string currentDateTime();//Zwraca aktualny czas systemowy


	void SetCursor(int, int);
	void GetCursor(int&, int&);

	void WriteFile();
};

#endif