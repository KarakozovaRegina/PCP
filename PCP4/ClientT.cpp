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
    SOCKET clientSocket; // клиентский сокет
        //............................................................
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        //............................................................
        if ((clientSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());
        
        //............................................................
        SOCKET serverSocket;

        SOCKADDR_IN serv; // параметры сокета сервера
        serv.sin_family = AF_INET; // используется ip-адресация 
        serv.sin_port = htons(2000); // порт 2000
        serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // адрес сервера 
        char obuf[50] = "Hello"; //буфер вывода
        char ibuf[50];                    //буфер ввода 
        int lobuf = 0; //количество отправленных 
        int  libuf = 0;                    //количество принятых байт
        //...................передача-клиент...........................
      

        while (true) {
           
            cout << "Введите кол-во сообщений: ";
            int count;
            cin >> count;

            for (int i = 0; i < count; i++) {

                if ((lobuf = sendto(clientSocket, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR) // отправить сообщение
                    throw  SetErrorMsgText("send:", WSAGetLastError());
                if ((libuf = recv(clientSocket, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                    throw  SetErrorMsgText("recv:", WSAGetLastError());

                cout << " Сообщение от сервера " << ": " << ibuf <<" " << (i + 1) << endl;
            }
            //............................................................
            if (closesocket(clientSocket) == SOCKET_ERROR)
                throw SetErrorMsgText("closesocket:", WSAGetLastError());
            //.............................................................
            if (WSACleanup() == SOCKET_ERROR)
                throw SetErrorMsgText("Cleanup:", WSAGetLastError());
        }
      /* .......................................................
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
        }*/
       
    }
    catch (string errorMsgText)
    {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }
    //................................................................
    return 0;
}