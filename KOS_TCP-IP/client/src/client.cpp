#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <platform/platform.h>
#include <kos_net.h>

#include <utility>
#include <vector>

#define RECEIVE_BUF_SIZE 2
#define EXAMPLE_PORT 1106
#define NUM_RETRIES 10

class CommandReceiver
{
protected:
    int clientSocketFD;
    const char* server_address;
public:
    CommandReceiver(const char* sa) : server_address(sa) {}

    int Connect()
    {
        /* Create socket for connection with server. */
        clientSocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (-1 == clientSocketFD)
        {
            perror("cannot create socket");
            return EXIT_FAILURE;
        }

        /* Creating and initialisation of socket`s address structure for connection with server. */
        struct sockaddr_in stSockAddr;
        memset(&stSockAddr, 0, sizeof(stSockAddr));
        stSockAddr.sin_family = AF_INET;
        stSockAddr.sin_port = htons(EXAMPLE_PORT);

        struct addrinfo hints;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        struct addrinfo *resultHints = NULL;
        int res = getaddrinfo(server_address, NULL, &hints, &resultHints);
        if (res != 0)
        {
            perror("cannot get address info");
            close(clientSocketFD);
            return EXIT_FAILURE;
        }

        struct sockaddr_in* in_addr = (struct sockaddr_in *)resultHints->ai_addr;
        memcpy(&stSockAddr.sin_addr.s_addr, &in_addr->sin_addr.s_addr, sizeof(in_addr_t));
        freeaddrinfo(resultHints);

        res = -1;
        for (int i = 0; res == -1 && i < NUM_RETRIES; i++)
        {
            sleep(1); // Wait some time for server be ready.
            res = connect(clientSocketFD, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr));
        }

        if (res == -1)
        {
            perror("connect failed");
            close(clientSocketFD);
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    int getCommand(std::pair<int, int>& p)
    {   
        /* Connection is now established, we can communicate. */
        while(1)
        {
            char buf[RECEIVE_BUF_SIZE];
            /* Receive data from server. */
            if (-1 == recv(clientSocketFD, buf, sizeof(buf), 0))
            {
                perror("[Client] recv failed\n");
                close(clientSocketFD);
                return EXIT_FAILURE;
            }
            else
            {
                /* Parse recieved command and it`s argument. Send to GPIO. */
                p.first = static_cast<int>(buf[0]);
                p.second = static_cast<int>(buf[1]);
                fprintf(stderr, "[Client] Received command: [%d] [%d]\n", p.first, p.second);

                return EXIT_SUCCESS;
            }
        }
    }

    ~CommandReceiver()
    {
        /* Close connection with server. */
        shutdown(clientSocketFD, SHUT_RDWR);
        close(clientSocketFD);
    }

};

class IEngine
{
public:
    virtual void Run(const int& direction, const int& value) = 0;
};


class Console_Engine : public IEngine
{
private:
    int PWM;
    const char name;
public:
    Console_Engine(const char& c) : PWM(0), name(c) {}

    void Run(const int& dir, const int& val) override
    {   
        PWM = val*dir;
        fprintf(stderr, "[Motor] Motor%c goes %d\n", name, PWM);
        PWM = 0;
    }
};

class GPIO_Engine : public IEngine
{
public:
    void Run(const int& dir, const int& val) override {}
};


class Robot
{
private:
    IEngine* engines[2];
    const double wheelRadius;
    CommandReceiver* cmdrcv;
public:    
    Robot(const double& wr, CommandReceiver* cr) : engines({new Console_Engine('A'), new Console_Engine('B')}),
        wheelRadius(wr), cmdrcv(cr) {}

    void Run()
    {   
        while(1)
        {
            std::pair<int, int> command;
            cmdrcv->getCommand(command);
            switch(command.first)
            {
                //rotate left
                case 1:
                    Rotate(-1, command.second);
                    break;

                //move forward
                case 2:
                    Move(1, command.second);
                    break;

                //move backward
                case 3:
                    Move(-1, command.second);
                    break;

                //rotate right
                case 4:
                    Rotate(1, command.second);
                    break;

                default:
                    fprintf(stderr, "[Robot] Unknown command {%d}!\n", command.first);
                    break;
            }
            fprintf(stderr, "\n");
        }
    }

    void Move(const int& direction, const int& distance) const
    {
        engines[0]->Run(direction, distance);
        engines[1]->Run(direction, distance);
    }

    void Rotate(const int& direction, const int& angle) const
    {
        engines[0]->Run(direction, angle);
        engines[1]->Run((direction == -1 ? 1 : -1), angle);
    }
};


int main(int argc, const char *argv[])
{
    #if PLATFORM_OS(KOS)
    #define server_addr "10.0.2.2"
        /* Add network interface. */
        if (!configure_net_iface(DEFAULT_INTERFACE, "10.0.2.10", DEFAULT_MASK, DEFAULT_GATEWAY, DEFAULT_MTU)) {
            perror(DEFAULT_INTERFACE ": network iface configuration failed\n");
            return EXIT_FAILURE;
        }

        if (!list_network_ifaces()) {
            perror("listing of host network interfaces failes\n");
            return EXIT_FAILURE;
        }
    #else
    #define server_addr "localhost"
    #endif

    fprintf(stderr, "\n\n");

    /* Initialise basic instances. */
    CommandReceiver cmdrcv(server_addr);
    if (cmdrcv.Connect() != EXIT_SUCCESS) return EXIT_FAILURE;
    Robot robot(0.0, &cmdrcv);
    
    /* Run an infinite cycle for cmd receiving. */
    robot.Run();

    return EXIT_SUCCESS;
}
