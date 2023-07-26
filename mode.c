#include "shell.h"

/**
 * interactive_mod - returns true if shell is interactive mode
 * @data: struct address
 * Return: 1 if interactive mode, 0 otherwise
 */
int interactive_mod(info_t *data)
{
	return (isatty(STDIN_FILENO) && data->readfd <= 2);
}


/**
 * isDelim - checks if character is a delimeter
 * @z: the char to check
 * @delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int isDelim(char z, char *delim)
{
	while (*delim)
		if (*delim++ == z)
			return (1);
	return (0);
}
