#include <conio.h>

#include "Client.h"


int main()
{
	Client c;
	int choice;

	c.Initiation(); // Wywołanie metody inicjującej Winsocka
	c.CreateSocket(); // Wywołanie metody tworzącej Gniazdo
	c.GetAdress(); // Wywołanie metody ustalającej adres IP oraz Port 

	if (c.Connect() == true) // Wywołanie metody pozwalającej na połączenie się z serwerem
	{
		cout << "Press 1 if you want to start conversation with the client." << endl
			<< "Press 2 if you want to read a file from the server." << endl;
		cin >> choice;
		cout << endl;

		c.RunThread(choice); // Wywołanie metody tworzącej nowy wątek (odbierający dane lub odbierający dane i zapisujący do pliku)
		c.Sending(); // Wywołanie metody wysyłającej dane
	}

	WSACleanup();
	_getch();
	return 0;
}