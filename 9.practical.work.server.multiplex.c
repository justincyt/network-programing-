#include<stdio.h>
#include<arpa/inet.h>// inet_addr
#include<sys/socket.h>// socket
#include<netdb.h>
#include<string.h>//strlen
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>
#include<sys/select.h>
#define PORT 8784;
#define MAX 1000
#define MAX_CLIENT 100 


int chat_message(int server_family) 
{ 
    char buff[MAX]; //buffer
    char msg[MAX] = ""; //store received messages
    char delimiter[2] = "\n"; 
    int  n=0,check =0; 

    while(true) 
    { 
        bzero(buff, MAX); //using the memset fucntion  
        
        //receive message from client
        printf("From Client: ");
        while(strcmp(&buff[n], delimiter) != 0)//Compared between two string and return its value
        {
            recv(server_family, &buff[n], sizeof(buff[n]), 0); 
            printf("%c", buff[n]);

            strcat(msg, &buff[n]);//connect two string together 
        }
        
        bzero(buff, MAX); 

        //send messages to client
        printf("[Server]: ");
        fgets(buff, sizeof(buff), stdin);
        send(server_family, &buff[n], sizeof(buff), 0);

        //server exit program 
        if(strncmp("exit", buff, 4) == 0) //Compared between two string and return its value 
        { 
            check=1;

            printf("Server Disconnecting...\n");
            shutdown(server_family, SHUT_RDWR);

            break; 
        }
    }
    return check;
    
}


int main(){
    struct sockaddr_in serveraddr,caddr;
    int server_family,bind_create,listen_port;
    int clen,accept_create;
    unsigned short port = 8784;
    int clientfd[100];
    char s[MAX]="";
    memset(clientfd,0,sizeof(clientfd));
    //create socket
    server_family=socket(AF_INET,SOCK_STREAM,0);
    if (server_family<0){
        printf("Could not create socket.....\n");
        exit(0);
    }
    else{
        printf("Socket created\n");
    }
    //reusing the address function
    int reu_address= setsockopt(server_family,SOL_SOCKET,SO_REUSEADDR, 
                    &(int){1}, sizeof(int));
    
    // enable non-blocking socket
    int nonblocking_option = fcntl(server_family, F_GETFL, 0);
    nonblocking_option |=O_NONBLOCK;
    fcntl(server_family, F_SETFL, nonblocking_option);


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

    //create the non blocking 
    while(true){
        fd_set set;  //initialize a set 
        FD_ZERO(&set);  //clear the set 
        FD_SET(server_family, &set); // add listening socket to the set 
        int maxfd = server_family;
        for (int i=0;i<100;i++){
            if(clientfd[i]>0){
                FD_SET(clientfd[i], &set);
            }
            if (clientfd[i]>maxfd){
                maxfd=clientfd[i];
            }
        }
        select(maxfd+1,&set,NULL,NULL,NULL); 


        //accept connection from client 
        clen=sizeof(caddr); 
        accept_create=accept(server_family,(struct sockaddr*)&caddr,&clen);
        if (FD_ISSET(server_family,&set))
        {
            while(accept_create<0){
            
                accept_create=accept(server_family,(struct sockaddr*)&caddr,&clen);
                
                
                if (accept_create>0){
                    // nonblocking for client
                    printf("Connection accepted\n");
                    //enable nonblocking for client 
                    int nonblocking_option = fcntl(accept_create, F_GETFL, 0);
                    nonblocking_option |=O_NONBLOCK;
                    fcntl(accept_create, F_SETFL, nonblocking_option); 
                }
                else{
                    printf("Failed. %d\n",accept_create);
                }
                
            } 


            //add it to the clientfd arrays 
            for (int i=0;i<MAX_CLIENT;i++)
            {
                if(clientfd[i]==0)
                {
                    clientfd[i]=accept_create;
                    break;
                }
            }
        }

        // create the
        for(int i=0;i<MAX_CLIENT;i++)
        {
            if(clientfd[i]>0 && FD_ISSET(clientfd[i], &set))
            {
                if(recv(clientfd[i],s,sizeof(s),0)>0){
                    printf("Client %d says:%s\nserver>\n",clientfd[i],s);
                }
                else{
                    printf("Client %d has disconnected.\n",clientfd[i]);
                    clientfd[i]=0;
                }
            }
        }
            

        //create the accept function
        if(chat_message(accept_create) == 1){
                    
            break;
        }
        else{
            chat_message(accept_create);
        }
    }  
    
    return 0;
}

    
  
 