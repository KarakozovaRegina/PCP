#include <iostream>
#include "Winsock2.h" // заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
#pragma warning( disable : 4996)//for inet_ntoa
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
    SOCKET serverSocket; // клиентский сокет
    SOCKADDR_IN clnt;//структура сокета клиента

    int i;
    //............................................................
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        //............................................................
        if ((serverSocket = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());
        //............................................................
  
        SOCKADDR_IN serv; // параметры сокета sS
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
        while (true) {
            int i = 0;

           while (true) 
           {

               Sleep(3000);
                if ((libuf = recvfrom(serverSocket, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc)) == SOCKET_ERROR)
                    throw  SetErrorMsgText("recv:", WSAGetLastError());

      

                cout << i <<  "	Сообщение: " << ibuf << endl;
                i++;
             

                if ((lobuf = sendto(serverSocket, ibuf, strlen(ibuf) + 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR) // отправить сообщение
                    throw  SetErrorMsgText("send:", WSAGetLastError());
            
            }
            //............................................................
            if (closesocket(serverSocket) == SOCKET_ERROR)
                throw SetErrorMsgText("closesocket:", WSAGetLastError());
            //.............................................................
            if (WSACleanup() == SOCKET_ERROR)
                throw SetErrorMsgText("Cleanup:", WSAGetLastError());
        }
      
    }
    catch (string errorMsgText)
    {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }
    //................................................................
    return 0;
}

