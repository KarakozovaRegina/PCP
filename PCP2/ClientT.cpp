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


int main()
{
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;
    SOCKET sS; // клиентский сокет
        //............................................................
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        //............................................................
        if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());
        //............................................................

        SOCKET cC; // серверный сокет
        if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());
        SOCKADDR_IN serv; // параметры сокета сервера
        serv.sin_family = AF_INET; // используется IP-адресация 
        serv.sin_port = htons(2000); // TCP-порт 2000
        serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // адрес сервера
        if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
            throw SetErrorMsgText("connect:", WSAGetLastError());
        //...................передача-клиент...........................
        char ibuf[50], //буфер ввода 
            obuf[50] = "Hello from Client "; //буфер вывода
        int libuf = 0, //количество принятых байт
            lobuf = 0; //количество отправленных байь 

        int count;
        cout << "Введите кол-во сообщений: ";
        cin >>  count;
        
        //............................................................
        for (int i = 0; i < count; i++) {
            if ((lobuf = send(cC, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)
                throw  SetErrorMsgText("send:", WSAGetLastError());

            if ((libuf = recv(cC, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                throw  SetErrorMsgText("recv:", WSAGetLastError());

            cout << " Сообщение от сервера " << ": " << ibuf << (i + 1) << endl;


        }
        if (SOCKET_ERROR == send(cC, "Конец передачи", sizeof("Конец передачи"), NULL))
        {
            cout << "send exit : " << GetLastError() << endl;
        }
        //............................................................
        if (closesocket(sS) == SOCKET_ERROR)
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