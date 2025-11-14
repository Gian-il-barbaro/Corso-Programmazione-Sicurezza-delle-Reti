#include "network.h"

float calcolaSomma(float val1, float val2)  {
   return (val1 + val2);
}
// Calcola la somma dei numeri primi in un intervallo [min, max]
long calcolaNumeriPrimi(int min, int max){
    int i, j, isPrime;
    long sum = 0;
    for (i = min; i <= max; i++) {
        if (i < 2) continue;
        isPrime = 1;
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) {
            //printf("Trovato numero primo: %d\n", i);
            sum += i;
        }
    }
    return sum;
}

int main(){
    socketif_t sockfd;
    FILE* connfd;
    int res, i;
    long length=0;
    char request[MTU], url[MTU], method[10], c;
    
    sockfd = createTCPServer(8002);
    if (sockfd < 0){
        printf("[SERVER] Errore: %i\n", sockfd);
        return -1;
    }
    
    while(true) {
        connfd = acceptConnectionFD(sockfd);
        
        fgets(request, sizeof(request), connfd);
        strcpy(method,strtok(request, " "));
        strcpy(url,strtok(NULL, " "));
        while(request[0]!='\r') {
            fgets(request, sizeof(request), connfd);
            if(strstr(request, "Content-Length:")!=NULL)  {
                length = atol(request+15);
            }
        }
        
        if(strcmp(method, "POST")==0)  {
            for(i=0; i<length; i++)  {
                c = fgetc(connfd);
            }
        }
        
        if (strstr(url, "calcola-numeri-primi") != NULL) {
            printf("Chiamata a funzione calcola_numeri_primi\n");
            
            char *function, *min_str, *max_str;
            int min, max;
            long somma;
   
            // skeleton: decodifica (de-serializzazione) dei parametri
            function = strtok(url, "?&");
            min_str = strtok(NULL, "?&");
            max_str = strtok(NULL, "?&");
            strtok(min_str,"=");
            min = atoi(strtok(NULL,"="));
            strtok(max_str,"=");
            max = atoi(strtok(NULL,"="));
            
            // chiamata alla business logic
            somma = calcolaNumeriPrimi(min, max);
            
            // skeleton: codifica (serializzazione) del risultato
            fprintf(connfd,"HTTP/1.1 200 OK\r\n\r\n{\r\n    \"somma\":%ld\r\n}\r\n", somma);
        }
        else if(strstr(url, "calcola-somma") != NULL){
            printf("Chiamata a funzione sommatrice\n");
            
            char *function, *op1, *op2;
            float somma, val1, val2;
   
            // skeleton: decodifica (de-serializzazione) dei parametri
            function = strtok(url, "?&");
            op1 = strtok(NULL, "?&");
            op2 = strtok(NULL, "?&");
            strtok(op1,"=");
            val1 = atof(strtok(NULL,"="));
            strtok(op2,"=");
            val2 = atof(strtok(NULL,"="));
            
            // chiamata alla business logic
            somma = calcolaSomma(val1, val2);
            
            // skeleton: codifica (serializzazione) del risultato
            fprintf(connfd,"HTTP/1.1 200 OK\r\n\r\n{\r\n    \"somma\":%f\r\n}\r\n", somma);
        }
        else {
            fprintf(connfd, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"errore\": \"Funzione non riconosciuta!\"}\r\n");
        }
        fclose(connfd);
                
        printf("\n\n[SERVER] sessione HTTP completata\n\n");
    }
    
    closeConnection(sockfd);
    return 0;
}



