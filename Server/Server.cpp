#include "Server.h"


Server::Server()
{
	MainSocket = SOCKET_ERROR;
	AcceptSocket = SOCKET_ERROR;
}


Server::~Server() { }


void Server::Initiation() //Definicja metody Initiation
{
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData); // Wyb�r wersji 2.2 Winsocka
}


void Server::CreateSocket() //Definicja metody CreateSocket
{
	MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Domena, typ gniazda, protok�
}


void Server::GetAdress() //Definicja metody GetAdress
{
	service.sin_family = AF_INET; // Rodzina adres�w
	service.sin_addr.s_addr = inet_addr("192.168.0.102"); // Adres IP serwera
	service.sin_port = htons(55555); // Numer Portu
}


void Server::WaitingForConnection() //Definicja metody WaitingForConnection
{
	if (::bind(MainSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) // Przypisanie gniazda portowi
	{
		SetConsoleTextAttribute(hOut, FOREGROUND_RED);
		cout << "Server: bind() failed: " << WSAGetLastError() << endl;
		closesocket(MainSocket);
	}

	if (listen(MainSocket, 10) == SOCKET_ERROR) // Nas�uchiwanie na po��czenie
	{
		cout << "Server: listen(): Error listening on socket " << WSAGetLastError() << endl;
	}
	cout << "Server: Waiting for a client to connect..." << endl;
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
	cout << "***Hint: Server is ready...run your client program...***" << endl;

	while (true)
	{
		AcceptSocket = SOCKET_ERROR;
		while (AcceptSocket == SOCKET_ERROR)
		{
			AcceptSocket = accept(MainSocket, 0, 0); // Akceptacja po��czenia
		}
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
		cout << "Server: Client Connected!" << endl;
		MainSocket = AcceptSocket;
		break;
	}
}


string Server::CurrentDateTime()  
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%F %T", timeinfo);
	std::string str(buffer);

	return str;
}


void Server::Sending() //Definicja metody Sending
{
	// Wysy�anie danych do klienta
	int bytesSent;
	int bytesSentTotal = 0;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	int wordCount = 1;
	int wordCountSum = 0;
	int charCount = 0;
	int charCountSum = 0;
	clock_t cto;                         //poczatek zegara
	cto = clock();
	while (true)
	{

		char sendbuf[200] = "";
		cin.getline(sendbuf, 200);

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



		int x, y;
		GetCursor(x, y);
		y--;
		x = 0;
		SetCursor(x, y);
		if (strncmp(sendbuf, "INFO", 4)) {
			bytesSent = send(MainSocket, sendbuf, strlen(sendbuf), 0);
			bytesSentTotal += bytesSent;
		}

		if (bytesSent == SOCKET_ERROR)
		{
			SetConsoleTextAttribute(hOut, FOREGROUND_RED);
			cout << "Server: send() error " << WSAGetLastError() << "." << endl;
			break;
		}
		else if (!strncmp(sendbuf, "CLOSE", 7))
		{
			SetConsoleTextAttribute(hOut, FOREGROUND_RED);
			cout << "Server: Connection Closed." << endl;

			Sleep(3000);
			WSACleanup();
		}
		else if (!strncmp(sendbuf, "INFO", 4)) {
			clock_t ct;                   //koniec zegara
			ct = clock();
			double time = (double)(ct - cto) / CLOCKS_PER_SEC;
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << "Wyslano slow lacznie: " << wordCountSum - 1 << " Wyslano znakow lacznie " << charCountSum << endl;
			cout << "Rozmowa trwa: " << time << "sec" << endl;
			cout << "Waga wyslanych danych: " << bytesSentTotal << "byte" << endl;
		}
		else if (bytesSent != 0)
		{
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_RED); //Zolty

			//	cout << "Bytes sent: " << bytesSent << " Wyslano slow: "<< wordCount << " Wyslano znakow" << charCount << endl;
			wordCount = 1;
			//	charCountSum = 0;


			string currentTime = CurrentDateTime();

			char tab2[80]; //Aktualny czas
			strncpy(tab2, currentTime.c_str(), sizeof(tab2));
			tab2[sizeof(tab2) - 1] = 0;

			char userName[20]; //Nazwa u�ytkownika
			strncpy(userName, " | Server: ", sizeof(userName));
			userName[sizeof(userName) - 1] = 0;


			cout << tab2 << userName << sendbuf << endl; //Wy�wietlanie wiadomo�ci na ekranie

		    //Zapis do pliku
			ofstream myfile;
			myfile.open("history.txt", std::ios_base::app);
			if (myfile.is_open())
			{
				myfile << tab2 << userName << sendbuf << endl;
				myfile.close();
			}
			
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);//Jasny do pisania

		}
	}
}




