// MinimalTCPServer.cpp : Defines the entry point for the console application.
//

// primerServer.cpp: определяет точку входа для консольного приложения.
//
//#include<windows.h>
#include "stdafx.h"
#include <iostream>
#include <winsock2.h> // Включаем использование сокетов
#pragma comment(lib, "WSock32.lib") // Используем эту библиотеку

using namespace std;

void main()
{
	WSADATA wsa_data; // Информация о сокетах
	SOCKET s; // Серверный сокет

	s = NULL;

	// 1. Инициализируем сокеты (требуем версию 1.1 как минимум)
	if (WSAStartup(0x101, &wsa_data) != 0) return;

	// 2. Открываем серверный сокет
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) return;

	// 3. Привязываем сокет к адресу
	sockaddr_in addr; // Для хранения адреса
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(9090); // Наш порт, который будем открывать для коннектов
	if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)) != 0) return;

	// 4. Устанавливаем сокет в режим прослушивания
	if (listen(s, 1) == SOCKET_ERROR) return;

	// 5. Бесконечный цикл получения соединений
	while (true)
	{
		// Принимаем соединения от клиентов
		int from_len;
		SOCKET client_socket; // Сокет клиента
		sockaddr_in client_addr; // Адрес клиента

		from_len = sizeof(client_addr);
		client_socket = accept(s, (LPSOCKADDR)&client_addr, &from_len);

		// Проверяем ошибочность соединения
		if (client_socket == INVALID_SOCKET)
			continue; // Если что опять переходим в режим ожидания соединения

		cout << "Client connected [" << inet_ntoa(client_addr.sin_addr) << "]..." << endl;

		// Обмениваемся данными с клиентом.
		// Тут должны быть вызовы функций send и recv. Функция recv, если что,
		// ждет пока клиент что-нибудь пошлет, send отсылает данные и сразу
		// возвращает управление программе

		// Отсылаем "приветствие" подключившемуся клиенту
		char *greeting = "Hello!!!";
		send(client_socket, greeting, (int)strlen(greeting), 0);

		// Получаем ответ клиента
		char buf[1024];
		int len = recv(client_socket, buf, 1024, 0);
		if (len != SOCKET_ERROR)
		{
			buf[len] = 0; // Не забываем ставить терминирующий ноль (если мы строку принимаем)
			cout << buf << endl;
		}

		// Закрываем сокет
		closesocket(client_socket);

		// Переходим к следующему клиенту
		cout << "...disconnected" << endl;
	}

	// 6. Закрываем серверный сокет
	closesocket(s);

	// 7. Завершаем работу с сокетами
	WSACleanup();

}

