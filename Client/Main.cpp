#include <conio.h>

#include "Client.h"


int main()
{
	Client c;
	int choice;

	c.Initiation(); // Wywo³anie metody inicjuj¹cej Winsocka
	c.CreateSocket(); // Wywo³anie metody tworz¹cej Gniazdo
	c.GetAdress(); // Wywo³anie metody ustalaj¹cej adres IP oraz Port 

	if(c.Connect() == true) // Wywo³anie metody pozwalaj¹cej na po³¹czenie siê z serwerem
	{
		cout << "Press 1 if you want to start conversation with the client." << endl
		     << "Press 2 if you want to read a file from the server." << endl;
		cin >> choice;
		cout << endl;

		c.RunThread(choice); // Wywo³anie metody tworz¹cej nowy w¹tek (odbieraj¹cy dane lub odbieraj¹cy dane i zapisuj¹cy do pliku)
		c.Sending(); // Wywo³anie metody wysy³aj¹cej dane
	}

	WSACleanup();
	_getch();
	return 0;
}