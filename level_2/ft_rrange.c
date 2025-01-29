#include <stdlib.h>
#include <stdio.h>

int     *ft_rrange(int start, int end)
{
	int i = 0;
	int n = end - start + 1;
	int *range;

	if (start > end)
		return (ft_rrange(end, start));
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
