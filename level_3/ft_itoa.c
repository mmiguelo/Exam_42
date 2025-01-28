#include <stdlib.h>

int	get_length(int nbr)
{
	int len = 0;
	if (nbr <= 0)
		len = 1;
	while (nbr)
	{
		len++;
		nbr /= 10;
	}
	return (len);
}

char	*ft_itoa(int nbr)
{
	int len = get_length(nbr);
	char *new = malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	new[len] = '\0';
	if (nbr == 0)
		new[0] = '0';
	if (nbr < 0)
		new[0] = '-';
	while (nbr)
	{
		int digit = nbr % 10;
		if (digit < 0)
			digit = -digit;
		new[--len] = '0' + digit;
		nbr /= 10;
	}
	return (new);
}
