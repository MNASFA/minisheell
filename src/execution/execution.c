#include "execution.h"

static int	ft_lstsize(t_env *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
		i++;
		lst = lst ->next;
	}
	return (i);
}

static int	ft_lstsize_commands(t_exec *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
		i++;
		lst = lst ->next;
	}
	return (i);
}

int open_infiles(t_exec *commands)
{
	int fd;

    fd = 0;
	while (commands->infiles)
    {
		fd = open(commands->infiles->filename, O_RDONLY);
        commands->infiles = commands->infiles->next;
    }
	return(fd);
}

int open_outfiles(t_exec *commands)
{
	int fd;

    fd = 1;
		while (commands ->outfiles)
		{
			if (commands->outfiles->append == 0)
				fd = open(commands->outfiles->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else
				fd = open(commands->outfiles->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
            commands->outfiles= commands->outfiles->next;
        }
	return(fd);
}

void filtre_comands(t_exec **commands)
{
	if (commands == NULL || *commands == NULL)
		return;

	t_exec *cur = *commands;

	if ((cur ->infiles || cur ->outfiles) && !(cur->cmd))
	{
		*commands = cur->next;
		free(cur);
		filtre_comands(commands);
	}
	cur = *commands;
	filtre_comands(&cur->next);
}

void close_all_pipes(t_exee *exee)
{
    int i;
    
    i = 0;
    while (i < exee->cmd_count - 1)
    {
        close(exee->pipes[i][0]);
        close(exee->pipes[i][1]);
        i++;
    }
}
char **env_list_to_array(t_env *env)
{
	char **enve;
	int i;
	i = 0;

	enve = (char **)malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	while(env)
	{
		enve[i] = env->full;
		i++;
		env = env->next;
	}
	enve[i]= NULL;
	return (enve);
}

t_exee *init_execution(t_exec *commands)
{
    t_exee *exe;
    int i;
    
    exe = malloc(sizeof(t_exee));
    if (!exe)
        return NULL;
    exe->cmd_count = ft_lstsize_commands(commands);
    exe->infile = STDIN_FILENO;
    exe->outfile = STDOUT_FILENO;
    if (exe->cmd_count > 1) {
        exe->pipes = malloc(sizeof(int *) * (exe->cmd_count - 1));
        if (!exe->pipes) {
            free(exe);
            return NULL;
        }
        i = 0;
        while (i < exe->cmd_count - 1) {
            exe->pipes[i] = NULL;
            i++;
        }
    } 
    else
        exe->pipes = NULL;
    exe->pids = malloc(sizeof(int) * exe->cmd_count);
    if (!exe->pids)
        return (free(exe->pipes), free(exe), NULL);
    i = 0;
    while (i < exe->cmd_count) {
        exe->pids[i] = 0;
        i++;
    }
    return exe;
}

void	freeee(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
char	*get_full_path(char *argv, char **env)
{
	char	**parse_array;
	char	**dir;
	char	*path;
	int		i;
	int		existence;

	i = 0;
	while (strncmp(env[i], "PATH=", 5))
		i++;
	parse_array = ft_split_exe(argv, ' ');
	dir = ft_split_exe(env[i] + 5, ':');
	i = 0;
	while (dir[i])
	{
		dir[i] = ft_strjoin(dir[i], "/");
		dir[i] = ft_strjoin(dir[i], parse_array[0]);
		existence = access(dir[i], X_OK);
		if (existence == 0)
		{
			path = ft_strdup(dir[i]);
			return (freeee(dir), freeee(parse_array), path);
		}
		i++;
	}
	return (freeee(dir), freeee(parse_array), NULL);
}

char	*get_full_path_f(char *argv, char **env)
{
	char	**parse_array;
	char	*str;

	parse_array = ft_split_exe(argv, ' ');
	if (strncmp(parse_array[0], "./", 2) == 0)
	{
		str = ft_strdup(parse_array[0]);
		freeee(parse_array);
		return (str);
	}
	else
		return (get_full_path(argv, env));
}

void setup_pipes(t_exee *exee)
{
    int i = 0;
    
    if (exee->cmd_count <= 1)
        return;
        
    while (i < exee->cmd_count - 1)
    {
        if (!exee->pipes[i])
        {
            exee->pipes[i] = malloc(sizeof(int) * 2);
            if (!exee->pipes[i])
            {
                perror("malloc failed for pipe");
                exit(EXIT_FAILURE);
            }
        }
        if (pipe(exee->pipes[i]) < 0)
        {
            perror("pipe creation failed");
            exit(EXIT_FAILURE);
        }
        i++;
    }
}

void setup_first_command_io(t_exee *exee, t_exec *cmd, int *cmd_infile, int *cmd_outfile)
{
    if (cmd->infiles)
    {
        *cmd_infile = open_infiles(cmd);
        if (*cmd_infile < 0)
        {
            perror("Failed to open input file");
            exit(EXIT_FAILURE);
        }
    }
    else if (exee->infile != STDIN_FILENO)
        *cmd_infile = exee->infile;
    if (cmd->outfiles)
    {
        *cmd_outfile = open_outfiles(cmd);
        if (*cmd_outfile < 0)
        {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }
    }
    else if (exee->cmd_count > 1)
        *cmd_outfile = exee->pipes[0][1];
}

void setup_middle_command_io(t_exee *exee, t_exec *cmd, int cmd_index, int *cmd_infile, int *cmd_outfile)
{
    if (cmd->infiles)
    {
        close(exee->pipes[cmd_index - 1][0]);
        *cmd_infile = open_infiles(cmd);
        if (*cmd_infile < 0)
        {
            perror("Failed to open input file");
            exit(EXIT_FAILURE);
        }
    }
    else
        *cmd_infile = exee->pipes[cmd_index - 1][0];
    if (cmd->outfiles)
    {
        close(exee->pipes[cmd_index][1]);
        *cmd_outfile = open_outfiles(cmd);
        if (*cmd_outfile < 0)
        {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }
    }
    else
        *cmd_outfile = exee->pipes[cmd_index][1];
}

void setup_last_command_io(t_exee *exee, t_exec *cmd, int cmd_index, int *cmd_infile, int *cmd_outfile)
{
    if (cmd->infiles)
    {
        close(exee->pipes[cmd_index - 1][0]);
        *cmd_infile = open_infiles(cmd);
        if (*cmd_infile < 0)
        {
            perror("Failed to open input file");
            exit(EXIT_FAILURE);
        }
    }
    else
        *cmd_infile = exee->pipes[cmd_index - 1][0];
    if (cmd->outfiles)
    {
        *cmd_outfile = open_outfiles(cmd);
        if (*cmd_outfile < 0)
        {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }
    }
    else if (exee->outfile != STDOUT_FILENO)
        *cmd_outfile = exee->outfile;
}

void setup_command_io(t_exee *exee, t_exec *cmd, int cmd_index, int *cmd_infile, int *cmd_outfile)
{
    if (cmd_index == 0)
        setup_first_command_io(exee, cmd, cmd_infile, cmd_outfile);
    else if (cmd_index == exee->cmd_count - 1)
        setup_last_command_io(exee, cmd, cmd_index, cmd_infile, cmd_outfile);
    else
        setup_middle_command_io(exee, cmd, cmd_index, cmd_infile, cmd_outfile);
}

void execute_child_process(t_exee *exee, t_exec *cmd, int cmd_infile, int cmd_outfile, char **env)
{
    char *str = NULL;

    if (cmd_infile != STDIN_FILENO)
        dup2(cmd_infile, STDIN_FILENO);
    if (cmd_outfile != STDOUT_FILENO)
        dup2(cmd_outfile, STDOUT_FILENO);
    close_all_pipes(exee);
    if (cmd_infile != STDIN_FILENO)
        close(cmd_infile);
    if (cmd_outfile != STDOUT_FILENO)
        close(cmd_outfile);
    str = get_full_path_f(cmd->cmd, env);
    if (!str)
    {
        fprintf(stderr, "Command not found: %s\n", cmd->cmd);
        exit(EXIT_FAILURE);
    }
    if (execve(str, cmd->args, env) == -1)
    {
        perror("execve failed");
        free(str);
        exit(EXIT_FAILURE);
    }
}

void execute_commands(t_exee *exee, t_exec *commands, char **env)
{
    t_exec *cmd;
    int cmd_index;

    setup_pipes(exee);
    cmd = commands;
    cmd_index = 0;
    while (cmd_index < exee->cmd_count)
    {
        int cmd_infile = STDIN_FILENO;
        int cmd_outfile = STDOUT_FILENO;
        
        exee->pids[cmd_index] = fork();
        if (exee->pids[cmd_index] == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (exee->pids[cmd_index] == 0)
        {
            setup_command_io(exee, cmd, cmd_index, &cmd_infile, &cmd_outfile);
            execute_child_process(exee, cmd, cmd_infile, cmd_outfile, env);
        }  
        cmd = cmd->next;
        cmd_index++;
    }
    close_all_pipes(exee);
    if (exee->infile != STDIN_FILENO)
        close(exee->infile);
    if (exee->outfile != STDOUT_FILENO)
        close(exee->outfile);
}

void cleanup_exe(t_exee *exe)
{
    int i;
    
    close_all_pipes(exe);
    i = 0;
    while (i < exe->cmd_count - 1)
    {
        free(exe->pipes[i]);
        i++;
    }
    free(exe->pipes);
    free(exe->pids);
    free(exe);
}

void execution(t_exec *commands, t_env *envi)
{
    t_exee *exe;
    t_exec *cmdd;
    char **env;
    int i;
    int status;

    env = env_list_to_array(envi);
    i = 0;
    cmdd = (t_exec *)malloc(sizeof(t_exec));
    cmdd = commands;
    
    filtre_comands(&cmdd);
    if (!cmdd)
	{
		printf("No commands to execute after filtering\n");
        return;
    }
    exe = init_execution(cmdd);
    execute_commands(exe, cmdd, env);
    for (i = 0; i < exe->cmd_count; i++)
		waitpid(exe->pids[i], &status, 0);
    cleanup_exe(exe);
}
