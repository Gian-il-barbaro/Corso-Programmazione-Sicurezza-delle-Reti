#include "network.h"

int main(void) {
    int request;
    int response;
    socketif_t socket;
    char hostAddress[MAXADDRESSLEN];
    int port;
    int counter=0;
    socket = createUDPInterface(35000);
    
    while(true){
        printf("[SERVER] Sono in attesa di richieste da qualche client\n");
    
        try {
            UDPReceive(socket, &request, sizeof(request), hostAddress, &port);
            counter++;
            printf(counter);
        }
        catch (keyboardInterrupt) {
            printf("\n[SERVER] Ricevitore terminato dall'utente\n");
            closeInterface(socket);
            print("Totale: %d",counter);
            return 0;
        }
    }
}

