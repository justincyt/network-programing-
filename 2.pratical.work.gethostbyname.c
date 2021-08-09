// Import the library to write socket
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen 
#include <errno.h>

#include <netdb.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>  //inet_addr

                
                // -------------------------- FUNCTION ------------------------------

char *addrtype(int addrtype) { 
        switch(addrtype) {     //Option for IPV
                case AF_INET: 
                        return "AF_INET"; //Option for IPv4 
                case AF_INET6: 
                        return "AF_INET6"; //Option for IPv6
        }
        return "Unknown";
}

                // --------------------------- MAIN ---------------------------

int main(int argc, char **argv) {
        struct hostent *hostname; 
        int i = 0;
        char domain[100];
        if (argc < 2) {
                printf("Enter the domain:"); 
                scanf("%s",domain);
                hostname=gethostbyname(domain); 
               
        }
        else
        {
                hostname = gethostbyname(argv[1]);
        }
        
        
        if (!hostname) {
                printf("Lookup Failed: %s\n", hstrerror(h_errno));
                return 0;
        }
        
        i = 0;
        while(hostname ->h_addr_list[i] != NULL) {
                printf("The IP address:");
                printf("%s\n", inet_ntoa((struct in_addr)*((struct in_addr *) hostname->h_addr_list[i])));
                i++;
        }
        return 0;
}
