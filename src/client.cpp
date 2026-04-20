#include <iostream>
#include <cstring>
#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

void receiveMessage(int socket_way)
{
    char buffer[1024];
    while (true)
    {
        int buffer_status = recv(socket_way, buffer, 1024, 0);
        if (buffer_status <= 0)
        {
            cout << "Disconnected..." << "\n";
            return;
        }
        cout << "\n" << string(buffer, buffer_status) << "\nMessage: ";
        memset(buffer, 0, 1024);
    }
}

void sendMessage(int socket_way)
{
    string message;
    cout << "Your Name: ";
    getline(cin, message);
    send(socket_way, message.c_str(), message.length(), 0);
    while (true)
    {
        cout << "Message: ";
        getline(cin, message);
        send(socket_way, message.c_str(), message.length(), 0);
    }
}

int main()
{
    int my_way = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        cout << "Invalid server address...\n";
        return -1;
    }
    int connection_status = connect(my_way, (struct sockaddr *)&address, sizeof(address));
    if (connection_status == 0)
    {
        cout << "Connected....\n";
    }
    else
    {
        cout << "Failed to connect...\n";
        return -1;
    }

    thread sender(sendMessage, my_way);
    thread receiver(receiveMessage, my_way);

    sender.join();
    receiver.join();

    close(my_way);
    return 0;
}