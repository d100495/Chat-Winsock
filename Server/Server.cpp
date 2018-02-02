#include "Server.h"


Server::Server()
{
	MainSocket = SOCKET_ERROR;
	AcceptSocket = SOCKET_ERROR;
}


Server::~Server() { }


void Server::Initiation() //Definicja metody Initiation
{
	wVersionRequested = MAKEWORD(2,2); 
	WSAStartup(wVersionRequested, &wsaData); // Wybór wersji 2.2 Winsocka
}


void Server::CreateSocket() //Definicja metody CreateSocket
{
	MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Domena, typ gniazda, protokó³
}


void Server::GetAdress() //Definicja metody GetAdress
{
	service.sin_family = AF_INET; // Rodzina adresów
	service.sin_addr.s_addr = inet_addr("127.0.0.1"); // Adres IP serwera
	service.sin_port = htons(55555); // Numer Portu
}


void Server::WaitingForConnection() //Definicja metody WaitingForConnection
{
	if (::bind(MainSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) // Przypisanie gniazda portowi
	{
		cout << "Server: bind() failed: " << WSAGetLastError() << endl;
		closesocket(MainSocket);
	}
 
	if (listen(MainSocket, 10) == SOCKET_ERROR) // Nas³uchiwanie na po³¹czenie
	{
		cout << "Server: listen(): Error listening on socket " << WSAGetLastError() << endl;
	}
	cout << "Server: Waiting for a client to connect..." << endl;
	cout << "***Hint: Server is ready...run your client program...***" << endl;

	while (true)
	{
		AcceptSocket = SOCKET_ERROR;
		while (AcceptSocket == SOCKET_ERROR)
		{
			AcceptSocket = accept (MainSocket, 0, 0); // Akceptacja po³¹czenia
		}
	cout << "Server: Client Connected!" << endl;
	MainSocket = AcceptSocket;
	break;
	}
}


void Server::Sending() //Definicja metody Sending
{
	// Wysy³anie danych do klienta
	int bytesSent;
	int bytesSentTotal = 0;
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
			cout << "Server: send() error " << WSAGetLastError() << "." << endl;
			break;
		}
		else if (!strncmp(sendbuf, "CLOSE", 7))
		{
			cout << "Server: Connection Closed." << endl;

			Sleep(3000);
			WSACleanup();
		}
		else if (!strncmp(sendbuf, "INFO", 4)) {
			clock_t ct;                   //koniec zegara
			ct = clock();
			double time = (double)(ct - cto) / CLOCKS_PER_SEC;
			cout << "Wyslano slow lacznie: " << wordCountSum - 1 << " Wyslano znakow lacznie " << charCountSum<< endl;
			cout << "Rozmowa trwa: " << time << "sec" << endl;
			cout << "Waga wyslanych danych: " << bytesSentTotal << "byte" << endl;
		}
		else if (bytesSent != 0)
		{
		//	cout << "Bytes sent: " << bytesSent << " Wyslano slow: "<< wordCount << " Wyslano znakow" << charCount << endl;
			wordCount = 1;
		//	charCountSum = 0;
			cout << "Server: " << sendbuf << endl;
			
		}
	}
}


void Server::Receiving() //Definicja metody Receiving
{
	// Odbieranie danych z klienta
	int bytesRecv = SOCKET_ERROR;
	hOut = GetStdHandle( STD_OUTPUT_HANDLE );

	while(true)
	{
		char recvbuf[200] = "";
		bytesRecv = recv(MainSocket, recvbuf, 200, 0);
 
		if (bytesRecv == SOCKET_ERROR)
		{
			//cout << "Server: recv() error " << WSAGetLastError() << "." << endl;
			cout << "Server: lost connection with client!" << endl;
			WSACleanup();
			break;
		}
		else if (!strncmp(recvbuf, "SHUTDOWN", 8))
		{
			cout << "Server: recv() finished. " << endl;
			break;
		}
		else
		{
			cout << "Server: recv() is OK." << endl;
			
			cout << "Client: " << recvbuf << endl;
			
			cout << "Server: Bytes received: " << bytesRecv << "." << endl;
		}
	}
}


void Server::RunThread(int choice) //Definicja metody RunThread
{
	if (choice == 1)
	{
		thread t(&Server::Sending, this);
		t.detach();
	}
	if (choice == 2)
	{
		thread t2(&Server::ReadFile, this);
		t2.detach();
	}
}


void Server::SetCursor(int x, int y)
{
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;

    pos.X=x;
    pos.Y=y;

    SetConsoleCursorPosition(console,pos);
}

void Server::GetCursor(int & x, int & y)
{
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(console,&csbi);

    x=csbi.dwCursorPosition.X;
    y=csbi.dwCursorPosition.Y;
}


void Server::ReadFile()
{
	ifstream myStream;
	string const Path("test.csv");
	myStream.open(Path, ios::ate);

	char* buffer;
	int length = myStream.tellg(); // Sprawdzenie pozycji, która odpowiada d³ugoœci pliku
	myStream.seekg(0, ios::beg); // Przejœcie na poczatek pliku
	if(myStream) // Sprawdzamy czy plik zosta³ otwarty.
	{
		buffer = new char[length];
		myStream >> buffer; // Odczytanie danych z pliku do bufora.

		if( SendAll( MainSocket, buffer, & length ) == - 1 ) 
		{
			cout << "We only sent " << length << " bytes because of the error!" << endl;
		}
		else
		{
			cout << "We sent " << length << " bytes." << endl;
		}
	}
	else
	{
		cout << "ERROR: Couldn't open the file." << endl;
	}
}


int Server::SendAll(int MainSocket, char* buffor, int* length)
{
    int total = 0; // Tyle bajtów ju¿ wys³aliœmy
    int bytesleft = * length; // Tyle jeszcze zosta³o do wys³ania
    int n;
    
    while( total < * length ) {
        n = send( MainSocket, buffor + total, bytesleft, 0 );
        if( n == - 1 ) { break; }
        total += n;
        bytesleft -= n;
    }
    * length = total; // Zwraca iloœæ faktycznie wys³anych bajtów
    
    return n ==- 1 ?- 1: 0; // Zwraca -1 w przypadku b³êdu, 0, gdy siê powiedzie
}