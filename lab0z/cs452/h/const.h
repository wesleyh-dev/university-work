/* Hardware constants */
#define	PAGESIZE	512	/* page size in bytes */
#define	BEGINTRAP	0x800	/* beginning of trap areas */
#define	BEGININTR	0x9c8	/* beginning of interrupt areas */
#define	BEGINDEVREG	0x1400	/* beginning of device registers */

/* utility constants */
#define	TRUE		1
#define	FALSE		0
#define HIDDEN		static
#define NULL 		((void *)0)
#define ENULL		-1
#define	NOPROC		-1
#define	SECOND		1000000L

/* trap types */
#define	PROGTRAP	0	/* program trap */
#define	MMTRAP		1	/* memory management trap */
#define	SYSTRAP		2	/* system call trap */
#define TRAPTYPES	3	/* number of trap types */

/* Exception Vector Numbers */
#define BUSERROR        2
#define ADDRESSERROR    3
#define ILLEGALINSTR    4
#define ZERODIVIDE      5
#define PRIVILEGE       8
#define L1010EMULATOR  10
#define L1111EMULATOR  11

/* memory management trap codes */
#define	ACCESSPROT	0	/* access protection violation */
#define	PAGEMISS	1	/* missing page */
#define	INVPAGE		2	/* invalid page number */
#define	SEGMISS		3	/* missing segment */

/* device operation codes */
#define	IOREAD		0
#define	IOWRITE		1
#define	IOSEEK		2

/* device completion codes */
#define	NORMAL		0
#define	HARDFAILURE	1
#define	ILOP		2
#define	ILBUF		3
#define	ILAMOUNT	4
#define	ILTRACK		5
#define	ILSECTOR	6
#define ENDOFINPUT	7
#define	NOSUCHDEV	8
#define DEVNOTREADY	9

/* device numbers according to the position of their device registers */
#define	TERM0		0
#define	TERM1		1
#define	TERM2		2
#define	TERM3		3
#define	TERM4		4
#define	PRINT0		5
#define	PRINT1		6
#define	DISK0		7
#define	DISK1		8
#define	DISK2		9
#define	DISK3		10
#define	FLOPPY0		11
#define	FLOPPY1		12
#define	FLOPPY2		13
#define	FLOPPY3		14

#define TERMINAL 0
#define PRINTER  1
#define DISK     2
#define FLOPPY   3
#define NOTUSED  4
#define CLOCK    5    

/* operations */
#define	MIN(A,B)	((A) < (B) ? A : B)
#define MAX(A,B)	((A) < (B) ? B : A)
#define	EVEN(A)		(((unsigned)A & 01) == 0)

#define MAXPROC         20
#define SEMMAX          10
