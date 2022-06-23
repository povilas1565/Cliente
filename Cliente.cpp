#include "Cliente.h"
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <winsock2.h>
#include <conio.h>
#include <string.h>
#pragma hdrstop
#pragma comment(lib, "Ws2_32.lib")
#pragma argsused



int main(int argc, char* argv[])
{
    char send1[500];
    char send2[500];
    int choice;
    std::cout<<"Vvedite 1 stroky: ";
    gets(send1);
    int length1 = strlen(send1);
    send1[length1]='@';
    send1[length1+1]='\0';

    std::cout<<"Vvedite 2 stroky: ";
    gets(send2);
    int length2 = strlen(send2);
    send2[length2]='@';

    std::cout<<std::endl<<"Menu"<<std::endl
        <<"Soedinenie strok                        [1]"<<std::endl
        <<"Proverka na ravenstvo dlinni            [2]"<<std::endl
        <<"Proverka na ravenstvo                   [3]"<<std::endl
        <<"Vstavka v zadannyu poziciu              [4]"<<std::endl
        <<"Exit                                    [5]"<<std::endl
        <<"Vvod: ";
    int da;
    std::cin>>da;
    if (da==5)
    {
        return 0;
    }
    choice = da+'0';
    send2[length2+1] = da+'0';
    send2[length2+2] = '\0';
    strcat(send1,send2);

    //cout<<endl<<send1<<endl<<endl;

    WSADATA wsaData;   //создание сервера

    /* 1. Параметр MAKEWORD(2, 2) - На современных платформах Win32 используется версия 2.2.
     Для получения значения параметра wVersionRequested можно использовать
     макрос MAKEWORD(2, 2) либо значение 0х0202.
       2. Параметр &wsaData - указатель на структуру LPWSADATA, которая при
     вызове функции WSAStartup заполняется сведениями о версии загружаемой
     библиотеки. */
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
    {
        printf("Error WSAStartup()\n");
        return 0;
    }
    else
        printf("WSAStartup()... OK\n");



    SOCKET m_socket;             //создание сокета

    /* 1. Параметр AF_INET - определяет семейство адресов
  протокола и ограничивает применение второго и третьего параметров.
  Он может принимать значения AF_UNIX, AF_INET, AF_OSI, AF_NS, AF_NETBIOS,
  AF_APPLETALK, AF_ATM и т.д.. Значение AF_INET позволяет взаимодействовать
  через объединенную сеть по IP-адресам.
       2. Параметр  SOCK_STREAM это тип сокета для данного протокола. Для протокола
  TCP/IP он может принимать одно из следующих значений: SOCK_STREAM
  (транспортный протокол TCP, ориентированная на соединение надежная связь),
  SOCK_DGRAM (транспортный протокол UDP, ненадежная дейтаграммная связь),
  SOCK_RAW (простые сокеты).
       3. Параметр IPPROTO_TCP - указывает конкретный протокол. Если этот параметр равен 0,
  это означает, что задействуется протокол по умолчанию для выбранных
  значений семейства адресов и типа. Для протокола TCP задается значение
  IPPROTO_IP, для протокола UDP - IPPROTO_UDP, для простых сокетов - IPPROTO_RAW
  или IPPROTO_ICMP. */

    m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, WSA_FLAG_OVERLAPPED, 0);
    if (m_socket == SOCKET_ERROR)
    {
        printf("Error socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }
    printf("WSASocket()...OK\n");


    char x[16];
    std::cout<<"Vvedite IP servera: ";
    std::cin>>x;

    sockaddr_in clientService;              //Установление соединения
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(2222);
    HOSTENT    *hst;

    if    ( inet_addr (x) !=INADDR_NONE ) clientService.sin_addr.s_addr = inet_addr(x);
        //else if   ( hst=gethostbyname ( x ) ) ( (unsigned long *) &clientService.sin_addr.S_un)[ 0 ]( ( unsigned long ** ) hst->h_addr_list ) [0] [0];
    else if ( inet_addr (x) == INADDR_NONE )
    {
        printf ( " Net takogo adresa   %s \n ", x );
        closesocket (m_socket) ;
        WSACleanup () ;
        getch();
        return  -1;
    }



    /* 1. Параметр m_socket - действительный ТСР-сокет для установления соединения.
       2. Параметр (SOCKADDR*)&clientService - структура адреса сокета (SOCKADDR_IN) для TCP,
       описывающая сервер к которому подключаются.
       3. Параметр sizeof(clientService) - длина переменной. */
    if (WSAConnect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService),NULL,NULL,NULL,NULL) == SOCKET_ERROR)
    {
        printf("Error connect(). Code: %d ", WSAGetLastError());
        /* По завершении работы с библиотекой Winsock необходимо вызвать функцию
           WSACleanup для выгрузки библиотеки и освобождения ресурсов. */
        WSACleanup();
        getch();
        return 0;
    }

    int bytesSent1; //размер 1 ого отправленного сообщения
    int bytesSent2; //размер 2 ого отправленного сообщения

    bytesSent1 = send(m_socket, send1, strlen(send1), 0);

    bytesSent1 = recv(m_socket, (char *) send1, sizeof (send1), 0);

    switch(choice)
    {
        case '1' :
        {
            printf("Rezultat: ");
            for(int i = 0; i < bytesSent1-1; i++)
            {
                if (send1[i]=='@')
                {
                    continue;
                }
                printf("%c", send1[i]);
            }
            getch();
            break;
        }
        case '2' :
        {
            printf("Otvet: ");
            for(int i = 0; i < bytesSent1; i++)
            {
                printf("%c", send1[i]);
            }
            getch();
            break;
        }
        case '3' :
        {
            printf("Otvet: ");
            for(int i = 0; i < bytesSent1; i++)
            {
                printf("%c", send1[i]);
            }
            getch();
            break;
        }
        case '4' :
        {
            char indexChar[5];

            for(int i = 0; i < bytesSent1; i++)
            {
                printf("%c", send1[i]);
            }
            int index = 0;
            std::cin>>index;
            if (index >9 && index <100)
            {
                int i = 1;
                int a[99];
                while(index > 0)
                {
                    a[i] = index % 10;
                    index = index / 10;
                    i--;
                }
                std::cout<<"a[0] = "<<a[0]<<std::endl<<"a[1] = "<<a[1]<<std::endl;
                indexChar[0] = a[0]+'0';
                indexChar[1] = a[1]+'0';
                indexChar[2] = '\0';
                bytesSent1 = send(m_socket, indexChar, 3, 0);
            }else
            {
                indexChar[0] = index+'0';
                indexChar[1] = '\0';
                bytesSent1 = send(m_socket, indexChar, 2, 0);
            }
            std::cout<<"Rezultat: ";
            bytesSent1 = recv(m_socket, (char *) send1, sizeof(send1), 0);
            for(int i = 0; i < bytesSent1; i++)
            {
                if (send1[i]=='@')
                {
                    break;
                }
                printf("%c", send1[i]);
            }
            getch();
        }
    }
}
