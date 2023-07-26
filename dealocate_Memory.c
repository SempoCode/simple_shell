#include "shell.h"

/**
 * ptrfree - frees a pointer and NULLs the address
 * @s: pointer to the address to be freed
 * Return: 1 on success, otherwise 0.
 */
int ptrfree(void **s)
{
	if (s && *s)
	{
		free(*s);
		*s = NULL;
		return (1);
	}
	return (0);
}
