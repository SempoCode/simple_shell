#include "shell.h"

/**
 * _putchar - function that prints a character.
 * @c: character to be printed
 * Return: integer.
 *
 */
int _putchar(char c)
{
	static int n;
	static char buffer[BUFFER_SIZE];

	if (c == BUFFER_FLUSH || n >= BUFFER_SIZE)
	{
		write(1, buffer, n);
n = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[n++] = c;
	return (1);
}


/**
 * _puts - prints an input string
 * @str: the string to be printed
 * Return: Nothing
 */

void _puts(char *str)
{
	int i = 0;

	if (!str)
		return;

	while (str[i] != '\0')
	{
		_putchar(str[i]);
		i++;
	}
}

/**
 * print_error - prints an error message
 * @info: the parameter & return info struct
 * @estr: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */
void print_error(info_t *info, char *estr)
{
	_eputs(info->fname);
	_eputs(": ");
	print_d(info->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(info->argv[0]);
	_eputs(": ");
	_eputs(estr);
}


/**
 * print_d - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 * Return: number of characters printed
 */

int print_d(int input, int fd)
{
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		count += _putchar('-');

	if (input < 0)
	{
		_abs_ = -input;

	}
	else
		_abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)

	{
		if (_abs_ / i)

		{
			count += _putchar('0' + current / i);
			current %= i;
		}

	}
	count += _putchar('0' + current);
	return (count);
}


/**
 * remove_comments - function replaces first instance of '#' with '\0'
 * @buffer: address of the string to modify
 * Return: Always 0;
 */
void remove_comments(char *buffer)
{
	int i;

	for (i = 0; buffer[i] != '\0'; i++)
		if (buffer[i] == '#' && (!i || buffer[i - 1] == ' '))
		{
			buffer[i] = '\0';
			break;
		}
}
