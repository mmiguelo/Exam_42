#include <unistd.h>

char *rev_print(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	while (i--)
		write(1, &str[i], 1);
	write(1, "\n", 1);
	return(str);
}
