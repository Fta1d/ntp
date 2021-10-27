#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


void ntpdate(char *arg);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Less arguments than expected\n");
        return 0;
    }

    ntpdate(argv[1]);
    return 0;
}

void ntpdate(char *arg) {
    char *host = "213.231.2.124";
    int port = 123;
    int len = 1024;
    int i;

    unsigned char pack[48] = {010,0,0,0,0,0,0,0,0};
    unsigned long  buf[len];
    struct protoent *proto;
    struct sockaddr_in server_addr;
    struct sockaddr saddr;
    socklen_t saddr_l = sizeof (saddr);
    int s;
    long tmit;

    proto = getprotobyname("udp");
    s = socket(PF_INET, SOCK_DGRAM, proto->p_proto);


    memset( &server_addr, 0, sizeof( server_addr ));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    printf("Sending package\n");
    sendto(s,pack,sizeof(pack),0,(struct sockaddr *)&server_addr,sizeof(server_addr));

    recvfrom(s,buf,48,0,&saddr,&saddr_l);

    tmit = ntohl((time_t)buf[4]);

    tmit -= 2208988800U;

    printf("Time: %s",ctime(&tmit));

    sleep(atoi(arg));

    i=time(0);

    printf("Difference is %ld seconds \n",(i-tmit));
}