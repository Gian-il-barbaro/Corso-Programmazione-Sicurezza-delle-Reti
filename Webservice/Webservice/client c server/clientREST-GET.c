#include "network.h"
#include <sys/time.h>

float calcolaSomma(float val1, float val2)  {
    char request[MTU], response[MTU];
    
    // stub: codifica (serializzazione) dei parametri e salvo in request
    sprintf(request, "http://localhost:8000/calcola-somma?param1=%f&param2=%f", val1, val2);
    
    // chiamata del webservice
    int res = doGET(request, response, MTU);
    if (res < 0){
        printf("Errore: %i\n", res);
        return -1;
    }
    
    printf("Risposta dal server:\n%s\n", response);
    
    // stub: de-codifica (de-serializzazione) del risultato da stringa a float
    return atof(strstr(response,":")+1);
}
long calcolaNumeriPrimi(int min, int max){
    char request[MTU], response[MTU];
    struct timeval start, end;

    gettimeofday(&start, NULL);  //  inizio timer

    sprintf(request, "http://localhost:8000/calcola-numeri-primi?min=%d&max=%d", min, max);
    
    int res = doGET(request, response, MTU);

    gettimeofday(&end, NULL);  // fine timer

    if (res < 0){
        printf("Errore: %i\n", res);
        return -1;
    }

    long elapsed_ms = (end.tv_sec - start.tv_sec) * 1000L + (end.tv_usec - start.tv_usec) / 1000L;
    printf("Tempo impiegato: %ld ms\n", elapsed_ms);

    printf("Risposta dal server:\n%s\n", response);
    return atoll(strstr(response, ":") + 1);
}

int main(int argc, char **argv){
    
    if(argc < 4)    {
        printf("USAGE: %s tipofunzione op1 op2 \n funzioni riconosciute: calcola-numeri-primi, calcola-somma\n", argv[0]);
        return -1;
    }
    if (strcmp(argv[1], "calcola-numeri-primi") == 0) {
        printf("Risultato: %ld\n", calcolaNumeriPrimi(atoi(argv[2]), atoi(argv[3])));
    } 
    else if (strcmp(argv[1], "calcola-somma") == 0) {
        printf("Risultato: %f\n", calcolaSomma(atof(argv[2]), atof(argv[3])));
    }
    else {
        printf("Funzione non riconosciuta!\n funzioni riconosciute: calcola-numeri-primi, calcola-somma\n");
    }
    
    return 0;
}


