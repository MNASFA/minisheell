/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:15:01 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/11 23:37:45 by aboukhmi         ###   ########.fr       */
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
		lst = lst->next;
	}
	return (i);
}

static int	ft_lstsize_commands(t_exec **lst)
{
	int	i;
	t_exec *current;

	if (!lst || !(*lst))
		return (0);
	i = 0;
	current = *lst;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

int open_in(char *filename, int *i)
{
    int k;

    if (!filename)
        return (-3);
    if (*i > 2)
        close(*i);
    k = open(filename, O_RDONLY);
    if (k == -1)
    {
        set_exit_status(1, 1337);
        return (-3);
    }
    return (k);
}

int open_out(char *filename, int *i, t_redir *red)
{
    int k;

    if (!filename || !red)
        return (-3);
    if (*i > 2)
        close(*i);
    if (red->type == REDIR_OUT)
        k = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if (red->type == APPEND)
        k = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644); 
    else
        return (-3);
    if (k == -1)
    {
        set_exit_status(1, 1337);
        return (-3);
    }
    return (k);
}

int open_out_her(t_redir *red, int *i)
{
    int k;

    if (!red)
        return (-3);
    if (*i == -2)
        k = red->herdoc_fd;
    else
    {
        if (*i > 2)
            close(*i);
        k = red->herdoc_fd;
    }
    if (k == -1)
    {
        set_exit_status(1, 1337);
        return (-3);
    }
    return (k);
}

static int	handle_input_redir(t_redir *red, t_exec **commands)
{
	int	i;

	i = open_in(red->filename, &(*commands)->infd);
	if (i == -3)
	{
		perror(red->filename);
		return (-1);
	}
	if (isatty(i))
	{
		close(i);
		(*commands)->infd = -2;
	}
	else
		(*commands)->infd = i;
	return (0);
}

static int	handle_output_redir(t_redir *red, t_exec **commands)
{
	int	i;

	i = open_out(red->filename, &(*commands)->outfd, red);
	if (i == -3)
	{
		perror(red->filename);
		return (-1);
	}
	if (isatty(i))
	{
        close(i);
        (*commands)->outfd = -2;
	}
	else
		(*commands)->outfd = i;
	return (0);
}

static int	handle_heredoc_redir(t_redir *red, t_exec **commands)
{
	int	i;

	i = open_out_her(red, &(*commands)->infd);
	if (i == -3)
	{
		perror("heredoc");
		return (-1);
	}
	(*commands)->infd = i;
	return (0);
}

int	open_in_out(t_exec **commands)
{
	t_redir	*red;

	if (!commands || !(*commands))
		return (-1);
	red = (*commands)->redirections;
	while (red)
	{
		if (red->type == REDIR_IN)
		{
			if (handle_input_redir(red, commands) == -1)
				return (-1);
		}
		else if (red->type == REDIR_OUT || red->type == APPEND)
		{
			if (handle_output_redir(red, commands) == -1)
				return (-1);
		}
		else if (red->type == HEREDOC)
		{
			if (handle_heredoc_redir(red, commands) == -1)
				return (-1);
		}
		red = red->next;
	}
	return (0);
}

void close_all_pipes(t_exee *exee)
{
    int i;
    
    if (!exee || !exee->pipes || exee->cmd_count <= 1)
        return;
    i = 0;
    while (i < exee->cmd_count - 1)
    {
        if (exee->pipes[i])
        {
            close(exee->pipes[i][0]);
            close(exee->pipes[i][1]);
        }
        i++;
    }
}

char **env_list_to_array(t_env *env)
{
    char **enve;
    int i;
    
    i = 0;
    enve = (char **)malloc(sizeof(char *) * (ft_lstsize(env) + 1));
    if (!enve)
        return (NULL);
    while (env)
    {
        enve[i] = env->full;
        i++;
        env = env->next;
    }
    enve[i] = NULL;
    return (enve);
}

static int	init_pipes_and_pids(t_exee *exe)
{
	int	i;

	exe->pipes = malloc(sizeof(int *) * (exe->cmd_count - 1));
	if (!exe->pipes)
		return (-1);
	i = 0;
	while (i < exe->cmd_count - 1)
	{
		exe->pipes[i] = NULL;
		i++;
	}
	exe->pids = malloc(sizeof(int) * exe->cmd_count);
	if (!exe->pids)
	{
		free(exe->pipes);
		return (-1);
	}
	i = 0;
	while (i < exe->cmd_count)
	{
		exe->pids[i] = 0;
		i++;
	}
	return (0);
}

