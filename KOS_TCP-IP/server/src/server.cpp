#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define RECEIVE_BUF_SIZE        1024
#define EXAMPLE_PORT            1106
#define MAX_PENDING_CONNECTIONS 10

#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

static void printErrorMessage(const char *message)
{
    printf(ANSI_COLOR_RED);
    perror(message);
    printf(ANSI_COLOR_RESET);
}

int main(int argc, const char *argv[])
{
    struct sockaddr_in stSockAddr;

    /* Create receiving socket for incomming connection requests. */
    int serverSocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* Allow port reuse, otherwise it will be unavailable for 2xMSL after program finished. */
    int optval_v = 1;
    if (setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &optval_v, sizeof(int)) < 0)
    {
        printErrorMessage("setsockopt(SO_REUSEADDR) failed");
        return EXIT_FAILURE;
    }

    if (-1 == serverSocketFD)
    {
        printErrorMessage("Can not create socket");
        return EXIT_FAILURE;
    }

    /* Create and initialise socket`s address structure. */
    memset(&stSockAddr, 0, sizeof(stSockAddr));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(EXAMPLE_PORT);
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind socket with local address. */
    if (-1 == bind(serverSocketFD, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
        printErrorMessage("Error bind failed");
        close(serverSocketFD);
        return EXIT_FAILURE;
    }

    /* Set socket to listening state. */
    if (-1 == listen(serverSocketFD, MAX_PENDING_CONNECTIONS))
    {
        printErrorMessage("Error listen failed");
        close(serverSocketFD);
        return EXIT_FAILURE;
    }

    {
        fd_set readset;

        FD_ZERO(&readset);
        FD_SET(serverSocketFD, &readset);

        /* Waiting for incoming connection request. */
        if (select(serverSocketFD + 1, &readset, NULL, NULL, NULL) == 1)
        {
            /* Receiving of incoming connection request. */
            int ConnectFD = accept(serverSocketFD, NULL, NULL);

            if (ConnectFD == -1)
            {
                printErrorMessage("Error accept failed");
                close(serverSocketFD);
                return EXIT_FAILURE;
            }
            
            /* Connection is now established, we can communicate. */
            while(1)
            {
                /* Send 2-bytes array as: {"Command code", "Argument value"} */
                char arr[2] = {0x01, 0x46};

                /* Try to send data. */
                if (-1 == send(ConnectFD, arr, sizeof(arr), 0))
                {
                    printErrorMessage("[Server] Send failed\n");
                    close(ConnectFD);
                    close(serverSocketFD);
                    return EXIT_FAILURE;
                }
                else
                {
                    /* Print sent data in terminal. */
                    fprintf(stderr, "[Server] I sent: %d %d\n", int(arr[0]), int(arr[1]));
                }

                /* Sleep for 2 seconds and repeat. */
                sleep(5);
            }

            /* Close incomming connection. */
            if (-1 == shutdown(ConnectFD, SHUT_RDWR))
            {
                printErrorMessage("Can not shutdown socket");
                close(ConnectFD);
                close(serverSocketFD);
                return EXIT_FAILURE;
            }

            /* Close accepted connection handler. */
            close(ConnectFD);
        }
        else
        {
            /* In case of error close receive socket. */
            close(serverSocketFD);
            return EXIT_FAILURE;
        }
    }

    /* Stop processing of incoming connection requests. */
    shutdown(serverSocketFD, SHUT_RDWR);
    close(serverSocketFD);

    return EXIT_SUCCESS;
}
