/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:15:01 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/24 15:38:51 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    t_redir *list;
    list = commands->infiles;
	while (list)
    {
        if (list->is_herdoc == 0)
		    fd = open(list->filename, O_RDONLY);
        else
            fd = list->herdoc_fd;
        list = list->next;
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

	if ((cur ->infiles || cur ->outfiles) && !(cur->cmd) && cur->next)
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
    if (exe->cmd_count > 1)
    {
        exe->pipes = malloc(sizeof(int *) * (exe->cmd_count - 1));
        if (!exe->pipes)
        {
            free(exe);
            return (NULL);
        }
        i = 0;
        while (i < exe->cmd_count - 1)
        {
            exe->pipes[i] = NULL;
            i++;
        }
    } 
    else
        exe->pipes = NULL;
    if (exe->cmd_count > 1)
    {
        exe->pids = malloc(sizeof(int) * exe->cmd_count);
        if (!exe->pids)
            return (free(exe->pipes), free(exe), NULL);
    }
    else
        exe->pids = NULL;
    i = 0;
    while (i < exe->cmd_count && exe->cmd_count > 1)
    {
        exe->pids[i] = 0;
        i++;
    }
    return exe;
}

void	freeee(char **str)
{
	int	i;

	if (!str)
		return;
	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
char	*get_full_path(char *argv, t_env **envi)
{
	// char	**parse_array;
	char	**dir;
    char    *temp = NULL;
    char    *temp2 = NULL;
	char	*path;
    char    **env;
	int		i;
	int		existence;
    env = env_list_to_array(*envi);
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	// parse_array = ft_split_exe(argv, ' '); 
    dir = ft_split_exe(env[i] + 5, ':');
	i = 0;
	while (dir[i])
	{
		temp = ft_strjoin(dir[i], "/");
		temp2 = ft_strjoin(temp, argv);
        free(dir[i]);
        dir[i] = temp2;
        free(temp);
		existence = access(dir[i], X_OK);
		if (existence == 0)
		{
			path = ft_strdup(dir[i]);
			return (freeee(dir),  path);
		}
		i++;
	}
	return (freeee(dir), freeee(env),  NULL);
}
int is_built_in(char *str)
{
    if (!strcmp(str, "echo") || !strcmp(str, "cd") || !strcmp(str, "exit") 
    || !strcmp(str, "pwd") || !strcmp(str, "export") || !strcmp(str, "env")
    || !strcmp(str, "unset"))
        return (1);
    return (0);
}
char	*get_full_path_f(char *argv, t_env **env)
{
	// char	**parse_array;
	char	*str;

	// parse_array = ft_split_exe(argv, ' ');
    if (argv && argv[0] == '\0')
        return("\0");
    if (strncmp(argv, "/", 1) == 0)
    {
        ft_putstr_fd("minishell: /: Is a directory\n", 2);
        exit(set_exit_status(126, 1337));
    }
	else if (strncmp(argv, "./", 2) == 0 || is_built_in(argv))
	{
		str = ft_strdup(argv);
		// freeee(parse_array);
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
                exit(set_exit_status(1, 1337));
            }
        }
        if (pipe(exee->pipes[i]) < 0)
        {
            perror("pipe creation failed");
            exit(set_exit_status(1, 1337));
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
            if(exee->cmd_count > 1)
                exit(set_exit_status(1, 1337));
            else 
                return;
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
            exit(set_exit_status(1, 1337));
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
            exit(set_exit_status(1, 1337));
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
            exit(set_exit_status(1, 1337));
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
            exit(set_exit_status(1, 1337));
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
            exit(set_exit_status(1, 1337));
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
int custom_execve(char *str, char **args, t_env **env, t_exee *exe)
{
    if(!ft_strcmp(args[0], "echo"))
        ft_echo(args);
    else if (!ft_strcmp(args[0], "cd"))
        cd(args[1], env);
    else if (!ft_strcmp(args[0], "pwd"))
        pwd(*env);
    else if (!ft_strcmp(args[0], "exit"))
        ft_exit(args, 0, env);
    else if (!ft_strcmp(args[0], "export"))
        ft_export(args, env);
    else if (!ft_strcmp(args[0], "env"))
        print_env(*env);
    else if (!ft_strcmp(args[0], "unset"))
        ft_unset(env, args);
    else
    {
        if (execve(str, args, env_list_to_array(*env)) == -1)
        {
            perror(str);
            free(str);
            exit(set_exit_status(1, 1337));
        }
        return(0);
    }
    if(exe->cmd_count != 1)
        return(exit(set_exit_status(0, 1337)), 0);
    return(0);
}
void execute_child_process(t_exee *exee, t_exec *cmd, int cmd_infile, int cmd_outfile, t_env **env)
{
    char *str = NULL;

    if (cmd_infile != STDIN_FILENO)
        dup2(cmd_infile, STDIN_FILENO);
    if (cmd_outfile != STDOUT_FILENO)
        dup2(cmd_outfile, STDOUT_FILENO);
    if(exee->cmd_count > 1)
    {
        close_all_pipes(exee);
        if (cmd_infile != STDIN_FILENO)
            close(cmd_infile);
        if (cmd_outfile != STDOUT_FILENO)
            close(cmd_outfile);
    }
    if (cmd->var_in_quotes == 0 && cmd->expanded_flag == 1)
    {
        char **splitted = ft_split_exe(cmd->cmd, ' ');
        if (!splitted)
            return;
        free(cmd->cmd);
        cmd->cmd = ft_strdup(splitted[0]);
        char **new_args = renew_args(splitted, cmd->args);
        cmd->args = new_args;
    }
    str = get_full_path_f(cmd->cmd, env);
    if (!str || (str && str[0] == '\0'))
    { 
        if (str && str[0] == '\0')
            write(2, "Command '' not found\n", 22);
        else
            fprintf(stderr, "%s: Command not found\n", cmd->cmd);
        exit(set_exit_status(127, 1337));
    }
    custom_execve(str, cmd->args, env, exee);
}

void handle_single_io(t_exee *exee, t_exec *cmd, int *in, int *out)
{
    setup_command_io(exee, cmd, 0, in, out);
}

void handle_single_command(t_exee *exee, t_exec *cmd, t_env **env)
{
    int status;
    int in = STDIN_FILENO, out = STDOUT_FILENO;
    handle_single_io(exee, cmd, &in, &out);
    
    if (is_built_in(cmd->cmd))
    {
        int std_in = dup(STDIN_FILENO), std_out = dup(STDOUT_FILENO);
        if (in != STDIN_FILENO) dup2(in, STDIN_FILENO);
        if (out != STDOUT_FILENO) dup2(out, STDOUT_FILENO);
        
        char *path = get_full_path_f(cmd->cmd, env);
        if (path)
        {
            custom_execve(path, cmd->args, env, exee);
            free(path);
        }
        dup2(std_in, STDIN_FILENO); dup2(std_out, STDOUT_FILENO);
        close(std_in); close(std_out);
    }
    else
    {
        pid_t pid = fork();
        if (pid == 0) 
            execute_child_process(exee, cmd, in, out, env);
        else if (pid > 0)
        {
            if (!exee->pids)
                exee->pids = malloc(sizeof(pid_t));
            if (exee->pids)
                exee->pids[0] = pid;
            waitpid(pid, &status, 0);
            set_exit_status(WEXITSTATUS(status), 1);
        }
    }
}

void handle_pipeline(t_exee *exee, t_exec *cmds, t_env **env)
{
    t_exec *cmd = cmds;
    int i = 0, in, out;
    
    setup_pipes(exee);
    while (i < exee->cmd_count)
    {
        in = STDIN_FILENO;
        out = STDOUT_FILENO;
        if ((exee->pids[i] = fork()) == 0)
        {
            setup_command_io(exee, cmd, i, &in, &out);
            execute_child_process(exee, cmd, in, out, env);
        }
        cmd = cmd->next; i++;
    }
    close_all_pipes(exee);
}

void execute_commands(t_exee *exee, t_exec *cmds, t_env **env)
{
    if (exee->cmd_count == 1)
        handle_single_command(exee, cmds, env);
    else
        handle_pipeline(exee, cmds, env);
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

void execution(t_exec *commands, t_env **envi)
{
    t_exee *exe;
    t_exec *cmdd;
    int i;
    int status;

    i = 0;
    if(commands && !commands->cmd)
    {
        open_infiles(commands);
        open_outfiles(commands);
        return;
    }
    // env = env_list_to_array(envi);
    i = 0;
    cmdd = commands;
    exe = init_execution(cmdd);
    execute_commands(exe, cmdd, envi);
    if (exe->cmd_count == 1 && is_built_in(commands->cmd))
    {
        cleanup_exe(exe);
        return;
    }
    i = 0;
    while (waitpid(-1, &status, 0) != -1)
    {
        if (WIFEXITED(status))
            set_exit_status(WEXITSTATUS(status), 0);
        if (WIFSIGNALED(status))
            set_exit_status(WTERMSIG(status) + 128, 0);
    }
    cleanup_exe(exe);
}
