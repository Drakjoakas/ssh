#include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <errno.h>
 #include <string.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <sys/wait.h>
 #include <signal.h>
 #define LENGTH 20000

void stringToArray(char *input, char **params) {
    char *p;
    char *command;
    int index = 1;
    command = strtok(input, " ");
    if (command) {
        printf("%s\n",command);
    }

    params[0] = command;
    
    do {
        p = strtok(NULL," ");

        if (p) {
            params[index] = p;
            index++;
        }
    } while(p);

    params[index] = NULL;
}

int main(int argc, char const *argv[])
{
    int numbytes;
    char buf[100];

    struct sockaddr_in cliente;
    struct sockaddr_in servidor;


    int server_fd, cliente_fd;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1)
    {
        perror("Server-setsockopt() error!");
        exit(1);
    }
    else
        printf("Server-setsockopt is OK...\n");

    servidor.sin_family = AF_INET;         
    servidor.sin_port = htons( atoi(argv[1]) );
    servidor.sin_addr.s_addr = INADDR_ANY; 
    memset(&(servidor.sin_zero), '\0', 8);

    int sin_size_servidor = sizeof( servidor );

    if (bind(server_fd, (struct sockaddr *)&servidor, sin_size_servidor) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(server_fd, 1) == -1)
    {
        perror("listen");
        exit(1);
    }

    int sin_size_cliente = sizeof( cliente );

    if ((cliente_fd = accept(server_fd, (struct sockaddr *)&cliente, &sin_size_cliente)) == -1)
    {
        perror("accept");
        exit(1);
    }
    printf("server: conexion cliente desde %s\n", inet_ntoa(cliente.sin_addr));

    char buf_peticion[256];
    char buf_respuesta[LENGTH] = "Respuesta recibida.";
    int n;
    
    while (1)
    {
        //Limpiar buf_peticion
        bzero(buf_peticion, 256);
        char *params[10];


        if (n = recv(cliente_fd, buf_peticion, sizeof(buf_peticion), 0) == -1){
            perror("recv");
            exit(1);    
        }

        printf("~%s~\n",buf_peticion);

        
        if (strcmp(buf_peticion,"exit") == 0 )  break;
        
        
        stringToArray(buf_peticion,params);

        

        pid_t pid = fork();
        if (pid < 0){
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            dup2(cliente_fd,1);
            dup2(cliente_fd,2);
            //close(server_fd);
            execvp(params[0],params);

            perror("execv");
            exit(errno);
        }/* else {
            if(send(cliente_fd, buf_respuesta, strlen(buf_respuesta), 0) < 0)
            {
                printf("ERROR: al enviar la salida del comando al cliente\n");
                exit(1);
            }
        }
        */

        

        

        

    }     



    close(cliente_fd);
    close(server_fd);

    shutdown(server_fd, SHUT_RDWR);

    // Termina con exit(0) que significa terminacion exitosa
    exit(0);

    return 0;
}