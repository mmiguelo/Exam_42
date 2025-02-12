/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmiguelo <mmiguelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:58:13 by mmiguelo          #+#    #+#             */
/*   Updated: 2025/02/12 13:00:10 by mmiguelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>

void	put_str(char *str, int *ret)
{
	if (!str)
		str = "(null)";
	while (*str)
		*ret += write(1, str++, 1);
}

void	put_digit(long long int nbr, int base, int *ret)
{
	char	*hexa = "0123456789abcdef";

	if (nbr < 0)
	{
		nbr *= -1;
		*ret += write(1, "-", 1);
	}
	if (nbr >= base)
		put_digit((nbr / base), base, ret);
	*ret += write(1, &hexa[nbr % base], 1);
}

int	ft_printf(const char *input, ...)
{
	int			ret = 0;
	va_list		args;

	va_start(args, input);
	while (*input)
	{
		if ((*input == '%') && *(input + 1))
		{
			input++;
			if (*input == 's')
				put_str(va_arg(args, char *), &ret);
			else if (*input == 'd')
				put_digit((long long int)va_arg(args, int), 10, &ret);
			else if (*input == 'x')
				put_digit((long long int)va_arg(args, unsigned int), 16, &ret);
		}
		else
			ret += write(1, input, 1);
		input++;
	}
	return (va_end(args), ret);
}

/*
int		main()
{
	printf("%s\n", "this is a string");
	ft_printf("%s\n", "this is a string");

	printf("Magic %s is %d\n", "number", 42);
	ft_printf("Magic %s is %d\n", "number", 42);

	printf("Hexadecimal for %d is %x\n", 42, 42);
	ft_printf("Hexadecimal for %d is %x\n", 42, 42);

	printf("Hexadecimal for %d is %x\n", 3456, 3456);
	ft_printf("Hexadecimal for %d is %x\n", 3456, 3456);

	return (0);
}
*/
