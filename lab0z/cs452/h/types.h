/* virtual address */
typedef struct vad_t {
	unsigned un       :8,		/* segment number */
	         v_seg    :5,		/* segment number */
	 	 v_page   :10,		/* page number */
	         v_offset :9;		/* byte offset in page */
} vad_t;

/* page descriptor */
typedef struct pd_t {
	unsigned un	  :14,		/* unused */
		 pd_p	  :1,		/* presence bit */
		 pd_m	  :1,		/* modified bit */
		 pd_r	  :1,		/* reference bit */
	         pd_frame :15;		/* page frame number */
} pd_t;

/* segment descriptor */
typedef struct sd_t {
	unsigned un	 :18,		/* unused */
		 sd_p	 :1,		/* presence bit */
		 sd_prot :3,		/* access protection bits */
	         sd_len	 :10;		/* page table length */
	pd_t	 *sd_pta;		/* address of the page table */
} sd_t;

/* status register */
typedef struct sr_t {
      unsigned	 ps_t   :1,		/* trace */
                        :1,
		 ps_s	:1,		/* processor mode */
		 ps_m	:1,		/* translation */
		     	:1,		/* wait */
                 ps_int :3,             /* interrupt mask */
                        :3,
		 ps_x   :1,		/* extend */
		 ps_n	:1,		/* negative */
		 ps_z	:1,		/* zero */
		 ps_o	:1,		/* overflow */
	         ps_c	:1;		/* carry */
} sr_t;

/* 2 byte temporary storeage that holds the system #, program trap #, type of mm trap and device type */
/* these 2 bytes are included in the processor state for convenience. */
typedef union tmp_t {
	struct tmp_int {
	        unsigned	:10,		/* not used */
			 in_dev	:3,		/* device type */
		         in_dno	:3;		/* device number */
	} tmp_int;
	struct tmp_mm {
	       unsigned	 mm_typ :2,		/* type of mm trap */
			 mm_seg	:4,		/* segment number */
		         mm_pg	:10;		/* page number */
	} tmp_mm;
	struct tmp_pr {
		unsigned	:8,		/* not used */
			pr_typ	:8;		/* type of prog trap */
	} tmp_pr;
	struct tmp_sys {
		unsigned	:8,		/* not used */
		     sys_no	:8;		/* number of sys trap */
	} tmp_sys;
} tmp_t;

/* processor state */
typedef struct {
	int	 s_r[17];		/* d0-d7, a0-a7 + pc */
#define	s_sp	 s_r[15]			/* stack pointer */
#define s_pc	 s_r[16]			/* program counter */
	sr_t	 s_sr;			/* status register */
	tmp_t	 s_tmp;			/* 2 byte temporary storage */
        sd_t     *s_crp;                /* segment table address register */
} state_t;

/* Device registers */
typedef	struct {
	unsigned d_op,		/* operation register */
		 d_dadd;	/* address, amount, track or sector number */
#define	d_amnt	d_dadd		/* synonyms for the above */
#define	d_track	d_dadd
#define	d_sect	d_dadd
	char	 *d_badd;	/* buffer address register */
	unsigned d_stat;	/* status register */
} devreg_t;




typedef	struct {
	int	io_len;
	int	io_sta;
} iores_t;

