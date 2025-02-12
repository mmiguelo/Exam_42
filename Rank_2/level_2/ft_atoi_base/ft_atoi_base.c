int	check_char(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	else if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	return (-1);
}

int	ft_atoi_base(const char *str, int str_base)
{
	int i = 0;
	int result = 0;
	int sign = 1;
	int value;

	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		value = check_char(str[i]);
		if (value == -1 || value >= str_base)
			break;
		result = result * str_base + value;
		i++;
	}
	return (result * sign);
}
