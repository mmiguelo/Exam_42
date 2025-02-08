#include <stddef.h>

size_t	ft_strspn(const char *s, const char *accept)
{
	size_t i = 0;
	size_t n = 0;

	while (*s)
	{
		while(accept[i] && *s != accept[i])
			i++;
		if (accept[i] == '\0')
			return (n);
		i = 0;
		n++;
		s++;
	}
	return (n);
}

