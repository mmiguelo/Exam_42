
void	ft_swap(int *a, int *b)
{
	int	temp = *a;
	*a = *b;
	*b = temp;
}

void sort_int_tab(int *tab, unsigned int size)
{
	unsigned int	i = 0;
	while(i < (size - 1))
	{
		if (tab[i] > tab[i + 1])
		{
			ft_swap(&tab[i], &tab[i + 1]);
			i = 0;
		}
		else
			i++;
	}
}