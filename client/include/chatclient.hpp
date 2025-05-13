// client/include/chatclient.hpp

#ifndef CHATCLIENT_HPP
#define CHATCLIENT_HPP

/*  класс ChatClient отвечает за идею клиента как токового, 
 *  он регулирует взаимодействие между собой сокета, графического интерфейса и ввод сообщений. 
 *
 *  как он начинает свою работу?
 *      - он создаётся с помощью статического метода например Start, который возвращает указатель на обект ChatClient
 *      - с параметрами ip и порта.
 *      - Метод старт создаёт сокет, создаёт окно и возвращает new ChatClient(сокет, окно) 
 *
 *      - после метода Start вызывается метод Run в котором мониторятся действия:
 *              - пользователь нажал кнопку
 *              - пришло сообщение от сервера
 *              - клиент написал сообщение надо отправить
 */

#include "sockets.hpp"
#include "messageprocessor.hpp"
#include "guiinterface.hpp"

class ChatClient
{
    NetworkManager *network;
    GUIInterface *gui;
    MessageProcessor *proccessor;

    ChatClient(NetworkManager *net, GUIInterface *ui, MessageProcessor *proc = 0);
public:
    ~ChatClient();

    // Здесь создаются сокет и окно
    static ChatClient *Start(const char *ip, const char *str_port);

    // здесь основной цикл
    void Run();

private:
    ChatClient(const ChatClient&);
    ChatClient& operator=(const ChatClient&);
};

#endif // CHATCLIENT_HPP
