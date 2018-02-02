#include "Client.h"


Client::Client()
{
	MainSocket = SOCKET_ERROR;
	AcceptSocket = SOCKET_ERROR;
}


Client::~Client() { }


void Client::Initiation() //Definicja metody Initiation
{
	wVersionRequested = MAKEWORD(2,2); 
	WSAStartup(wVersionRequested, &wsaData); // Wybór wersji 2.2 Winsocka
}


void Client::CreateSocket() //Definicja metody CreateSocket
{
	MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Domena, typ gniazda, protokó³
}


void Client::GetAdress() //Definicja metody GetAdress
{
	service.sin_family = AF_INET; // Rodzina adresów
	service.sin_addr.s_addr = inet_addr("127.0.0.1"); // Adres IP serwera
	service.sin_port = htons(55555); // Numer Portu
}


bool Client::Connect() //Definicja metody Connect
{
	if (connect(MainSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
		cout << "Client: connect() - Failed to connect." << endl;
        WSACleanup();
        return false;
    }
    else
    {
		cout << "Client is ready to send and read data" << endl;
		return true;
    }
}


void Client::Sending() //Definicja metody Sending
{
	// Wysy³anie danych do serwera
	int bytesSentTotal = 0;
	int bytesSent;
	hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	int wordCount = 1;
	int wordCountSum = 0;
	int charCount = 0;
	int charCountSum = 0;
	clock_t cto;                         //poczatek zegara
	cto = clock();
	while(true)
	{
		char sendbuf[200] = "";
		cin.getline(sendbuf,200);

		if (strncmp(sendbuf, "INFO", 4)) {
			charCount = strlen(sendbuf);
			for (int i = 0; i < strlen(sendbuf); i++)
			{
				if (sendbuf[i] == ' ' && sendbuf[i + 1] != ' ') wordCount++;
				if (sendbuf[i] == ' ') charCount--;
			}
			wordCountSum += wordCount;
			charCountSum += charCount;
		}

		int x,y;
		GetCursor(x, y);
		y--;
		x = 0;
		SetCursor(x, y);

		if (strncmp(sendbuf, "INFO", 4)) {
			bytesSent = send(MainSocket, sendbuf, strlen(sendbuf), 0);
			bytesSentTotal += bytesSent;
		}

		if(bytesSent == SOCKET_ERROR)
		{
			//cout << "Client: send() error " << WSAGetLastError() << "." << endl;
			cout << "Client: lost connection with server!" << endl;
			break;
		}	
		else if (!strncmp(sendbuf, "CLOSE", 7))
		{
			cout << "Client: Connection Closed." << endl;
			Sleep(3000);
			WSACleanup();
		}
		else if (!strncmp(sendbuf, "INFO", 4)) {
			clock_t ct;                   //koniec zegara
			ct = clock();
			double time = (double)(ct - cto) / CLOCKS_PER_SEC;
			cout << "Wyslano slow lacznie: " << wordCountSum - 1 << " Wyslano znakow lacznie " << charCountSum << endl;
			cout << "Rozmowa trwa: " << time << "sec" << endl;
			cout << "Waga wyslanych danych: " << bytesSentTotal << "byte" << endl;
		}
		else if (bytesSent != 0)
		{
			//cout << "Bytes sent: " << bytesSent << endl;
			wordCount = 1;
			cout << "Client: " << sendbuf << endl;
			
		}
	}
}

void Client::Receiving() //Definicja metody Receiving
{
	// Odbieranie danych z serwera
	int bytesRecv = SOCKET_ERROR;
	hOut = GetStdHandle( STD_OUTPUT_HANDLE );

    while(true)
	{
		char recvbuf[200] = "";
		bytesRecv = recv(MainSocket, recvbuf, 200, 0);

        if (bytesRecv == SOCKET_ERROR)
		{
			cout << "Client: recv() error " << WSAGetLastError() << "." << endl;
			break;
		}
		else if (!strncmp(recvbuf, "SHUTDOWN", 8))
		{
			cout << "Client: recv() finished. " << endl;
			break;
		}
		else
		{
			//cout << "Client: recv() is OK." << endl;
			
			cout << "Server: " << recvbuf << endl;
			
			//cout << "Client: Bytes received: " << bytesRecv << "." << endl;
		}
	}
}


void Client::RunThread(int choice) //Definicja metody RunThread
{
	if (choice == 1)
	{
		thread t(&Client::Receiving, this);
		t.detach();
	}
	if (choice == 2)
	{
		thread t2(&Client::WriteFile, this);
		t2.detach();
	}
}


void Client::SetCursor(int x, int y)
{
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;

    pos.X=x;
    pos.Y=y;

    SetConsoleCursorPosition(console,pos);
}

void Client::GetCursor(int & x, int & y)
{
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(console,&csbi);

    x=csbi.dwCursorPosition.X;
    y=csbi.dwCursorPosition.Y;
}


void Client::WriteFile()
{
	int bytesRecv = SOCKET_ERROR;
	ofstream myStream;
	string const Path("test.csv");
	myStream.open(Path, ios::app);

	char buffer[10000] = "";
	bytesRecv = recv(MainSocket, buffer, 10000, 0);
	myStream << buffer;
	cout << "We received " << bytesRecv << " bytes." << endl;
}