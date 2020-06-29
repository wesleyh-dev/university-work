/* THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - WESLEY HARMON */

#include <stdio_ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utime.h>
#include <ustat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define ARMAG	"!<arch>\n" 		/* String that begins an archive file.  */
#define SARMAG	8							/* Size of that string.  */
#define ARFMAG 	"`\n"					/* String in ar_fmag at end of each header. (2 bytes) */

struct ar_hdr {
	char ar_name[16];						/* Member file name, sometimes / terminated. */
	char ar_date[12];						/* File date, decimal seconds since Epoch.  */
	char ar_uid[6], ar_gid[6];	/* User and group IDs, in ASCII decimal.  */
	char ar_mode[8];						/* File mode, in ASCII octal.  */
	char ar_size[10];						/* File size, in ASCII decimal.  */
	char ar_fmag[2];						/* Always contains ARFMAG.  */
															/* Total of 60 bytes*/
};

int afd;

// If 'q' is given as the option
int qfunc(int argc, char *argv[]) {

	afd = open(argv[2], O_CREAT | O_RDWR);
	if (afd < 0)	return -1;

	// Create archive if it does not exist and set mtime and set permissions to 0666
	struct stat arstat;
	char *achk[strlen(ARMAG)];
	if (read(afd, achk, SARMAG) != SARMAG) {
		write(afd, ARMAG, 8);
		if (stat(argv[2], &arstat) < 0) return -1;
		mode_t md = 0666;
		arstat.st_mode = md;
	}
	// Set timestamp of archive
	time_t artm = arstat.st_mtime;
	struct utimbuf art = {artm, artm};
	if (utime(argv[2], &art) < 0 ) return -1;


	// Append files to the archive
	for (int i = 3; i < argc; i++) {

		struct stat fdstat;
		struct ar_hdr afile;

		int fd = 0;
		if ((fd = open(argv[i], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IRGRP | S_IROTH | S_IWOTH)) < 0) return -1;

		if (stat(argv[i], &fdstat) < 0) return -1;

		// NAME

		// DATE
		time_t mtm = fdstat.st_mtime;
		struct utimbuf artimes = {mtm, mtm};
		snprintf(afile.ar_date, sizeof(struct ar_hdr[1]), "%ld", artimes.modtime);

		// UID & GID
		snprintf(afile.ar_uid, sizeof(struct ar_hdr[2]),"%d", fdstat.st_uid);
		snprintf(afile.ar_gid, sizeof(struct ar_hdr[3]),"%d", fdstat.st_gid);

		// MODE
		int usr = 0;
		int grp = 0;
		int oth = 0;
			// Restore permissions if archive
		if ((fdstat.st_mode & S_IRUSR)) usr+=4;
		if ((fdstat.st_mode & S_IWUSR)) usr+=2;
		if ((fdstat.st_mode & S_IXUSR)) usr+=1;

		if ((fdstat.st_mode & S_IRGRP)) grp+=4;
		if ((fdstat.st_mode & S_IWGRP)) grp+=2;
		if ((fdstat.st_mode & S_IXGRP)) grp+=1;

		if ((fdstat.st_mode & S_IROTH)) oth+=4;
		if ((fdstat.st_mode & S_IWOTH)) oth+=2;
		if ((fdstat.st_mode & S_IXOTH)) oth+=1;

		snprintf(afile.ar_mode, sizeof(struct ar_hdr[4]), "0%d%d%d", usr, grp, oth);

		// SIZE
		snprintf(afile.ar_size, sizeof(struct ar_hdr[5]),"%ld", fdstat.st_size);

		// ARFMAG
		strncpy(afile.ar_fmag, ARFMAG, sizeof(struct ar_hdr[6]));

		// Write the file's header info to the archive


		// if (lseek(afd, 0, SEEK_END) < 0 ) return -1;
		// if (write(afd, afile.ar_name, sizeof(struct ar_hdr[0])) < 0 ) return -1;
		// if (write(afd, afile.ar_date, sizeof(struct ar_hdr[1])) < 0 ) return -1;

		// if (write(afd, afile.ar_uid, sizeof(struct ar_hdr[2])) < 0 ) return -1;
		// if (write(afd, afile.ar_gid, sizeof(struct ar_hdr[3])) < 0 ) return -1;
		// if (write(afd, afile.ar_mode, sizeof(struct ar_hdr[4])) < 0 ) return -1;
		// if (write(afd, afile.ar_size, sizeof(struct ar_hdr[5])) < 0 ) return -1;
		// if (write(afd, afile.ar_fmag, sizeof(struct ar_hdr[6])) < 0 ) return -1;


	}

	close(afd);
	return 0;
}

// If 'x' is given as the option


// If 't' is given as the option


// If 'A' is given as the option


int main(int argc, char *argv[])
{
	// Check number of arguments
	if (argc <= 2) {
		printf("Arguments are in the wrong format. Need more information.\n");
		return -1;
	}

	// Check that only one option is used
	if (strlen(argv[1]) > 1){
		printf("Arguments are in the wrong format. Please provide only one option.\n");
		return -1;
	}

	// 'q' option
	if (strcmp(argv[1], "q") == 0) {
		if (argc <= 3) {
			printf("Arguments are in the wrong format. Please provide valid arguments.\n");
			return -1;
		}
		if (qfunc(argc, argv) < 0) printf("Error using option 'q'\n");
	}



	return 0;
}
