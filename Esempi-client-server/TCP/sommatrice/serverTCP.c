#include "network.h"

int main(void) {
    int request, response;
    socketif_t socket;
    connection_t connection;
    int somma = 0;
    socket = createTCPServer(35000);
    if (socket < 0){
        printf("[SERVER] Errore di creazione del socket: %i\n", socket);
    }
    else
    {
        
        printf("[SERVER] Sono in attesa di richieste di connessione da qualche client\n");    
        connection = acceptConnection(socket);
        printf("[SERVER] Connessione instaurata\n");
        while (1) {
            printf("[SERVER] In attesa di richieste dal client\n");
            TCPReceive(connection, &request, sizeof(request));
            if (request == 0)
                printf("[SERVER] Calcolo somma per il client\n");
                break;
            printf("[SERVER] Ho ricevuto la seguente richiesta dal client: %d\n", request);
            somma += request;
            printf("[SERVER] Somma parziale: %d\n", somma);
            response = somma;
            
        }
        printf("[SERVER] Invio la risposta al client\n");
        TCPSend(connection, &response, sizeof(response));
        
    }
}

