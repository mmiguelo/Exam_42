#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"
//#include <stdio.h>
//#include <fcntl.h>

char	*ft_strchr(char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return (s);
		s++;
	}
	return (NULL);
}

size_t	ft_strlen(const char *str)
{
	size_t		i = 0;

	while (str[i])
		i++;
	return (i);
}

void	ft_strcpy(char *dest, const char *src)
{
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
}

char	*ft_strdup(const char *src)
{
	size_t	len = ft_strlen(src);
	char	*dest = malloc(len + 1);

	if (!dest)
		return (NULL);
	ft_strcpy(dest, src);
	return (dest);
}

char	*ft_strjoin(char *s1, const char *s2)
{
	size_t	len_s1 = ft_strlen(s1);
	size_t	len_s2 = ft_strlen(s2);
	char	*join = malloc(len_s1 + len_s2 + 1);

	if (!s1 || !s2 || !join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy(join + len_s1, s2);
	free(s1);
	return (join);
}

char	*get_next_line(int fd)
{
	static char		buff[BUFFER_SIZE + 1];
	char			*line;
	char			*newline;
	int				countread;
	int				tocopy;

	line = ft_strdup(buff);
	while (!(newline = ft_strchr(line, '\n')) && (countread = read(fd, buff, BUFFER_SIZE)) > 0)
	{
		buff[countread] = '\0';
		line = ft_strjoin(line, buff);
	}
	if (ft_strlen(line) == 0)
		return (free(line), NULL);
	if (newline)
	{
		tocopy = newline - line + 1;
		ft_strcpy(buff, newline + 1);
	}
	else
	{
		tocopy = ft_strlen(line);
		buff[0] = '\0';
	}
	line[tocopy] = '\0';
	return (line);
}

/*
int	main()
{
	int		fd;
	char	*line;

	// Open file as read-only
	fd = open("test", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return 1;
	}

	// Read and display each line in the file
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}

	// Close file
	close(fd);
	return 0;
}
*/