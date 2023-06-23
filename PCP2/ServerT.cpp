#include <iostream>
#include "Winsock2.h" // заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
#pragma warning( disable : 4996)//for inet_ntoa
using namespace std;

int recv(
    SOCKET s, // [in] дескриптор сокета (канал для приема)
    const char* buf, // [in] указатель буфер данных
    int lbuf, // [in] количество байт данных в буфере
    int flags // [in] индикатор 
);



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
int recv(
    SOCKET s, // [in] дескриптор сокета (канал для приема)
    const char* buf, // [in] указатель буфер данных
    int lbuf, // [in] количество байт данных в буфере
    int flags // [in] индикатор 
);


int main()
{
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;
    SOCKET sS; // клиентский сокет
    SOCKADDR_IN clnt;

    int i;
    //............................................................
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        //............................................................
        if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());
        //............................................................
        SOCKADDR_IN serv; // параметры сокета sS
        serv.sin_family = AF_INET; // используется IP-адресация 
        serv.sin_port = htons(2000); // порт 2000
        serv.sin_addr.s_addr = INADDR_ANY; // любой собственный IP-адрес

        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind:", WSAGetLastError());

        if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
            throw SetErrorMsgText("listen:", WSAGetLastError());
        cout << "Сервер слушает!" << endl;

        SOCKET cS; // сокет для обмена данными с клиентом
        SOCKADDR_IN clnt; // параметры сокета клиента
        memset(&clnt, 0, sizeof(clnt)); // обнулить память
        int lclnt = sizeof(clnt); // размер SOCKADDR_IN

        int t;
                //............................................................
        while (true) {
            i = 1;
            // Функция accept создает канал на стороне сервера и иоздает сокет для обмена данными по этому каналу
            if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
                throw  SetErrorMsgText("accept:", WSAGetLastError());

            cout << "Адрес клиента: " << inet_ntoa(clnt.sin_addr) << ":" << htons(clnt.sin_port) << "\n\n\n\n\n";

            char ibuf[50],                     //буфер ввода 
                obuf[50] = "sever: принято ";  //буфер вывода
            int  libuf = 0,                    //количество принятых байт
                lobuf = 0;

            while (true) {

                t = clock();

                if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                {
                    cout << "Recv: " << WSAGetLastError() << endl;
                    break;
                }


                _itoa(lobuf, obuf + sizeof("sever: принято ") - 1, 10);

                cout << "Принято сообщение " << i << ": " << ibuf << endl;
                i++;

                if ((lobuf = send(cS, ibuf, strlen(ibuf) + 1, NULL)) == SOCKET_ERROR)
                {
                    cout << "Send: " << WSAGetLastError() << endl;
                    break;
                }



            }
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

