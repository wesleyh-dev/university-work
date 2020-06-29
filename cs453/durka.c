void f(char *s)
{
	char buf[426];
	strcpy(buf, s);
}

void agonistic_heaven(void)
{
	static uid_t uid;
	uid = geteuid();
	setresuid (uid, uid, uid);
}

void flugeldufel(void)
{
	system("/usr/bin/fortune"); /* Oh no! */
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		exit(1);

	uid_t uid = getuid();
	uid_t euid = geteuid();

	printf("running as uid = %u, euid = %u\n", uid, euid);

	f(argv[1]);

	return 0;
}

