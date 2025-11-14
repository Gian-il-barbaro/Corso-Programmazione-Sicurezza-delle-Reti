#include "network.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Prototipo della funzione
const char* getContentType(const char* filename);

int main() {
    char *HTMLResponse = "HTTP/1.1 200 OK\r\n\r\n"
                         "<html><head><title>An Example Page</title></head>"
                         "<body>Hello World, this is a very simple HTML document.</body></html>\r\n";

    socketif_t sockfd;
    FILE *connfd;
    char request[MTU], method[10], tmp[MTU];
    char *response_body = NULL;
    long length = 0;

    sockfd = createTCPServer(8000);
    if (sockfd < 0) {
        printf("[SERVER] Errore: %i\n", sockfd);
        return -1;
    }

    while (true) {
        connfd = acceptConnectionFD(sockfd);

        if (!fgets(request, sizeof(request), connfd)) {
            fclose(connfd);
            continue;
        }

        printf("[SERVER] %s", request);
        strcpy(tmp, request);
        strcpy(method, strtok(request, " "));

        // Leggi intestazioni
        length = 0;
        while (fgets(request, sizeof(request), connfd) && request[0] != '\r') {
            printf("[SERVER] %s", request);
            if (strstr(request, "Content-Length:") != NULL) {
                length = atol(request + 15);
            }
        }

        // === GESTIONE POST (UPLOAD) ===
        if (strcmp(method, "POST") == 0) {
            char file_name[MTU] = "upload";
            char *body = malloc(length);
            if (!body) {
                fputs("HTTP/1.1 500 Internal Server Error\r\n\r\n", connfd);
                fclose(connfd);
                continue;
            }

            fread(body, 1, length, connfd);

            // Trova nome file
            char *name_start = strstr(body, "filename=\"");
            if (name_start) {
                name_start += 10;
                char *name_end = strchr(name_start, '"');
                if (name_end && (name_end - name_start < MTU)) {
                    strncpy(file_name, name_start, name_end - name_start);
                    file_name[name_end - name_start] = '\0';
                }
            }

            // Pulisci path
            for (int i = 0; file_name[i]; i++) {
                if (file_name[i] == '/' || file_name[i] == '\\')
                    file_name[i] = '_';
            }

            // Trova inizio dati
            char *data_start = strstr(body, "\r\n\r\n");
            if (!data_start) {
                free(body);
                fputs("HTTP/1.1 400 Bad Request\r\n\r\n", connfd);
                fclose(connfd);
                continue;
            }
            data_start += 4;

            // Trova fine dati
            char *data_end = body + length;
            for (char *p = data_start; p < body + length - 3; p++) {
                if (p[0] == '\r' && p[1] == '\n' && p[2] == '-' && p[3] == '-') {
                    data_end = p;
                    break;
                }
            }

            size_t data_len = data_end - data_start;

            FILE *f = fopen(file_name, "wb");
            if (f) {
                fwrite(data_start, 1, data_len, f);
                fclose(f);
                printf("[SERVER] File %s salvato (%zu bytes)\n", file_name, data_len);
            } else {
                printf("[SERVER] Errore nel salvataggio file\n");
            }

            free(body);

            fputs("HTTP/1.1 200 OK\r\n\r\n"
                  "<html><body>Upload completato!</body></html>", connfd);
        }

        // === GESTIONE GET ===
        else if (strcmp(method, "GET") == 0) {
            char *myfile_start = strstr(tmp, "myfile=");
            char *myfile = NULL;

            if (myfile_start != NULL) {
                myfile_start += strlen("myfile=");
                char *myfile_end = strchr(myfile_start, ' ');
                size_t len = myfile_end ? (size_t)(myfile_end - myfile_start) : strlen(myfile_start);

                myfile = malloc(len + 1);
                strncpy(myfile, myfile_start, len);
                myfile[len] = '\0';
            }

            if (!myfile) {
                fputs(HTMLResponse, connfd);
            } else {
                FILE *fptr1 = fopen(myfile, "rb");
                if (!fptr1) {
                    printf("[SERVER] Errore: file %s non trovato\n", myfile);
                    fputs("HTTP/1.1 404 Not Found\r\n\r\n"
                          "<html><head><title>404 Not Found</title></head>"
                          "<body>404 Page Not Found.</body></html>", connfd);
                } else {
                    fseek(fptr1, 0, SEEK_END);
                    length = ftell(fptr1);
                    fseek(fptr1, 0, SEEK_SET);

                    response_body = malloc(length);
                    if (!response_body) {
                        fclose(fptr1);
                        fputs("HTTP/1.1 500 Internal Server Error\r\n\r\n", connfd);
                    } else {
                        fread(response_body, 1, length, fptr1);
                        fclose(fptr1);

                        char header[1024];
                        snprintf(header, sizeof(header),
                                 "HTTP/1.1 200 OK\r\n"
                                 "Content-Type: %s\r\n"
                                 "Content-Length: %ld\r\n"
                                 "Content-Disposition: inline; filename=\"%s\"\r\n"
                                 "\r\n", getContentType(myfile), length, myfile);

                        fputs(header, connfd);
                        fwrite(response_body, 1, length, connfd);
                        free(response_body);
                    }
                }
                free(myfile);
            }
        }

        // === METODO NON SUPPORTATO ===
        else {
            fputs("HTTP/1.1 405 Method Not Allowed\r\n\r\n", connfd);
        }

        fclose(connfd);
        printf("\n[SERVER] Sessione completata\n\n");
    }

    closeConnection(sockfd);
    return 0;
}

const char* getContentType(const char* filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return "application/octet-stream";
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
    if (strcmp(ext, ".pdf") == 0) return "application/pdf";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".txt") == 0) return "text/plain";
    return "application/octet-stream";
}
