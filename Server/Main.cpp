#include <conio.h>

#include "Server.h"


int main()
{
	Server s;
	int choice;

	while (SOCKET_ERROR)
	{
		s.Initiation(); // Wywo�anie metody inicjuj�cej Winsocka
		s.CreateSocket(); // Wywo�anie metody tworz�cej Gniazdo
		s.GetAdress(); // Wywo�anie metody ustalaj�cej adres IP oraz Port
		s.WaitingForConnection(); // Wywo�anie metody pozwalaj�cej na po��czenie si� z klientem

		cout << "Press 1 if you want to start conversation with the client." << endl
			<< "Press 2 if you want to send a file to the client." << endl;
		cin >> choice;
		cout << endl;

		s.RunThread(choice); // Wywo�anie metody tworz�cej nowy w�tek (wysy�aj�cy dane lub odczytuj�cy plik i wysy�aj�cy do klienta)
		s.Receiving(); // Wywo�anie metody odbieraj�cej dane
	}

	WSACleanup();
	_getch();
	return 0;
}