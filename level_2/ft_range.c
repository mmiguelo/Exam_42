#include <stdlib.h>
#include <unistd.h>

int     *ft_range(int start, int end)
{
	int i;
	int *range;
	int n;

	i = 0;
	n = abs((end - start)) + 1;
	range = malloc(sizeof(int) * n);
	if (range)
	{
		if (start < end)
			while (i < n)
				range[i++] = start++;
		else
			while (i < n)
				range[i++] = start--;
	}
	return (range);
}
