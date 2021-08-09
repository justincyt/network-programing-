#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen 
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  //inet_addr
#include<stdbool.h>
#define PORT 8784;


char *addrtype(int addr_type) { 
    switch(addr_type) {     //Option for IPV
        case AF_INET: 
            return "AF_INET"; //Option for IPv4 
        case AF_INET6: 
            return "AF_INET6"; //Option for IPv6
        }
    return "Unknown";
}

int main(int argc, char **argv){
    //gethostbyname function 
    unsigned port =8784;
    struct hostent *hostname; 
    int i = 0;
    char domain[100];
    if (argc < 2) {
        printf("Enter the domain:"); 
        scanf("%s",domain);
        hostname=gethostbyname(domain); 
    }
    else{
        hostname = gethostbyname(argv[1]);
    }
    
    if (!hostname){
        printf("Lookup Failed: %s\n", hstrerror(h_errno));
        exit(0);
    }
    while(hostname ->h_addr_list[i] != NULL){
        printf("The IP address:");
        printf("%s\n", inet_ntoa((struct in_addr)*((struct in_addr *) hostname->h_addr_list[i])));
        i++;
    }


    // Client for socket
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd<0){
        printf("Erro socket\n");
        exit(0);
    }
    printf("Socket created\n");
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    memcpy((char *) &addr.sin_addr.s_addr, hostname->h_addr_list[0], hostname->h_length);
    addr.sin_port = htons(port);
    int connect_server=connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    if (connect_server<0){
        printf("Error connect\n");
        exit(0);
    }
    printf("Connected with server\n");

    //create the data trasfer for Server 
    char message[2048];
    // recv from sever
    while(true){
        int server_recv=recv(sockfd, message, sizeof(message), 0);
        if (server_recv==0){
            printf("Error\n");
            exit(0);
        }
        else{
            //recv message from server
            printf("From server: %s\n", message);
            memset(message, 0, sizeof(message));

            //send messagefor server when recv message from client
            printf("[Client]: \n");
            scanf("%s",message);
            send(connect_server,message,strlen(message)+1,0);
        }
    }
    return 0;
    
}

