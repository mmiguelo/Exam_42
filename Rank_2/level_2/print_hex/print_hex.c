#include <unistd.h>

int	ft_atoi(char *str)
{
	int r = 0;
	while(*str)
		r = r * 10 + (*str++ - '0');
	return (r);
}

void ft_printhex(int nb)
{
	char *base = "0123456789abcdef";

	if (nb >= 16)
		ft_printhex(nb / 16);
	write(1, &base[nb % 16], 1);
}

int main(int ac, char **av)
{
	if (ac ==2)
		ft_printhex(ft_atoi(av[1]));
	write(1, "\n", 1);
	return (0);
}
