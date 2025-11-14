#include "network.h"

int main(void) {
    connection_t connection;
    int response;
 	
    printf("[CLIENT] Creo una connessione logica col server\n");
    connection = createTCPConnection("localhost", 35000);
    if (connection < 0) {
        printf("[CLIENT] Errore nella connessione al server: %i\n", connection);
        return -1;
    }

    FILE *fptr1;
    char filename[100];
  
    printf("Nome del file da richiedere:\n");
    scanf("%s", filename);

    printf("[CLIENT] Richiedo il file al server\n");
    TCPSend(connection, filename, sizeof(filename));  

    TCPReceive(connection, &response, sizeof(response));
    if (response == -1) {
        printf("[CLIENT] Errore: il server non ha trovato il file %s\n", filename);
        closeConnection(connection);
        return -1;
    }

    
    printf("[CLIENT] Ricezione del file %s in corso...\n", filename);
    fptr1 = fopen(filename, "w");
    if (fptr1 == NULL) {
        printf("Errore nell'apertura del file %s in scrittura\n", filename);
        closeConnection(connection);
        return -1;
    }

    char buffer[1024];
    int n;
    
    while ((n = TCPReceive(connection, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, n, fptr1);
    }

    printf("[CLIENT] File ricevuto e salvato come %s\n", filename);

    fclose(fptr1);


    closeConnection(connection);
    return 0;
}