static t_exee	*cleanup_and_return_null(t_exee *exe)
{
	if (exe->infile != -1)
		close(exe->infile);
	if (exe->outfile != -1)
		close(exe->outfile);
	free(exe);
	return (NULL);
}

static t_exee	*init_basic_fields(t_exec **commands)
{
	t_exee	*exe;

	exe = malloc(sizeof(t_exee));
	if (!exe)
		return (NULL);
	exe->cmd_count = ft_lstsize_commands(commands);
	exe->infile = dup(STDIN_FILENO);
	exe->outfile = dup(STDOUT_FILENO);
	exe->cd_in = -2;
	exe->cd_out = -2;
	exe->saved_in = -1;
	exe->saved_out = -2;
	exe->pipes = NULL;
	exe->pids = NULL;
	if (exe->infile == -1 || exe->outfile == -1)
		return (cleanup_and_return_null(exe));
	return (exe);
}

t_exee	*init_execution(t_exec **commands)
{
	t_exee	*exe;

	exe = init_basic_fields(commands);
	if (!exe)
		return (NULL);
	if (exe->cmd_count > 1)
	{
		if (init_pipes_and_pids(exe) == -1)
			return (cleanup_and_return_null(exe));
	}
	return (exe);
}

int is_built_in(char *str)
{
    if (!str)
        return (0);
    if (!strcmp(str, "echo") || !strcmp(str, "cd") || !strcmp(str, "exit") 
        || !strcmp(str, "pwd") || !strcmp(str, "export") || !strcmp(str, "env")
        || !strcmp(str, "unset"))
        return (1);
    return (0);
}

