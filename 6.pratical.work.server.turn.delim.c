#include<stdio.h>
#include<arpa/inet.h>// inet_addr
#include<sys/socket.h>// socket
#include<netdb.h>
#include<string.h>//strlen
#include<stdlib.h>
#include<stdbool.h>
# define PORT 8784;
#define MAX 1000 

void chat_message(int server_family) 
{ 
    char buff[MAX]; //buffer
    char msg[MAX] = ""; //store received messages
    char delimiter[2] = "\n"; 
    int n = 0; 

    while(true) 
    { 
        bzero(buff, MAX); 
        
        //receive message
        printf("From Client : ");
        while(strcmp(&buff[n], delimiter) != 0)
        {
            recv(server_family, &buff[n], sizeof(buff[n]), 0); 
            printf("%c", buff[n]);

            strcat(msg, &buff[n]);//connect two string together
        }

        bzero(buff, MAX); 

        //send messages
        printf("[Server] : ");
        fgets(buff, sizeof(buff), stdin);
        send(server_family, &buff[n], sizeof(buff), 0);

        if(strncmp("exit",buff, 4) == 0) 
        { 
            printf("Server Exit...\n"); 
            break; 
        }
    } 
}


int main(){
    struct sockaddr_in serveraddr,caddr;
    int server_family,bind_create,listen_port;
    int clen,accept_create;
    unsigned short port = 8784;
    //create socket
    server_family=socket(AF_INET,SOCK_STREAM,0);
    if (server_family<0){
        printf("Could not create socket.....\n");
        exit(0);
    }
    else{
        printf("Socket created\n");
    }
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=AF_INET;
    serveraddr.sin_port=htons(port);
    inet_pton(AF_INET,"0.0.0.0",&serveraddr.sin_addr); // "0.0.0.0" is the IPv4 address and using inet_pton to convert the IP address from text format to numeric binary 

    //create the bind
    bind_create=bind(server_family,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(bind_create<0){
        printf("Error binding\n");
        exit(0);
    }
    printf("Bind done\n");

    //create the listen 
    listen_port=listen(server_family,5);
    if(listen_port<0){
        printf("Error listening\n");
        exit(0);

    }
    printf("Successfully listened\n");
    
    //create the accept function
    clen=sizeof(caddr);
    accept_create=accept(server_family,(struct sockaddr*)&caddr,&clen);
    if(accept_create<0){
        printf("Error accept\n");
        exit(0);
    }
    printf("Connection accepted\n");

    //data transfer  
    chat_message(accept_create);
    return 0;
}