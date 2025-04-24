#include "execution.h"



int open_infiles(t_exec *commands)
{
	int fd = 0;

	while(commands)
	{
		if (commands->infile)
			fd = open(commands->infile, O_RDONLY);
		commands = commands->next;
	}
	return(fd);
}

int open_outfiles(t_exec *commands)
{
	int fd = 1;

	while (commands)
	{
		if (commands ->outfile)
		{
			if (commands->append == 0)
				fd = open(commands->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else
				fd = open(commands->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
			}
			commands = commands->next;
	}
	return(fd);
}

void filtre_comands(t_exec **commands)
{
	if (commands == NULL || *commands == NULL)
		return;

	t_exec *cur = *commands;

	if ((cur ->infile || cur ->outfile) && !(cur->cmd))
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
    // close(exee->infile);
    // close(exee->outfile);
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
	exe = malloc(sizeof(t_exee));
	exe->cmd_count = ft_lstsize_commands(commands);
	exe->pipes = malloc(sizeof(int *) * (exe->cmd_count - 1));
	exe->pids = malloc(sizeof(int) * (exe->cmd_count));
	return(exe);
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

void execute_commands(t_exee *exee, t_exec *commands, char **env)
{
    int i;
    char *str;
    t_exec *cmdd;

    cmdd = commands;
    i = 0;
    
    if (exee->cmd_count > 1) {
        for (i = 0; i < exee->cmd_count - 1; i++) {
            if (!exee->pipes[i]) {
                exee->pipes[i] = malloc(sizeof(int) * 2);
                if (!exee->pipes[i]) {
                    perror("malloc failed for pipe");
                    exit(EXIT_FAILURE);
                }
            }
            if (pipe(exee->pipes[i]) < 0) {
                perror("pipe creation failed");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    cmdd = commands;
    for (i = 0; i < exee->cmd_count; i++) {
        exee->pids[i] = fork();
        if (exee->pids[i] == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        
        if (exee->pids[i] == 0)
		{ 
            if (i == 0)
			{
                if (exee->infile != STDIN_FILENO) {
                    dup2(exee->infile, STDIN_FILENO);
                }
                if (exee->cmd_count > 1)
				{
                    dup2(exee->pipes[0][1], STDOUT_FILENO);
                }
            }
            else if (i == exee->cmd_count - 1) 
			{
                dup2(exee->pipes[i - 1][0], STDIN_FILENO);
                if (exee->outfile != STDOUT_FILENO) {
                    dup2(exee->outfile, STDOUT_FILENO);
                }
            }
            else
			{
                dup2(exee->pipes[i - 1][0], STDIN_FILENO);
                dup2(exee->pipes[i][1], STDOUT_FILENO);
            }
            
            if (exee->cmd_count > 1) {
                for (int j = 0; j < exee->cmd_count - 1; j++) {
                    close(exee->pipes[j][0]);
                    close(exee->pipes[j][1]);
                }
            }
            if (exee->infile != STDIN_FILENO)
                close(exee->infile);
            if (exee->outfile != STDOUT_FILENO)
                close(exee->outfile);
            str = get_full_path_f(cmdd->cmd, env);
            if (!str) {
                fprintf(stderr, "Command not found: %s\n", cmdd->cmd);
                exit(EXIT_FAILURE);
            }
            if (execve(str, cmdd->args, env) == -1) {
                perror("execve failed");
                free(str);
                exit(EXIT_FAILURE);
            }
        }
        
        cmdd = cmdd->next;
    }
    if (exee->cmd_count > 1) {
        for (i = 0; i < exee->cmd_count - 1; i++) {
            close(exee->pipes[i][0]);
            close(exee->pipes[i][1]);
        }
    }
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

void execution(t_exec *commands, char **env)
{
    t_exee *exe;
    t_exec *cmdd;
    int infile;
    int outfile;
    int i;
    int status;

    i = 0;
    cmdd = commands;
    
    infile = open_infiles(cmdd);
    outfile = open_outfiles(cmdd);
    
    filtre_comands(&cmdd);
    // Check if commands list is still valid
    if (!cmdd)
	{
		printf("No commands to execute after filtering\n");
        if (infile > 0 && infile != STDIN_FILENO)
		close(infile);
        if (outfile > 0 && outfile != STDOUT_FILENO)
		close(outfile);
        return;
    }
    exe = init_execution(cmdd);
    exe->infile = infile;
    exe->outfile = outfile;
	
    execute_commands(exe, cmdd, env);
    for (i = 0; i < exe->cmd_count; i++) {
		waitpid(exe->pids[i], &status, 0);
    }
    
    cleanup_exe(exe);
}
