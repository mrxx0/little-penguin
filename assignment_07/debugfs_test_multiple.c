#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define SIZE 4096

int main()
{
	int foo_fd;
	int foo_fd_user;
	char tmp[SIZE];
	int read_ret;
	pid_t userpid;
	int userpid_status = 0;

	printf("\n[---\tRead and write test from multiple users...\t---]\n");
	printf(" Opening and writing to /sys/kernel/debug/fortytwo/foo as root\n");
	printf("REAL UID = [%d] | EFFECTIVE UID = [%d]\n", getuid(), geteuid());
	if ((foo_fd = open("/sys/kernel/debug/fortytwo/foo", O_WRONLY)) == -1){
		perror("failed to open /sys/kernel/debug/fortytwo/foo\n");
		close(foo_fd);
	}
	printf("foo_fd = [%d]\n", foo_fd);
	printf("\n[----\tWriting to /sys/kernel/debug/fortytwo/foo...\t----]\n\n");
	write(foo_fd, "Hello There!\n", strlen("Hello There!\n"));
	userpid = fork();
	if (userpid < 0)
		perror("fork failed\n");
	if (userpid == 0)
	{
		setuid(1000);
		printf("REAL UID = [%d] | EFFECTIVE UID = [%d]\n", getuid(), geteuid());

		printf(" Opening /sys/kernel/debug/fortytwo/foo as user\n");
		if ((foo_fd_user = open("/sys/kernel/debug/fortytwo/foo", O_RDONLY)) == -1){
			perror("failed to open /sys/kernel/debug/fortytwo/foo\n");
			close(foo_fd_user);
		}
		printf("foo_fd_user = [%d]\n", foo_fd_user);
		printf("reading /sys/kernel/debug/fortytwo/foo as classic user\n");
		bzero(tmp, SIZE);
		if ((read_ret = read(foo_fd_user, tmp, SIZE)) == -1)
		{
			printf("Failed to read /sys/kernel/debug/fortytwo/foo\n");
			close(foo_fd_user);
		}
		if (read_ret > 0 && read_ret != SIZE)
			tmp[read_ret] = 0;
		tmp[SIZE - 1] = 0;
		printf("Content of /sys/kernel/debug/fortytwo/foo : \n");
		printf("[%s]\n", tmp);
		printf("Closing foo_fd_user\n");
		close(foo_fd_user);
		exit(0);

	}
	else
	{
		waitpid(userpid, &userpid_status, 0);
		printf("Closing foo_fd\n");
		close(foo_fd);
	}
	return 0;
}