void Server::Receiving() //Definicja metody Receiving
{
	// Odbieranie danych z klienta
	int bytesRecv = SOCKET_ERROR;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	while (true)
	{
		char recvbuf[200] = "";
		bytesRecv = recv(MainSocket, recvbuf, 200, 0);

		if (bytesRecv == SOCKET_ERROR)
		{
			SetConsoleTextAttribute(hOut, FOREGROUND_RED);
			//cout << "Server: recv() error " << WSAGetLastError() << "." << endl;
			cout << "Server: lost connection with client!" << endl;
			WSACleanup();
			break;
		}
		else if (!strncmp(recvbuf, "SHUTDOWN", 8))
		{
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
			cout << "Server: recv() finished. " << endl;
			break;
		}
		else
		{

			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
			cout << "Server: recv() is OK." << endl;

			SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY); //CiemnoSzary
			string currentTime = CurrentDateTime();

			char tab2[80]; //Aktualny czas
			strncpy(tab2, currentTime.c_str(), sizeof(tab2));
			tab2[sizeof(tab2) - 1] = 0;

			char userName[20]; //Nazwa u�ytkownika
			strncpy(userName, " | Client: ", sizeof(userName));
			userName[sizeof(userName) - 1] = 0;


			cout << tab2;//Wy�wietlanie daty
			cout<<userName << recvbuf << endl; //Wy�wietlanie wiadomo�ci na ekranie

			//Zapis do pliku
			ofstream myfile;
			myfile.open("history.txt", std::ios_base::app);
			if (myfile.is_open())
			{
				myfile << tab2 << userName << recvbuf << endl;
				myfile.close();
			}
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
			cout << "Server: Bytes received: " << bytesRecv << "." << endl;
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);//Jasny do pisania
		}
	}
}


void Server::LoadChatHistory()
{
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);//Seledyn

	ifstream myReadFile;
	myReadFile.open("history.txt");


	string output;
	//std::vector<char> output;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			cout << "=========Historia konwersacji=========" << endl;

			while (getline(myReadFile, output))
			{
				std::cout << output << std::endl;
			}
			cout << "=========Rozpoczeto nowa konwersacje=========" << endl;
			cout << endl;

		}
	}
	myReadFile.close();
}

void Server::RunThread(int choice) //Definicja metody RunThread
{
	if (choice == 1)
	{
		thread t(&Server::Sending, this);
		LoadChatHistory();
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
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;

	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(console, pos);
}

void Server::GetCursor(int & x, int & y)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(console, &csbi);

	x = csbi.dwCursorPosition.X;
	y = csbi.dwCursorPosition.Y;
}


void Server::ReadFile()
{
	ifstream myStream;
	string const Path("test.csv");
	myStream.open(Path, ios::ate);

	char* buffer;
	int length = myStream.tellg(); // Sprawdzenie pozycji, kt�ra odpowiada d�ugo�ci pliku
	myStream.seekg(0, ios::beg); // Przej�cie na poczatek pliku
	if (myStream) // Sprawdzamy czy plik zosta� otwarty.
	{
		buffer = new char[length];
		myStream >> buffer; // Odczytanie danych z pliku do bufora.

		if (SendAll(MainSocket, buffer, &length) == -1)
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

		SetConsoleTextAttribute(hOut, FOREGROUND_RED);
		cout << "ERROR: Couldn't open the file." << endl;
	}
}


int Server::SendAll(int MainSocket, char* buffor, int* length)
{
	int total = 0; // Tyle bajt�w ju� wys�ali�my
	int bytesleft = *length; // Tyle jeszcze zosta�o do wys�ania
	int n;

	while (total < *length) {
		n = send(MainSocket, buffor + total, bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
	}
	*length = total; // Zwraca ilo�� faktycznie wys�anych bajt�w

	return n == -1 ? -1 : 0; // Zwraca -1 w przypadku b��du, 0, gdy si� powiedzie
}