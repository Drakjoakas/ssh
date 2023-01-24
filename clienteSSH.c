#include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <errno.h>
 #include <string.h>
 #include <netdb.h>
 #include <sys/types.h>
 #include <netinet/in.h>
 #include <sys/socket.h>
 #define MAXDATASIZE 100
 #define MAXDATASIZE_RESP 40000

int main(int argc, char const *argv[])
{
    
    // Estos 2 son para el comando
    char comando[MAXDATASIZE];
    int len_comando;
    // Estos 2 son para la respuesta
    int numbytes;
    char buf[MAXDATASIZE_RESP];

    int sockfd;  
    struct hostent *he;
    struct sockaddr_in cliente; // información de la dirección de destino 

    if (argc != 3) {
        fprintf(stderr,"usage: client hostname puerto\n");
        exit(1);
    }

    if ((he=gethostbyname(argv[1])) == NULL) {  // obtener información de host servidor 
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    cliente.sin_family = AF_INET;    // Ordenación de bytes de la máquina 
    cliente.sin_port = htons( atoi(argv[2]) );  // short, Ordenación de bytes de la red 
    cliente.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(cliente.sin_zero), '\0',8);  // poner a cero el resto de la estructura 

    if (connect(sockfd, (struct sockaddr *)&cliente, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    do {
        printf("\n $ ");
        fgets(comando,MAXDATASIZE-1,stdin);
        
        printf("%i",strcmp(comando,""));
            
        /* Se deja el ultimo caracter vacio */
        len_comando = strlen(comando) -1;
        comando[len_comando] = '\0';

        if(send(sockfd,comando, len_comando, 0) == -1) {
            perror("send()");
            exit(1);
        }

        if ((numbytes=recv(sockfd, buf, MAXDATASIZE_RESP-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        printf("\n%s\n",buf);

    } while(strcmp(comando,"exit") != 0 );

    close(sockfd);

    return 0;
}