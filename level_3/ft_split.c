#include <stdlib.h>

int ft_str_c_len(char *str, char c)
{
	int i;
	int count;

	i = 0;
	count = 1;
	while (str[i])
	{
		if (str[i] == c)
		{
			while (str[i] == c)
				i++;
			count++;
		}
		i++;
	}
	return (count);
}

char	*ft_str_cpy(char *new, char *old, int size)
{
	new = malloc(sizeof(char) * (size + 1));
	if (new == NULL)
	{
		return (NULL);
	}
	int i = 0;
	while (i < size)
	{
		new[i] = old[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char    **ft_split(char *str)
{
	char **matriz;
	int i;
	int j;
	int n = 0;

	matriz = malloc(sizeof(char *) * (ft_str_c_len(str, ' ') + 1));
	if (matriz == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
			i++;
		j = i;
		while (str[i] && (str[i] != ' ' && str[i] != '\t'))
			i++;
		if (i > j)
			matriz[n++] = ft_str_cpy(matriz[n], &str[j], i - j);
	}
	matriz[n] = NULL;
	return (matriz);
}
