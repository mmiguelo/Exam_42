#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"
//#include <stdio.h> this are optional and used in main
//#include <fcntl.h> this are optional and used in main

char	*ft_strdup(const char *str)
{
	char	*dest;
	int	i = 0;

	if (!str)
		return (NULL);
	while (str[i])
        	i++;
    	dest = malloc(i + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*get_next_line(int fd)
{
	static char buffer[BUFFER_SIZE];
    	char        line[70000];
    	static int  buffer_pos;
    	static int  buffer_read;
    	int         i = 0;

    	if (fd < 0 || BUFFER_SIZE <= 0)
        	return (NULL);
    	while (1)
    	{
        	if (buffer_pos >= buffer_read)
        	{
            		buffer_read = read(fd, buffer, BUFFER_SIZE);
            		buffer_pos = 0;
            		if (buffer_read <= 0)
                		break;
        	}
        	line[i++] = buffer[buffer_pos++];
        	if (line[i - 1] == '\n')
            		break;
    	}
    	line[i] = '\0';
    	if (i == 0)
        	return (NULL);
    return (ft_strdup(line));
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
