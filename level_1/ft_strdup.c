#include <stdlib.h>

char    *ft_strdup(char *src)
{
	int i = 0;
	while (src[i])
		i++;
	char *new = malloc(sizeof(char) * (i + 1));
	if (!new)
		return NULL;
	i = 0;
	while(src[i])
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
