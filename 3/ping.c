#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <signal.h>

void signal_handler(int);
char* address;
int transmitted = 0,received = 0;
double rtt_sum = 0.0,rtt_min = 9999.0, rtt_max = 0.0;
/* チェックサムの計算 */
u_int16_t checksum(unsigned short *buf, int size)
{
    unsigned long sum = 0;
    while (size > 1) {
        sum += *buf;
        buf++;
        size -= 2;
    }
    if (size == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

/* protocolで指定されたプロトコルのraw socketを作成する */
int make_raw_socket(int protocol)
{
    int s = socket(AF_INET, SOCK_RAW, protocol);
    if (s < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return s;
}

/* ICMPヘッダの作成 */
void setup_icmphdr(u_int8_t type, u_int8_t code, u_int16_t id, u_int16_t seq, struct icmphdr *icmphdr)
{
    memset(icmphdr, 0, sizeof(struct icmphdr));
    icmphdr->type = type;
    icmphdr->code = code;
    icmphdr->checksum = 0;
    icmphdr->un.echo.id = id;
    icmphdr->un.echo.sequence = seq;
    icmphdr->checksum = checksum((unsigned short *)icmphdr, sizeof(struct icmphdr));
}

int main(int argc, char **argv)
{
    int n, soc;
    struct timeval start, end;
    char buf[1500];
    struct sockaddr_in addr;
    struct in_addr insaddr;
    struct icmphdr icmphdr;
    struct iphdr *recv_iphdr;
    struct icmphdr *recv_icmphdr;

    address = argv[1];
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    if (argc < 2) {
        printf("Usage : %s IP_ADDRESS\n", argv[0]);
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    soc = make_raw_socket(IPPROTO_ICMP);
    for(int seq=1;;seq++){
        gettimeofday(&start, NULL);
        setup_icmphdr(ICMP_ECHO, 0, 0, seq, &icmphdr);

        /* ICMPパケットの送信 */
        n = sendto(soc, (char *)&icmphdr, sizeof(icmphdr), 0, (struct sockaddr *)&addr, sizeof(addr));
        gettimeofday(&end, NULL);
        if (n < 1) {
            perror("sendto");
            return 1;
        }

        /* ICMPパケットの受信 */
        socklen_t addrlen = sizeof(addr);
        n = recvfrom(soc, buf, sizeof(buf), 0,(struct sockaddr *)&addr,&addrlen);
        if (n < 1) {
            perror("recv");
            return 1;
        }
        transmitted++;
        
        recv_iphdr = (struct iphdr *)buf;
        /* IPヘッダからヘッダ長を求め、icmpヘッダの開始位置を調べる */
        recv_icmphdr = (struct icmphdr *)(buf + (recv_iphdr->ihl << 2));
        insaddr.s_addr = recv_iphdr->saddr;
        /* 送信先と受信したパケットの送信源が一致するかと受信したパケットがICMP ECHO REPLYか確認 */
        if (!strcmp(argv[1], inet_ntoa(insaddr)) && recv_icmphdr->type == ICMP_ECHOREPLY && recv_icmphdr->un.echo.sequence == seq && recv_icmphdr->un.echo.id == 0) {
            received++;
            rtt_sum += (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
            if (rtt_sum < rtt_min) {
                rtt_min = rtt_sum;
            }
            if (rtt_sum > rtt_max) {
                rtt_max = rtt_sum;
            }
            printf("%d byte from %s: icmp_seq=%d time=%.3f ms\n",n, argv[1], seq, (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0);
        }
        sleep(1);
    }
    close(soc);
    return 0;
}

void signal_handler(int signal)
{
    printf("\n---%s ping statistics ---\n %d packets transmitted, %d received, %0.3f%% packet loss\n",address, transmitted, received, (double)(transmitted - received) / transmitted * 100);
    exit(1);
}