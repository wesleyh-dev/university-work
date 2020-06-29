#include <stdio.h>           // Standard IO, all C programs need this
#include <stdlib.h>          // Standard Library, all C programs need this

#include <sys/types.h>       // System variable types
#include <sys/socket.h>      // Type definitions related to sockets
#include <netinet/in.h>      // Type definitions related to Internet protocol      

#include <arpa/inet.h>       // Network utility functions
#include <netdb.h>           // Name services functions

#include <string.h>


#define LEN	1000

struct Packet
{
	int done;
	int seqNo;
	char line[1000];
};

struct Ack
{
	int done;
	int seqNo;
};


int main(int argc, char *argv[]) 
{
	int s;
	struct sockaddr_in in_addr;
	struct sockaddr_in sock_addr;
	struct sockaddr_in src_addr;
	int src_addr_len;
	int len;
	struct hostent *host_entry_ptr;
	
	if (argc != 3)
	{
		exit(1);
	}

	s = socket(PF_INET, SOCK_DGRAM, 0);

	in_addr.sin_family = AF_INET;
	in_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	in_addr.sin_port = htons(atoi(argv[2]));

	if (bind(s, (struct sockaddr *)&in_addr, sizeof(in_addr)) == -1)
	{ 
		exit(1);
	}

	FILE* fp = fopen(argv[1], "w");
	
	struct Packet msg;
	struct Ack ack;
	ack.done = 0;	
	ack.seqNo = 0;
	
	host_entry_ptr = gethostbyaddr((char *)&(src_addr.sin_addr.s_addr), sizeof(src_addr.sin_addr.s_addr), AF_INET);
	src_addr_len = sizeof(src_addr);
	
	while (1)
	{	
		recvfrom(s, &msg, sizeof(struct Packet), 0, (struct sockaddr *)&src_addr, &src_addr_len);
		if (msg.seqNo == ack.seqNo)
		{			
			fprintf(fp, "%s\n", msg.line);		
			if (msg.done == 1) 
			{	
				fclose(fp);
				break;
			}
		}
		else
		{
			ack.seqNo--;
		}
		sendto(s, &ack, sizeof(struct Ack), 0, (struct sockaddr *)&src_addr, src_addr_len);
		ack.seqNo++;
	}
	ack.seqNo++;
	ack.done = 1;
	while (1)
	{
		sendto(s, &ack, sizeof(struct Ack), 0, (struct sockaddr *)&src_addr, src_addr_len);
	}

}













