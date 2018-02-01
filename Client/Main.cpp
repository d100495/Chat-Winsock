#include <conio.h>

#include "Client.h"


int main()
{
	Client c;
	int choice;

	c.Initiation(); // Wywo�anie metody inicjuj�cej Winsocka
	c.CreateSocket(); // Wywo�anie metody tworz�cej Gniazdo
	c.GetAdress(); // Wywo�anie metody ustalaj�cej adres IP oraz Port 

	if(c.Connect() == true) // Wywo�anie metody pozwalaj�cej na po��czenie si� z serwerem
	{
		cout << "Press 1 if you want to start conversation with the client." << endl
		     << "Press 2 if you want to read a file from the server." << endl;
		cin >> choice;
		cout << endl;

		c.RunThread(choice); // Wywo�anie metody tworz�cej nowy w�tek (odbieraj�cy dane lub odbieraj�cy dane i zapisuj�cy do pliku)
		c.Sending(); // Wywo�anie metody wysy�aj�cej dane
	}

	WSACleanup();
	_getch();
	return 0;
}