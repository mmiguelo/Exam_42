#include <stddef.h>

size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t i = 0;
	size_t n = 0;

	while (*s)
	{
		while (reject[i] & *s != reject[i])
			i++;
		if (reject[i] != '\0')
			return (n);
		i = 0;
		n++;
		s++;
	}
	return (n);
}
