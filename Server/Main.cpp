#include <conio.h>

#include "Server.h"


int main()
{
	Server s;
	int choice;

	while (SOCKET_ERROR)
	{
		s.Initiation(); // Wywołanie metody inicjującej Winsocka
		s.CreateSocket(); // Wywołanie metody tworzącej Gniazdo
		s.GetAdress(); // Wywołanie metody ustalającej adres IP oraz Port
		s.WaitingForConnection(); // Wywołanie metody pozwalającej na połączenie się z klientem

		cout << "Press 1 if you want to start conversation with the client." << endl
			<< "Press 2 if you want to send a file to the client." << endl;
		cin >> choice;
		cout << endl;

		s.RunThread(choice); // Wywołanie metody tworzącej nowy wątek (wysyłający dane lub odczytujący plik i wysyłający do klienta)
		s.Receiving(); // Wywołanie metody odbierającej dane
	}

	WSACleanup();
	_getch();
	return 0;
}