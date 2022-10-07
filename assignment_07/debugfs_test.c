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
	int read_ret;
	char tmp[SIZE];

	/*	Open /sys/kernel/debug/foo */
	printf("[----\tOpening /sys/kernel/debug/fortytwo/foo... as root with READ and WRITE\t----]\n\n");
	if ((foo_fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR)) == -1) {
		perror("failed to open /sys/kernel/debug/fortytwo/foo");
		close(foo_fd);
	}
	printf("/sys/kernel/debug/fortytwo/foo fd = %d\n", foo_fd);

	bzero(tmp, SIZE);
	printf("\n[----\tReading /sys/kernel/debug/fortytwo/foo...\t----]\n\n");
	if ((read_ret = read(foo_fd, tmp, SIZE)) == -1)
	{
		printf("Failed to read /sys/kernel/debug/fortytwo/foo\n");
		close(foo_fd);
	}
	if (read_ret > 0 && read_ret != SIZE)
		tmp[read_ret] = 0;
	tmp[SIZE - 1] = 0;
	printf("Content of /sys/kernel/debug/fortytwo/foo : \n");
	printf("[%s]\n", tmp);

	printf("\n[----\tWriting to /sys/kernel/debug/fortytwo/foo...\t----]\n\n");
	write(foo_fd, "Hello There!\n", strlen("Hello There!\n"));
	bzero(tmp, SIZE);
	close(foo_fd);
	printf("Reopening /sys/kernel/debug/fortytwo/foo...\n");
	if ((foo_fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR)) == -1) {
		perror("failed to open /sys/kernel/debug/fortytwo/foo");
		close(foo_fd);
	}
	printf("/sys/kernel/debug/fortytwo/foo fd = %d\n", foo_fd);
	printf("Rereading /sys/kernel/debug/fortytwo/foo...\n");
	if ((read_ret = read(foo_fd, tmp, SIZE)) == -1)
	{
		printf("Failed to read /sys/kernel/debug/fortytwo/foo\n");
		close(foo_fd);
	}
	if (read_ret > 0 && read_ret != SIZE)
		tmp[read_ret] = 0;
	tmp[SIZE - 1] = 0;
	printf("Content of /sys/kernel/debug/fortytwo/foo : \n");
	printf("[%s]\n", tmp);

	setuid(1000);
	seteuid(1000);
	
	printf("[----\tOpening /sys/kernel/debug/fortytwo/foo as classic user READ AND WRITE\t----]\n\n");
	close(foo_fd);
	if ((foo_fd = open("/sys/kernel/debug/fortytwo/foo", O_RDWR)) == -1) {
		perror("failed to open /sys/kernel/debug/fortytwo/foo");
		close(foo_fd);
	}
	printf("Rereading /sys/kernel/debug/fortytwo/foo as classic user\n");
	bzero(tmp, SIZE);
	if ((read_ret = read(foo_fd, tmp, SIZE)) == -1)
	{
		printf("Failed to read /sys/kernel/debug/fortytwo/foo\n");
		close(foo_fd);
	}
	if (read_ret > 0 && read_ret != SIZE)
		tmp[read_ret] = 0;
	tmp[SIZE - 1] = 0;
	printf("Content of /sys/kernel/debug/fortytwo/foo : \n");
	printf("[%s]\n", tmp);

	printf("[----\tOpening /sys/kernel/debug/fortytwo/foo as classic user READ ONLY\t----]\n\n");
	close(foo_fd);
	if ((foo_fd = open("/sys/kernel/debug/fortytwo/foo", O_RDONLY)) == -1) {
		perror("failed to open /sys/kernel/debug/fortytwo/foo");
		close(foo_fd);
	}
	printf("Rereading /sys/kernel/debug/fortytwo/foo as classic user\n");
	bzero(tmp, SIZE);
	if ((read_ret = read(foo_fd, tmp, SIZE)) == -1)
	{
		printf("Failed to read /sys/kernel/debug/fortytwo/foo\n");
		close(foo_fd);
	}
	if (read_ret > 0 && read_ret != SIZE)
		tmp[read_ret] = 0;
	tmp[SIZE - 1] = 0;
	printf("Content of /sys/kernel/debug/fortytwo/foo : \n");
	printf("[%s]\n", tmp);

	printf("Closing foo_fd...\n");
	close(foo_fd);

	seteuid(0);
	printf("REAL UID = [%d] | EFFECTIVE UID = [%d]\n", getuid(), geteuid());
	printf("\n[---\tRead and write test from multiple users...\t---]\n");
	printf(" Opening and writing to /sys/kernel/debug/fortytwo/foo as root\n");
	if ((foo_fd = open("/sys/kernel/debug/fortytwo/foo", O_WRONLY)) == -1){
		perror("failed to open /sys/kernel/debug/fortytwo/foo\n");
		close(foo_fd);
	}
	printf("\n[----\tWriting to /sys/kernel/debug/fortytwo/foo...\t----]\n\n");
	write(foo_fd, "Hello There!\n", strlen("Hello There!\n"));
	printf("Foo is still open by root.\n");
	int foo_fd_user = 0;
	setuid(1000);
	seteuid(1000);
	printf("Opening and reading /sys/kernel/debug/fortytwo/foo as user\n");
	if ((foo_fd_user = open("/sys/kernel/debug/fortytwo/foo", O_RDONLY)) == -1){
		perror("failed to open /sys/kernel/debug/fortytwo/foo\n");
		close(foo_fd);
	}
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
	printf("Closing foo_fd\n");
	close(foo_fd);





}	
