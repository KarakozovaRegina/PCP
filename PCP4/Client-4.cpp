#include <iostream>
#include "Winsock2.h" // заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
#pragma warning(disable:4996) //для использования inet_addr()

using namespace std;

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
/// 
SOCKET clientSocket; // клиентский сокет

char obuf[50] = "Hello"; //буфер вывода
char ibuf[50];                    //буфер ввода 
int lobuf = 0; //количество отправленных 
int  libuf = 0;                    //количество принятых байт

SOCKADDR_IN all;
///

bool  GetServer(
    char* call, //[in] позывной сервера  
    short            port, //[in] номер порта сервера    
    struct sockaddr* from, //[out] указатель на SOCKADDR_IN
    int* flen  //[out] указатель на размер from 
) {

    //...........................................................
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
        throw SetErrorMsgText("socket:", WSAGetLastError());
    int optval = 1;
    if (setsockopt(clientSocket, SOL_SOCKET, SO_BROADCAST,//
        (char*)&optval, sizeof(int)) == SOCKET_ERROR)
        throw SetErrorMsgText("opt:", WSAGetLastError());

     // параметры сокета sS
    all.sin_family = AF_INET; // используется IP-адресация 
    all.sin_port = htons(2000); // порт 2000
    all.sin_addr.s_addr = INADDR_BROADCAST; // всем

    if ((lobuf = sendto(clientSocket, obuf, sizeof(obuf), NULL,
        (sockaddr*)&all, sizeof(all))) == SOCKET_ERROR)
        throw SetErrorMsgText("sendto:", WSAGetLastError());
    if ((libuf = recvfrom(clientSocket, ibuf, sizeof(ibuf), NULL, from, flen)) == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAETIMEDOUT)
        {
            return false;
        }
        else
        {
            throw  SetErrorMsgText("recv:", WSAGetLastError());
        }
    }

    if (!strcmp(ibuf, obuf))
    {
        cout << endl << "Позывной сервера совпадает.";
        return true;
    }
    else if (strcmp(ibuf, obuf) == 0) {
        cout << "Найден идентичный сервер!" << endl;
        return false;
    }
    else
    {
        cout << endl << "Позывной сервера не совпадает.";
        return false;
    }
/*
    if (strcmp(ibuf, obuf) == 0)
    {
        cout << endl << "Позывной сервера совпадает";
        return true;
    }
    else
    {
        cout << endl << "Позывной сервера не совпадает";
        return false;
    }
 */  
    //........................................................... 

}


int main()
{
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;

    SOCKADDR_IN clnt;
    int lc = sizeof(clnt);

    /*
    SOCKET serverSocket;

    SOCKADDR_IN serv; // параметры сокета сервера
    serv.sin_family = AF_INET; // используется ip-адресация 
    serv.sin_port = htons(2000); // порт 2000
    serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // адрес сервера 
    */

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        //............................................................



        GetServer(ibuf, 2000, (sockaddr*)&clnt,&lc);

        SOCKADDR_IN* addr = (SOCKADDR_IN*)&all;
        cout << endl << "Порт сервера: " << addr->sin_port;
        cout << endl << "IP-адрес сервера: " << inet_ntoa(addr->sin_addr);
    
        //............................................................
        if (closesocket(clientSocket) == SOCKET_ERROR) 
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
}
