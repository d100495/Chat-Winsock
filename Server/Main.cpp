#include <conio.h>

#include "Server.h"


int main()
{
	Server s;
	int choice;

	while (SOCKET_ERROR)
	{
		s.Initiation(); // Wywo³anie metody inicjuj¹cej Winsocka
		s.CreateSocket(); // Wywo³anie metody tworz¹cej Gniazdo
		s.GetAdress(); // Wywo³anie metody ustalaj¹cej adres IP oraz Port
		s.WaitingForConnection(); // Wywo³anie metody pozwalaj¹cej na po³¹czenie siê z klientem

		cout << "Press 1 if you want to start conversation with the client." << endl
			<< "Press 2 if you want to send a file to the client." << endl;
		cin >> choice;
		cout << endl;

		s.RunThread(choice); // Wywo³anie metody tworz¹cej nowy w¹tek (wysy³aj¹cy dane lub odczytuj¹cy plik i wysy³aj¹cy do klienta)
		s.Receiving(); // Wywo³anie metody odbieraj¹cej dane
	}

	WSACleanup();
	_getch();
	return 0;
}