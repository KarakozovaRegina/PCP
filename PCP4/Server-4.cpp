#include <iostream>
#include "Winsock2.h" // заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
#pragma warning( disable : 4996)//for inet_ntoa
using namespace std;

//dop
char name[] = "Hello";
char Errorname[] = "Позывной сервера не совпадает";
SOCKET serverSocket;// дескриптор сокета
SOCKADDR_IN clnt;//структура сокета клиента
int lc = sizeof(clnt);



string GetErrorMsgText(int code) // cформировать текст ошибки
{
    string msgText;
    switch (code) // проверка кода возврата 
    {
    case WSAEINTR: msgText = "WSAEINTR"; break;
    case WSAEACCES: msgText = "WSAEACCES"; break;
        //..........коды WSAGetLastError ..........................
    case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE";break;
    default: msgText = "***ERROR***"; break;
    };
    return msgText;
};
string SetErrorMsgText(string msgText, int code)
{
    return msgText + GetErrorMsgText(code);
};

bool GetRequestFromClient(char* name, short port, sockaddr* from, int* flen){
    char ibuf[50]; //буфер ввода 

    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;

    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket:", WSAGetLastError());

    if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
    {
        throw  SetErrorMsgText("error for bind:", WSAGetLastError());
    }

    if ((recvfrom(serverSocket, ibuf, sizeof(ibuf), NULL, from, flen)) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
        {
            return false;
        }
        else
        {
            throw  SetErrorMsgText("recv:", WSAGetLastError());
        }
    }

    if (strcmp(ibuf, name)==0)
    {
        cout << endl << "Позывной сервера совпадает";
        return true;

    }
    else
    {
        cout << endl << "Позывной сервера не совпадает";
        return false;
    }

    

}

bool PutAnswerToClient(char* name, struct sockaddr* to, int* lto) {
    if ((sendto(serverSocket, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
    {
        throw  SetErrorMsgText("send:", WSAGetLastError());
    }

    return true;
}

int main()
{
    setlocale(LC_ALL, "rus");

    try
    {
        WSADATA wsaData;//for library                                                
      
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
       
        //............................................................
        //--------GetRequestFromClient--------
        while (true) {
            if (GetRequestFromClient(name, 2000, (sockaddr*)&clnt, &lc)) {
                PutAnswerToClient(name, (sockaddr*)&clnt, &lc);
            }
            else {
                PutAnswerToClient(Errorname, (sockaddr*)&clnt, &lc);
            }

            SOCKADDR_IN* addr = (SOCKADDR_IN*)&clnt;
            std::cout << std::endl << "Порт клиента: " << addr->sin_port;
            std::cout << std::endl << "IP-адрес клиента: " << inet_ntoa(addr->sin_addr);
       }
        //............................................................
        if (closesocket(serverSocket) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }
    return 0;
}

























/*
string GetErrorMsgText(int code) // cформировать текст ошибки
{
    string msgText;
    switch (code) // проверка кода возврата 
    {
    case WSAEINTR: msgText = "WSAEINTR"; break;
    case WSAEACCES: msgText = "WSAEACCES"; break;
        //..........коды WSAGetLastError ..........................
    case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE";break;
    default: msgText = "***ERROR***"; break;
    };
    return msgText;
};
string SetErrorMsgText(string msgText, int code)
{
    return msgText + GetErrorMsgText(code);
};




SOCKET serverSocket;
SOCKADDR_IN clnt;//структура сокета клиента
int lc = sizeof(clnt);

bool GetRequestFromClient(char* name, short port, sockaddr* from, int* flen)
{
    char nameServer[50];
    memset(from, 0, sizeof(flen)); 

    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket:", WSAGetLastError());

    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
    {
        throw  SetErrorMsgText("Походу такой сервер уже есть:", WSAGetLastError());
    }

    if ((recvfrom(serverSocket, nameServer, sizeof(nameServer), NULL, from, flen)) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
        {
            return false;
        }
        else
        {
            throw  SetErrorMsgText("recv:", WSAGetLastError());
        }
    }

    if (!strcmp(nameServer, name))
    {
        cout << endl << "Позывной сервера совпадает.";
        return true;
    }
    else
    {
        cout << endl << "Позывной сервера не совпадает.";
        return false;
    }

}

int main()
{
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;
   
   

    int i;
    //............................................................
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        //............................................................
      
        //............................................................
  
        /*SOCKADDR_IN serv; // параметры сокета sS
        serv.sin_family = AF_INET; // используется IP-адресация 
        serv.sin_port = htons(2000); // порт 2000 
        serv.sin_addr.s_addr = INADDR_ANY; // адрес сервера
       if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind:", WSAGetLastError());

        SOCKADDR_IN clnt; // параметры сокета клиента
        memset(&clnt, 0, sizeof(clnt)); // обнулить память
        int lc = sizeof(clnt);
        char ibuf[50]; //буфер ввода 
       
        int libuf = 0; // кол-во принятых байт
        int lobuf = 0;
       
        GetRequestFromClient(name, 2000, (sockaddr*)&clnt, &lc);

            if (closesocket(serverSocket) == SOCKET_ERROR)
                throw SetErrorMsgText("closesocket:", WSAGetLastError());
            //.............................................................
            if (WSACleanup() == SOCKET_ERROR)
                throw SetErrorMsgText("Cleanup:", WSAGetLastError());
        
      
    }
    catch (string errorMsgText)
    {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }
    //................................................................
    return 0;
} */

