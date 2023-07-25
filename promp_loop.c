#include "shell.h"
/**
 * loop - the main shell loop
 * @data: structure with parameters
 * @av: the main argument vector
 * Return: 0 on success, 1 on error
 */
int loop(info_t *data, char **av)
{
	ssize_t n = 0;
	int builtin_ret = 0;

	while (n != -1 && builtin_ret != -2)
	{
		clear_info(data);
		if (interactive_mod(data))
			_puts("#cisfun$ ");
		_eputchar(BUFFER_FLUSH);
		n = get_input(data);
		if (n != -1)
		{
			set_info(data, av);
			builtin_ret = find_builtin(data);
			if (builtin_ret == -1)
				find_cmd(data);
		}
		else if (interactive_mod(data))
			_putchar('\n');
		free_info(data, 0);
	}
	writeHistory(data);
	free_info(data, 1);
	if (!interactive_mod(data) && data->status)
		exit(data->status);
	if (builtin_ret == -2)
	{
		if (data->err_num == -1)
			exit(data->status);
		exit(data->err_num);
	}
	return (builtin_ret);
}

/**
 * find_cmd - finds a command in path
 * @data: pointer to the info struct
 * Return: nothing
 */
void find_cmd(info_t *data)
{
	char *path = NULL;
	int i, n;

	data->path = data->argv[0];
	if (data->linecount_flag == 1)
	{
		data->line_count++;
		data->linecount_flag = 0;
	}
	for (i = 0, n = 0; data->arg[i]; i++)
		if (!isDelim(data->arg[i], " \t\n"))
			n++;
	if (!n)
		return;

	path = find_path(data, _getenv(data, "PATH="), data->argv[0]);
	if (path)
	{
		data->path = path;
		fork_cmd(data);
	}
	else
	{
		if ((interactive_mod(data) || _getenv(data, "PATH=") ||
					data->argv[0][0] == '/') &&
				is_cmd(data, data->argv[0]))
			fork_cmd(data);
		else if (*(data->arg) != '\n')
		{
			data->status = 127;
			print_error(data, "not found\n");
		}
	}
}


/**
 * find_builtin - finds a builtin command
 * @data: pointer to the struct
 * Return: -1 if builtin not found, 0 if builtin executed successfully,
 * 1 if builtin found but not successful, 2 if builtin signals exit()
 */
int find_builtin(info_t *data)
{
	int i, builtin = -1;

	builtin_table builtinlist[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtinlist[i].type; i++)
		if (_strcmp(data->argv[0], builtinlist[i].type) == 0)
		{
			data->line_count++;
			builtin = builtinlist[i].func(data);
			break;
		}

	return (builtin);
}


/**
 * fork_cmd - forks a an exec thread to run command
 * @data: pointer to the data struct
 * Return: void
 */
void fork_cmd(info_t *data)
{
	pid_t childPid;

	childPid = fork();
	if (childPid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (childPid == 0)
	{
		if (execve(data->path, data->argv, get_environ(data)) == -1)
		{
			free_info(data, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(data->status));
		if (WIFEXITED(data->status))
		{
			data->status = WEXITSTATUS(data->status);
			if (data->status == 126)
				print_error(data, "Permission denied\n");
		}
	}
}
