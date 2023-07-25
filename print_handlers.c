#include "shell.h"

/**
 * _putsfd - prints an input string
 * @s: the string to be printed
 * @fd: the filedescriptor to write to
 * Return: the number of chars put
 */
int _putsfd(char *s, int fd)
{
	int i = 0;

	if (!s)
		return (0);
	while (*s)
	{
		i += _putfd(*s++, fd);
	}
	return (i);
}


/**
 * _putfd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putfd(char c, int fd)
{
	static char buffer[BUFFER_SIZE];
	static int n;

	if (c == BUFFER_FLUSH || n >= BUFFER_SIZE)
	{
		write(fd, buffer, n);
		n = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[n++] = c;
	return (1);
}


/**
 * _eputs - prints an input string
 * @s: the string to be printed
 * Return: Nothing
 */
void _eputs(char *s)
{
	int i = 0;

	if (!s)
		return;
	while (s[i] != '\0')
	{
		_eputchar(s[i]);
		i++;
	}
}

/**
 * _eputchar - writes the character c to stderr
 * @c: The character to print
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _eputchar(char c)
{
	static int n;
	static char buffer[BUFFER_SIZE];

	if (c == BUFFER_FLUSH || n >= BUFFER_SIZE)
	{
		write(2, buffer, n);
		n = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[n++] = c;
	return (1);
}
