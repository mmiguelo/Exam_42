#include <stdlib.h>
#include <stdio.h>

int     *ft_rrange(int start, int end)
{
	int i;
	int n;
	int *range;

	i = 0;
	if (start > end)
		return (ft_rrange(end, start));
	n = end - start + 1;
	range = malloc(sizeof(int) * n);
	if (range)
	{
		while (i < n)
		{
			range[i] = end;
			end--;
			i++;
		}
	}
	return(range);
}
