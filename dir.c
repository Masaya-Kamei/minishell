#include <stdio.h>
#include <dirent.h>

int	main(void)
{
	DIR				*dir;
	struct dirent	*dp;

	dir = opendir("/Users/kamei/Documents");
	if (dir == NULL)
	{
		printf("null\n");
		return (0);
	}
	dp = readdir(dir);
	while (dp != NULL)
	{
		printf("%s\n", dp->d_name);
		dp = readdir(dir);
	}
	return (0);
}
