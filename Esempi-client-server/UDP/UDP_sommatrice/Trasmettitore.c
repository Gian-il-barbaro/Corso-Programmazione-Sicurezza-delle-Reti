#include "network.h"

int main(void) {
    int request;
    int response;
    socketif_t socket;
    char hostAddress[MAXADDRESSLEN];
    int port;
    int counter = 0;
    socket = createUDPInterface(20000);
    printf("Benvenuto nel calcolatore UDP!\n");
    printf("Per terminare il programma, inserire 0.\n");
    while(true) {
        UDPSend(socket, &request, sizeof(request), "192.168.200.1", 35000);
        if (counter == 1000) {
            printf("Chiusura del programma...\n");
            break;
        }
    }
    closeSocketInterface(socket);
    return 0;
}

