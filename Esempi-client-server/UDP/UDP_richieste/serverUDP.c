#include "network.h"

int main(void) {
    socketif_t socket;
    char response[]="Sono il server: ho ricevuto correttamente il tuo messaggio!\n";
    char request[MTU];
    char hostAddress[MAXADDRESSLEN];
    int port;
    int count = 0;
    socket = createUDPInterface(100);
    
    while(true) {
        if (count >= 5) {
            printf("[SERVER] Ho ricevuto 5 messaggi, chiudo il server\n");
            UDPSend(socket, response, strlen(response), hostAddress, port);
            break;
        }
        
        printf("[SERVER] Sono in attesa di richieste da qualche client\n");
    
        UDPReceive(socket, request, MTU, hostAddress, &port);
        count++;
        printf("[SERVER] Ho ricevuto un messaggio da host/porta %s/%d\n", hostAddress, port);
        printf("[SERVER] Contenuto: %s\n", request); 

        printf("[SERVER] Ho ricevuto %d messaggi\n", count);
           
        UDPSend(socket, response, strlen(response), hostAddress, port);
    }
    
}

