#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>

#define ARFMAG "`\n"
#define ARHDR "!<arch>\n"

void qfunc (int argc, char *argv[]) { /* Quickly append files and create new archive if necessary */

	char *arname = argv[2];
	//printf("Function q was reached with archive filename: %s\n",arname);

	int arfd; 
	if ((arfd = open(arname,O_RDWR | O_CREAT | O_APPEND, 0666)) == -1) {
		perror("open");
		exit(errno);
	}
	struct stat arstat;
	if (stat(arname,&arstat) == -1) {
		perror("stat");
		exit(errno);
	}
	if (arstat.st_size == 0) { /* If archive does not exist, write the header to the file */
		char *buf = ARHDR;
		if (write(arfd,buf,strlen(buf)) == -1) {
			perror("write");
			exit(errno);
		}
		//printf("Head was written to the archive file: %s",buf);
	}

	int i = 3;
	while (i<argc) { /* Append files to the archive without checking if they already exist */
		struct 
		{
			char name[17];		/* Name of this member.  */
			char date[13];		/* File mtime.  */
			char uid[7];		/* Owner uid; printed as decimal.  */
			char gid[7];		/* Owner gid; printed as decimal.  */
			char mode[9];		/* File mode, printed as octal.   */
			char size[11];		/* File size, printed as decimal.  */
			char fmag[3];		/* Should contain ARFMAG.  */
		} hdr;
		
		int fd;
		if ((fd = open(argv[i],O_RDONLY)) == -1) {
			perror("open");
			exit(errno);
		}
		
		struct stat fstat;
		if (stat(argv[i],&fstat) == -1) {
			perror("stat");
			exit(errno);
		}

		char *buf = (char *)malloc(17);
		strcpy(buf,argv[i]);
		strcat(buf,"/");
		sprintf(hdr.name,"%-16s",buf);
		if (write(arfd,hdr.name,strlen(hdr.name)) == -1) {
			perror("write");
			exit(errno);
		}

		sprintf(hdr.date,"%-12ld",fstat.st_mtime);
		if (write(arfd,hdr.date,strlen(hdr.date)) == -1) {
			perror("write");
			exit(errno);
		}
		
		sprintf(hdr.uid,"%-6u",fstat.st_uid);
		if (write(arfd,hdr.uid,strlen(hdr.uid)) == -1) {
			perror("write");
			exit(errno);
		}

		sprintf(hdr.gid,"%-6u",fstat.st_gid);
		if (write(arfd,hdr.gid,strlen(hdr.gid)) == -1) {
			perror("write");
			exit(errno);
		}
		
		sprintf(hdr.mode,"%-8o",fstat.st_mode);
		if (write(arfd,hdr.mode,strlen(hdr.mode)) == -1) {
			perror("write");
			exit(errno);
		}

		sprintf(hdr.size,"%-10ld",fstat.st_size);
		if (write(arfd,hdr.size,strlen(hdr.size)) == -1) {
			perror("write");
			exit(errno);
		}

		sprintf(hdr.fmag,"%-2s",ARFMAG);
		if (write(arfd,hdr.fmag,strlen(hdr.fmag)) == -1) {
			perror("write");
			exit(errno);
		}
		char *iobuf = (char *)malloc(fstat.st_blksize);
		while (read(fd,iobuf,sizeof(iobuf))) {
						
			if (write(arfd,iobuf,sizeof(iobuf)) == -1) {
				perror("write");
				exit(errno);
			}
		}
		close(fd);
		if (i+1<argc) write(arfd,"\n",1);
		i++;
	}

	close(arfd);
}

void xfunc(int argc, char *argv[]) {
	char *arname = argv[2];
	int arfd; 
	if ((arfd = open(arname,O_RDONLY)) == -1) {		
		perror("open");
		exit(errno);
	}

	int i = 3;
	while (i<argc) {
		lseek(arfd,8,SEEK_SET);
		char *arg = argv[i];
		int fd;
		if ((fd = open(arg,O_WRONLY | O_CREAT, 0666)) == -1) {
			perror("open");
			exit(errno);
		}
		
		char buf[17];
		if (read(arfd,buf,16) == -1) {
			perror("read");
			exit(errno);
		}
		buf[17] = '\0';
		int j = 0;
		char name[17];
		while (j<16) { // Extract actual file name from archive
			if (buf[j] == '/') {
				name[j] = '\0';
				break;
			}
			name[j] = buf[j];
			j++;
		}
		
		if (strcmp(name,arg) != 0) { // Requested file does not exist in archive
			lseek(arfd,-16,SEEK_CUR);
			unlink(arg);
		} else { // We have a match for extraction
			struct stat buf;
			if (stat(arg,&buf) == -1) {
				perror("stat");
				exit(errno);
			}
			struct utimbuf newtime;
			if (utime(arg,&newtime) == -1) {
				perror("utime");
				exit(errno);
			}
			newtime.actime = buf.st_mtime;
			newtime.modtime = buf.st_mtime;
			
			lseek(arfd,32,SEEK_CUR); // Seek to size 

			char csize[10];
			int size;
			if (read(arfd,csize,10) == -1) {
				perror("read");
				exit(errno);
			}
			size = atoi(csize);
			
			lseek(arfd,2,SEEK_CUR);
			char data[size];
			if (read(arfd,data,size) == -1) {
				perror("read");
				exit(errno);
			}
			if (write(fd,data,size) == -1) {
				perror("read");
				exit(errno);
			}
			lseek(arfd,2,SEEK_CUR); // Seek to next file name in archive
						
		}
		close(fd);
		i++;
	}
	close(arfd);
}

