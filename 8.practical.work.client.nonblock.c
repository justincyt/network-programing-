#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen 
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  //inet_addr
#include<stdbool.h>
#include<fcntl.h>
#define PORT 8784;
#define MAX 1000

void chat_message(int sockfd) 
{ 
    char message[MAX]; //message er
    char data[MAX] = ""; //store received messages
    char delimiter[2] = "\n";  
    int n=0;

    while(true)
    {   
        
        bzero(message , sizeof(message ));//using the memset fucntion  

        //sending messages to server 
        printf("[Client]: ");
        fgets(message , sizeof(message ), stdin);// data from the keyboard 
        send(sockfd, &message[n], sizeof(message ), 0);//send function to send message 
        
        //disconnect and exit program 
        if(strncmp("quit", message , 4) == 0) 
        { 
            printf("Client Exit...\n"); 
            break; 
        }
        
        bzero(message , sizeof(message ));//Copy the character from 0 to sizeof message pointed to by the str of message

        
        //receive message from server 
        printf("From Server : ");
        while(strcmp(&message[n], delimiter) != 0)//Compared between two string and return its value
        {
            recv(sockfd, &message[n] , sizeof(message[n]), 0); 
            printf("%c", message[n]);

            strcat(data, &message[n]);//connect two string together 
        }

    }

    shutdown(sockfd, SHUT_RDWR);//shutdown message 
   
            
} 

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
    //connect server 
    int connect_server=connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    if (connect_server<0){
        printf("Error connect\n");
        exit(0);
    }
    else{
        printf("Connected with server\n");
        //reuse address 
        setsockopt(sockfd, SOL_SOCKET , SO_REUSEADDR , &(int){1}, sizeof(int));
        // enable nonblocking 
        fcntl(sockfd,F_GETFL,O_NONBLOCK);
    }

    chat_message(sockfd);//recall function above 

    return 0;
}