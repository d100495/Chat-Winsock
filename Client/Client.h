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
	SOCKET MainSocket; // gniazdo g³ówne
	SOCKET AcceptSocket; // gniazdo tymczasowe
	WORD wVersionRequested; // zmienna przydatna do inicjacji Winsocka
	WSADATA wsaData;
	sockaddr_in service; // zmienna potrzebna do ustalenia adresu
	HANDLE hOut; // Zmienna potrzebna do ustalenia koloru napisów konsoli

public:
	Client(); // Konstruktor klasy Client
	~Client(); // Destruktor klasy Client

	void Initiation(); // Metoda inicjuj¹ca Winsocka
	void CreateSocket(); // Metoda tworz¹ca Gniazdo
	void GetAdress(); // Metoda ustalaj¹ca adres IP oraz Port
	bool Connect(); // Metoda pozwalaj¹ca na po³¹czenie siê z serwerem
	void Sending(); // Metoda wysy³aj¹ca dane
	void Receiving(); // Metoda odbieraj¹ca dane

	void loadChatHistory();

	void RunThread(int); // Metoda tworz¹ca nowy w¹tek

	string currentDateTime();//Zwraca aktualny czas systemowy


	void SetCursor(int, int);
	void GetCursor(int&, int&);

	void WriteFile();
};

#endif