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
	struct sockaddr_in dest_addr;
	int dest_addr_len = sizeof(dest_addr);
	struct hostent *host_entry_ptr;

	if (argc != 5)
	{
		exit(1);
	}

	dest_addr.sin_family = AF_INET;
	
	host_entry_ptr = gethostbyname(argv[2]);

	if (host_entry_ptr == NULL)
	{
		exit(1);
	}

	/* Where am I sending the message to??? */
	memcpy((char *) &(dest_addr.sin_addr.s_addr), host_entry_ptr->h_addr_list[0], host_entry_ptr->h_length);
	dest_addr.sin_port = htons(atoi(argv[3]));

	/* Set up the socket and bind to arbitrary local port */
	s = socket(PF_INET, SOCK_DGRAM, 0);
	memset((char *)&in_addr, 0, sizeof(in_addr));
	in_addr.sin_family = AF_INET;
	in_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	in_addr.sin_port = htons(0);
	if (bind(s, (struct sockaddr *)&in_addr, sizeof(in_addr)) == -1)
	{
		exit(1);
	} 
	

	/* Read from file and send using STOP AND WAIT */
	FILE *fp = fopen(argv[1], "r");
	
	int len;
	char buf[1000];
	struct Packet msg;
	struct Ack ack;
	msg.done = 0;
	msg.seqNo = 0;

	fd_set recv_fds;
	FD_ZERO(&recv_fds);
	FD_SET(s,&recv_fds);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = atof(argv[4]);

	while ((fscanf(fp, "%s\n", buf)) > 0)
	{
		strcpy(msg.line, buf);
		int check = 1;
		while (check == 1) 
		{	
			sendto(s, &msg, sizeof(struct Packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));			
			
			while (1) /* Wait */
			{
				timeout.tv_sec = 0;
				timeout.tv_usec = atof(argv[4]);
				
				if (select(FD_SETSIZE, (fd_set *)&recv_fds, NULL, NULL, &timeout) != 0)
				{
					recvfrom(s, &ack, sizeof(struct Ack), 0, (struct sockaddr *)&dest_addr, &dest_addr_len);
				}
				else 
				{
					check = 1;
					break;
				}
				
				if (ack.seqNo != msg.seqNo)
				{
					continue;
				}
				else
				{
					msg.seqNo++;
					check = 0;
					break;
				}
			}

		}
		
	}
	
	fclose(fp);
	msg.done = 1;
	sendto(s, &msg, sizeof(struct Packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	recvfrom(s, &ack, sizeof(struct Ack), 0, (struct sockaddr *)&dest_addr, &dest_addr_len);

}























