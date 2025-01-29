#include <stdlib.h>
#include <unistd.h>

int     *ft_range(int start, int end)
{
	int i = 0;
	int n = abs((end - start)) + 1;
	int *range = malloc(sizeof(int) * n);
	
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
