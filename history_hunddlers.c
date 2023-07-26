#include "shell.h"

/**
 * buildHistory_list - adds entry to a history linked list
 * @buffer: buffer
 * @data: data of the histry.
 * @lines: the history linecount, histcount
 * Return: Always 0
 */
int buildHistory_list(info_t *data, char *buffer, int lines)
{
	list_t *node = NULL;

	if (data->history)
		node = data->history;
	add_node_end(&node, buffer, lines);

	if (!data->history)
		data->history = node;
	return (0);
}

/**
 * writeHistory - creates a file, or appends to an existing file
 * @info: the parameter struct
 * Return: 1 on success, else -1
 */
int writeHistory(info_t *info)
{
	ssize_t fd;
	char *filename = getHistory_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);
	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
			{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUFFER_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * readHistory - reads history from file
 * @info: the parameter struct
 * Return: histcount on success, 0 otherwise
 */
int readHistory(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = getHistory_file(info);

	if (!filename)
		return (0);
	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			buildHistory_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		buildHistory_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);
	renumberHistory(info);
	return (info->histcount);
}


/**
 * renumberHistory - renumbers the history linked list after changes
 * @data: Structure containing potential arguments. Used to maintain
 * Return: the new histcount
 */
int renumberHistory(info_t *data)
{
	list_t *node = data->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
		return (data->histcount = i);
}


/**
 * getHistory_file - gets the history file
 * @data: parameter struct
 * Return: allocated string containg history file
 */

char *getHistory_file(info_t *data)
{
	char *buffer, *file;

	file = _getenv(data, "HOME=");
	if (!file)
		return (NULL);
	buffer = malloc(sizeof(char) * (_strlen(file) + _strlen(HIST_FILE) + 2));
	if (!buffer)
		return (NULL);
	buffer[0] = 0;
	_strcpy(buffer, file);
	_strcat(buffer, "/");
	_strcat(buffer, HIST_FILE);
	return (buffer);
}
