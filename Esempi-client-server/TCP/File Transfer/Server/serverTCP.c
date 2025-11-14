#include "network.h"

int main(void) {
    socketif_t socket;
    connection_t connection;
    FILE *fptr1;
    char filename[100];
    int response;

    socket = createTCPServer(35000);
    if (socket < 0) {
        printf("[SERVER] Errore di creazione del socket: %i\n", socket);
        return -1;
    }

    printf("[SERVER] In attesa di connessioni...\n");
    connection = acceptConnection(socket);
    printf("[SERVER] Connessione accettata.\n");

    printf("[SERVER] Ricevo richiesta di file...\n");
    TCPReceive(connection, filename, sizeof(filename));

    fptr1 = fopen(filename, "r");
    if (fptr1 == NULL) {
        printf("[SERVER] Impossibile aprire il file %s\n", filename);
        response = -1;
        TCPSend(connection, &response, sizeof(response));
        closeConnection(connection);
        return -1;
    }

    printf("[SERVER] File %s aperto. Invio in corso...\n", filename);
    response = 1;
    TCPSend(connection, &response, sizeof(response));  // conferma apertura file
    
    //creo un buffer per la lettura del file perchè mandare un carattere alla volta è inefficiente
    char buffer[8];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), fptr1)) > 0) {
        TCPSend(connection, buffer, n);
    }
    

    printf("[SERVER] Invio completato.\n");
    fclose(fptr1);
    closeConnection(connection);
    return 0;
}