void setup_pipes(t_exee *exee)
{
    int i = 0;
    
    if (!exee || exee->cmd_count <= 1)
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

void setup_first_command_io(t_exee **exee, t_exec **cmd)
{
    if (!cmd || !(*cmd) || !(*exee))
        return;
        
    if ((*cmd)->redirections)
    {
        if (open_in_out(cmd) == -1)
        {
            (*exee)->cd_in = -1;
            (*exee)->cd_out = -1;
            return;
        }
        if ((*cmd)->infd != -2)
            (*exee)->cd_in = (*cmd)->infd;
        if ((*cmd)->outfd != -2)
            (*exee)->cd_out = (*cmd)->outfd;
    }
    if ((*cmd)->infd == -2)
        (*exee)->cd_in = (*exee)->infile;
    if ((*exee)->cmd_count > 1 && (*cmd)->outfd == -2)
        (*exee)->cd_out = (*exee)->pipes[0][1];
    else if ((*cmd)->outfd == -2)
        (*exee)->cd_out = (*exee)->outfile;
}
    
int is_there_any_in(t_redir *red)
{
    while (red)
    {
        if (red->type == REDIR_IN || red->type == HEREDOC)
            return (1);
        red = red->next;
    }
    return (0);
}

int is_there_any_out(t_redir *red)
{
    while (red)
    {
        if (red->type == REDIR_OUT || red->type == APPEND)
            return (1);
        red = red->next;
    }
    return (0);
}

void safe_close(int *fd)
{
    if (*fd > 2)
    {
        close(*fd);
        *fd = -2;
    }
}

void setup_middle_command_io(t_exee **exee, t_exec **cmd, int cmd_index)
{
    if (!cmd || !(*cmd) || !(*exee))
        return;
    if ((*cmd)->redirections)
    {
        if (open_in_out(cmd) == -1)
        {
            (*exee)->cd_in = -1;
            (*exee)->cd_out = -1;
            return;
        }
        if (is_there_any_in((*cmd)->redirections))
        {
            safe_close(&(*exee)->pipes[cmd_index - 1][0]);
            (*exee)->cd_in = (*cmd)->infd;
        }
        if (is_there_any_out((*cmd)->redirections))
        {
            safe_close(&(*exee)->pipes[cmd_index][1]);
            (*exee)->cd_out = (*cmd)->outfd;
        }
    }
    if (!is_there_any_in((*cmd)->redirections))
        (*exee)->cd_in = (*exee)->pipes[cmd_index - 1][0];
    if (!is_there_any_out((*cmd)->redirections))
        (*exee)->cd_out = (*exee)->pipes[cmd_index][1];
}

static void	handle_redirections(t_exee **exee, t_exec **cmd, int cmd_index)
{
	if (open_in_out(cmd) == -1)
	{
		(*exee)->cd_in = -1;
		(*exee)->cd_out = -1;
		return ;
	}
	if (is_there_any_in((*cmd)->redirections))
	{
		close((*exee)->pipes[cmd_index - 1][0]);
		(*exee)->cd_in = (*cmd)->infd;
	}
	if (is_there_any_out((*cmd)->redirections))
	{
		(*exee)->cd_out = (*cmd)->outfd;
		if ((*exee)->cd_out == -2)
			(*exee)->cd_out = 1;
	}
}

static void	setup_default_io(t_exee **exee, t_exec **cmd, int cmd_index)
{
	if (!is_there_any_in((*cmd)->redirections))
		(*exee)->cd_in = (*exee)->pipes[cmd_index - 1][0];
	if (!is_there_any_out((*cmd)->redirections))
		(*exee)->cd_out = (*exee)->outfile;
}

void	setup_last_command_io(t_exee **exee, t_exec **cmd, int cmd_index)
{
	if (!cmd || !(*cmd) || !(*exee))
		return ;
	if ((*cmd)->redirections)
		handle_redirections(exee, cmd, cmd_index);
	setup_default_io(exee, cmd, cmd_index);
}

void setup_command_io(t_exee **exee, t_exec **cmd, int cmd_index)
{
    if (cmd_index == 0)
        setup_first_command_io(exee, cmd);
    else if (cmd_index == (*exee)->cmd_count - 1)
        setup_last_command_io(exee, cmd, cmd_index);
    else
        setup_middle_command_io(exee, cmd, cmd_index);
}

static int	execute_builtin(t_exec **cmd, t_env **env, t_exee **exe)
{
	if (!ft_strcmp((*cmd)->args[0], "echo"))
		ft_echo((*cmd)->args);
	else if (!ft_strcmp((*cmd)->args[0], "cd"))
		cd((*cmd)->args, env);
	else if (!ft_strcmp((*cmd)->args[0], "pwd"))
		pwd(*env);
	else if (!ft_strcmp((*cmd)->args[0], "exit"))
		ft_exit(cmd, 0, env, exe);
	else if (!ft_strcmp((*cmd)->args[0], "export"))
		ft_export((*cmd)->args, env);
	else if (!ft_strcmp((*cmd)->args[0], "env"))
		print_env((*cmd)->args, *env);
	else if (!ft_strcmp((*cmd)->args[0], "unset"))
		ft_unset(env, (*cmd)->args);
	else
		return (0);
	return (1);
}

static int	execute_external(char *str, t_exec **cmd, t_env **env, t_exee **exe)
{
	char	**env_array;

	env_array = env_list_to_array(*env);
	if (!env_array)
		exit(set_exit_status(1, 1337));
	if (execve(str, (*cmd)->args, env_array) == -1)
	{
		perror(str);
		free(env_array);
		cleanup_exe(*exe);
		exit(set_exit_status(127, 1337));
	}
	free(env_array);
	return (0);
}

int	custom_execve(char *str, t_exec **cmd, t_env **env, t_exee **exe)
{
	if ((cmd && !(*cmd)->args) || !(*cmd)->args || !(*cmd)->args[0])
		return (-1);
	if (execute_builtin(cmd, env, exe))
	{
        safe_close(&(*exe)->infile);
        safe_close(&(*exe)->outfile);
		if ((*exe)->cmd_count != 1)
			exit(set_exit_status(0, 1337));
		return (0);
	}
	return (execute_external(str, cmd, env, exe));
}

static void	setup_redirections(t_exee **exee)
{
	if ((*exee)->cd_in != STDIN_FILENO)
	{
		if (dup2((*exee)->cd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 input");
			exit(set_exit_status(1, 1337));
		}
	}
	if ((*exee)->cd_out != STDOUT_FILENO)
	{
		if (dup2((*exee)->cd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 output");
			exit(set_exit_status(1, 1337));
		}
	}
}

static void	cleanup_pipe_fds(t_exee **exee)
{
	if ((*exee)->cmd_count > 1)
	{
		close_all_pipes(*exee);
		if ((*exee)->cd_in > 2)
			close((*exee)->cd_in);
		if ((*exee)->cd_out > 2)
			close((*exee)->cd_out);
	}
}

static void	handle_command_not_found(char *str, t_exec **cmd, t_exee **exee)
{
	if (str && str[0] == '\0')
	{
		write(2, "Command '' not found\n", 22);
		free(str);
	}
	else
    {
        write (2, (*cmd)->cmd, ft_strlen((*cmd)->cmd));
        write(2, ": Command not found\n", 20);
    }
	cleanup_exe(*exee);
	exit(set_exit_status(127, 1337));
}

static void	execute_external_command(char **str, t_exec **cmd, t_env **env,
			t_exee **exee)
{
	*str = get_full_path_f((*cmd)->cmd, env);
	if (!*str || (*str && (*str)[0] == '\0'))
		handle_command_not_found(*str, cmd, exee);
	custom_execve(*str, cmd, env, exee);
}

void	execute_child_process(t_exee **exee, t_exec **cmd, t_env **env)
{
	char	*str;

	str = NULL;
	if (!cmd || !(*cmd))
		exit(set_exit_status(1, 1337));
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!(*cmd)->cmd)
		exit(set_exit_status(0, 1337));
	setup_redirections(exee);
	(*cmd)->outfd = (*exee)->cd_out;
	(*cmd)->infd = (*exee)->cd_in;
	cleanup_pipe_fds(exee);
	if (is_built_in((*cmd)->args[0]))
		custom_execve(str, cmd, env, exee);
	else
		execute_external_command(&str, cmd, env, exee);
	free(str);
}

void handle_single_io(t_exee **exee, t_exec **cmd)
{
    setup_command_io(exee, cmd, 0);
}

static void	handle_builtin_command(t_exee **exee, t_exec **cmd, t_env **env)
{
	(*exee)->saved_in = dup(STDIN_FILENO);
	(*exee)->saved_out = dup(STDOUT_FILENO);
	if ((*exee)->cd_in != STDIN_FILENO)
		dup2((*exee)->cd_in, STDIN_FILENO);
	if ((*exee)->cd_out != STDOUT_FILENO)
		dup2((*exee)->cd_out, STDOUT_FILENO);
	custom_execve(NULL, cmd, env, exee);
	dup2((*exee)->saved_in, STDIN_FILENO);
	dup2((*exee)->saved_out, STDOUT_FILENO);
	close((*exee)->saved_in);
	close((*exee)->saved_out);
	if ((*exee)->cd_in > 2)
		close((*exee)->cd_in);
	if ((*exee)->cd_out > 2)
		close((*exee)->cd_out);
}

static void	handle_fork_error(t_exee **exee)
{
	perror("fork");
	if ((*exee)->cd_in > 2)
		close((*exee)->cd_in);
	if ((*exee)->cd_out > 2)
		close((*exee)->cd_out);
	set_exit_status(1, 1337);
}

static void	handle_parent_process(t_exee **exee, pid_t pid)
{
	int	status;

	if ((*exee)->cd_in > 2)
		close((*exee)->cd_in);
	if ((*exee)->cd_out > 2)
		close((*exee)->cd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status), 0);
	if (WIFSIGNALED(status))
		set_exit_status(WTERMSIG(status) + 128, 0);
}

static void	handle_external_command(t_exee **exee, t_exec **cmd, t_env **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_child_process(exee, cmd, env);
	else if (pid > 0)
		handle_parent_process(exee, pid);
	else
		handle_fork_error(exee);
}

int	handle_single_command(t_exee **exee, t_exec **cmd, t_env **env)
{
	(*exee)->cd_in = STDIN_FILENO;
	(*exee)->cd_in = STDOUT_FILENO;
	if (!cmd || !(*cmd))
		return (0);
	handle_single_io(exee, cmd);
	if ((*cmd) && !(*cmd)->cmd)
	{
		if ((*cmd)->redirections)
		{
			if (open_in_out(cmd) == -1)
				return(set_exit_status(1, 1337));
		}
		return (0);
	}
	if ((*exee)->cd_in == -1 || (*exee)->cd_out == -1)
	    return(set_exit_status(1, 1337));
	(*env)->fd_in = (*exee)->cd_in;
	(*env)->fd_out = (*exee)->cd_out;
	if (is_built_in((*cmd)->cmd))
		handle_builtin_command(exee, cmd, env);
	else
		handle_external_command(exee, cmd, env);
    return(0);
}

static void	handle_io_error(t_exee **exee, int i)
{
	(*exee)->pids[i] = 0;
	set_exit_status(1, 1337);
}

static void	execute_command_in_pipeline(t_exee **exee, t_exec *cmd,
			t_exec *temp_cmd, t_env **env, int i)
{
	if (cmd->cmd && ((*exee)->pids[i] = fork()) == 0)
	{
		signal(SIGINT, SIG_DFL);
		execute_child_process(exee, &temp_cmd, env);
	}
	else if (!cmd->cmd)
		(*exee)->pids[i] = 0;
}

static void	cleanup_command_fds(t_exec *cmd)
{
	if (cmd->infd > 2)
	{
		close(cmd->infd);
		cmd->infd = -1;
	}
	if (cmd->outfd > 2)
	{
		close(cmd->outfd);
		cmd->outfd = -1;
	}
}

static void	process_single_command(t_exee **exee, t_exec **cmd, t_env **env,
			int i)
{
	t_exec	*temp_cmd;

	(*exee)->cd_in = STDIN_FILENO;
	(*exee)->cd_out = STDOUT_FILENO;
	temp_cmd = *cmd;
	setup_command_io(exee, &temp_cmd, i);
	if ((*exee)->cd_in == -1 || (*exee)->cd_out == -1)
		handle_io_error(exee, i);
	else
		execute_command_in_pipeline(exee, *cmd, temp_cmd, env, i);
	cleanup_command_fds(*cmd);
}

void	handle_pipeline(t_exee **exee, t_exec **cmds, t_env **env)
{
	t_exec	*cmd;
	int		i;

	if (!cmds || !(*cmds))
		return ;
	setup_pipes(*exee);
	cmd = *cmds;
	i = 0;
	while (i < (*exee)->cmd_count && cmd)
	{
		process_single_command(exee, &cmd, env, i);
		cmd = cmd->next;
		i++;
	}
	close_all_pipes((*exee));
}

void execute_commands(t_exee **exee, t_exec **cmds, t_env **env)
{
    if (!exee || !cmds)
        return;
    if ((*exee)->cmd_count == 1)
        handle_single_command(exee, cmds, env);
    else
        handle_pipeline(exee, cmds, env);
        
    if ((*exee)->infile > 2)
        close((*exee)->infile);
    if ((*exee)->outfile > 2)
        close((*exee)->outfile);
}

void cleanup_exe(t_exee *exe)
{
    int i;
 
    if (!exe)
        return; 
    if (exe->pipes && exe->cmd_count > 1)
    {
        i = 0;
        while (i < exe->cmd_count - 1)
        {
            if (exe->pipes[i])
                free(exe->pipes[i]);
            i++;
        }
        free(exe->pipes);
    }
    if (exe->pids)
        free(exe->pids);  
    if (exe->infile > 2)
        close(exe->infile);
    if (exe->outfile > 2)
        close(exe->outfile); 
    free(exe);
}

void closeallfiles(t_exec **commands)
{
    t_exec *command;

    if (!commands || !(*commands))
        return;
    command = *commands;
    while (command)
    {
        if (command->outfd > 2)
        {
            close(command->outfd);
            command->outfd = -2;
        }
        if (command->infd > 2)
        {
            close(command->infd);
            command->infd = -2;
        }
        command = command->next;
    }    
}

static void	wait_for_all_children(void)
{
	int	status;

	while (waitpid(-1, &status, 0) != -1)
	{
		if (WIFEXITED(status))
			set_exit_status(WEXITSTATUS(status), 0);
		if (WIFSIGNALED(status))
			set_exit_status(WTERMSIG(status) + 128, 0);
	}
}

static void	cleanup_env_fds(t_env **envi)
{
	if ((*envi)->fd_in > 2)
	{
		close((*envi)->fd_in);
		(*envi)->fd_in = -1;
	}
	if ((*envi)->fd_out > 2)
	{
		close((*envi)->fd_out);
		(*envi)->fd_out = -1;
	}
}

static void	handle_pipeline_cleanup(t_exee *exe, t_exec **commands,
			t_env **envi)
{
	wait_for_all_children();
	cleanup_env_fds(envi);
	cleanup_exe(exe);
	closeallfiles(commands);
}

void	execution(t_exec **commands, t_env **envi)
{
	t_exee	*exe;

	if (!commands || !(*commands) || !envi || !(*envi))
		return ;
	exe = init_execution(commands);
	if (!exe)
	{
		set_exit_status(1, 1337);
		return ;
	}
	execute_commands(&exe, commands, envi);
	if (exe->cmd_count == 1 && (*commands)->cmd
		&& is_built_in((*commands)->cmd))
		cleanup_exe(exe);
	else
		handle_pipeline_cleanup(exe, commands, envi);
}