void afunc(int argc, char *argv[]) {

	DIR *dirp;
	struct dirent *dp;
	struct stat buf;
	if ((dirp = opendir(".")) == NULL) {
		perror("opendir");
		exit(errno);
	}
	if (chdir(".") == -1) {
		perror("chdir");
		exit(errno);
	}

	char *arname = argv[2];
	int arfd; 
	if ((arfd = open(arname,O_RDWR | O_APPEND)) == -1) {
		perror("open");
		exit(errno);
	}

	while ((dp = readdir(dirp)) != NULL) {
		if (stat(dp->d_name,&buf) == -1) {
			perror("stat");
			exit(errno);
		}
		
		if (S_ISREG(buf.st_mode) && strcmp(dp->d_name,arname) != 0 && strcmp(dp->d_name,"myar.c") != 0 && strcmp(dp->d_name,"myar") != 0) { // Handle all regular files
			
			printf("Adding file %s to the archive\n",dp->d_name);
			struct 
			{
				char name[17];		/* Name of this member.  */
				char date[13];		/* File mtime.  */
				char uid[7];		/* Owner uid; printed as decimal.  */
				char gid[7];		/* Owner gid; printed as decimal.  */
				char mode[9];		/* File mode, printed as octal.   */
				char size[11];		/* File size, printed as decimal.  */
				char fmag[3];		/* Should contain ARFMAG.  */
			} hdr;
		
			int fd;
			if ((fd = open(dp->d_name,O_RDONLY)) == -1) {
				perror("open");
				exit(errno);
			}
		
			struct stat fstat;
			if (stat(dp->d_name,&fstat) == -1) {
				perror("stat");
				exit(errno);
			}

			// Check for mod time
			time_t curtime;
			time(&curtime);
			if (fstat.st_mtime < curtime - 2*60*60) {
				continue;
			}

			char *buf = (char *)malloc(17);
			strcpy(buf,dp->d_name);
			strcat(buf,"/");
			sprintf(hdr.name,"%-16s",buf);
			if (write(arfd,hdr.name,strlen(hdr.name)) == -1) {
				perror("write");
				exit(errno);
			}

			sprintf(hdr.date,"%-12ld",fstat.st_mtime);
			if (write(arfd,hdr.date,strlen(hdr.date)) == -1) {
				perror("write");
				exit(errno);
			}
		
			sprintf(hdr.uid,"%-6u",fstat.st_uid);
			if (write(arfd,hdr.uid,strlen(hdr.uid)) == -1) {
				perror("write");
				exit(errno);
			}

			sprintf(hdr.gid,"%-6u",fstat.st_gid);
			if (write(arfd,hdr.gid,strlen(hdr.gid)) == -1) {
				perror("write");
				exit(errno);
			}
		
			sprintf(hdr.mode,"%-8o",fstat.st_mode);
			if (write(arfd,hdr.mode,strlen(hdr.mode)) == -1) {
				perror("write");
				exit(errno);
			}

			sprintf(hdr.size,"%-10ld",fstat.st_size);
			if (write(arfd,hdr.size,strlen(hdr.size)) == -1) {
				perror("write");
				exit(errno);
			}

			sprintf(hdr.fmag,"%-2s",ARFMAG);
			if (write(arfd,hdr.fmag,strlen(hdr.fmag)) == -1) {
				perror("write");
				exit(errno);
			}
		
			while (1) {
				char *iobuf = (char *)malloc(fstat.st_blksize);
				if (read(fd,iobuf,sizeof(iobuf)) == -1) {
					perror("read");
					exit(errno);
				}			
				if (write(arfd,iobuf,sizeof(iobuf)) == -1) {
					perror("write");
					exit(errno);
				}
			}
			close(fd);
		}
	}

}

int main (int argc, char *argv[]) {
	if (argc == 1) {
		printf("Usage: not enough arguments.\n");
		exit(1);
	}

	char *key = argv[1];
	//printf("This is key: %s\n",key);
	if (strcmp(key,"q") == 0) {
		qfunc(argc,argv);
	} else if (strcmp(key,"x") == 0) {
		xfunc(argc,argv);
	} else if (strcmp(key,"A") == 0 && argc == 3) {
		afunc(argc,argv);
	}
	

}
