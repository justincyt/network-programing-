#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen 
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  //inet_addr
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
        
    if (!hostname) {
        printf("Lookup Failed: %s\n", hstrerror(h_errno));
        exit(0);
        }
    while(hostname ->h_addr_list[i] != NULL) {
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
    int connect_sever=connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    if (connect_sever<0){
        printf("Error connect\n");
        exit(0);
    }
    printf("Connected with server\n");

}