/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:15:01 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/10 19:44:00 by hmnasfa          ###   ########.fr       */
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
    if (*i == -2)
        k = open(filename, O_RDONLY);
    else
    {
        if (*i > 2)
            close(*i);
        k = open(filename, O_RDONLY);
    }
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
    k = -1;
    if (*i == -2)
    {
        if (red->type == REDIR_OUT)
            k = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else if (red->type == APPEND)
            k = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644); 
    }
    else
    {
        if (*i > 2)
            close(*i);
        if (red->type == REDIR_OUT)
            k = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else if (red->type == APPEND)
            k = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644); 
    }
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

int open_in_out(t_exec **commands)
{
    t_redir *red;
    int i;

    if (!commands || !(*commands))
        return (-1);

    red = (*commands)->redirections;
    while (red)
    {
        if (red->type == REDIR_IN)
        {
            i = open_in(red->filename, &(*commands)->infd);
            if (i == -3)
            {
                perror(red->filename);
                return (-1);
            }
            (*commands)->infd = i;
        }
        else if (red->type == REDIR_OUT || red->type == APPEND)
        {
            i = open_out(red->filename, &(*commands)->outfd, red);
            if (i == -3)
            {
                perror(red->filename);
                return (-1);
            }
            if (!isatty(i))
                (*commands)->outfd = i;
        }
        else if (red->type == HEREDOC)
        {
            i = open_out_her(red, &(*commands)->infd);
            if (i == -3)
            {
                perror("heredoc");
                return (-1);
            }
            (*commands)->infd = i;
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

t_exee *init_execution(t_exec **commands)
{
    t_exee *exe;
    int i;
    
    exe = malloc(sizeof(t_exee));
    if (!exe)
        return (NULL);
    exe->cmd_count = ft_lstsize_commands(commands);
    exe->infile = dup(STDIN_FILENO);
    exe->outfile = dup(STDOUT_FILENO);
    
    if (exe->infile == -1 || exe->outfile == -1)
    {
        free(exe);
        return (NULL);
    }
    
    if (exe->cmd_count > 1)
    {
        exe->pipes = malloc(sizeof(int *) * (exe->cmd_count - 1));
        if (!exe->pipes)
        {
            close(exe->infile);
            close(exe->outfile);
            free(exe);
            return (NULL);
        }
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
            close(exe->infile);
            close(exe->outfile);
            free(exe);
            return (NULL);
        }
        i = 0;
        while (i < exe->cmd_count)
        {
            exe->pids[i] = 0;
            i++;
        }
    }
    else
    {
        exe->pipes = NULL;
        exe->pids = NULL;
    }
    return (exe);
}

char *get_full_path(char *argv, t_env **envi)
{
    char **dir;
    char *temp = NULL;
    char *temp2 = NULL;
    char *path;
    char **env;
    int i;
    int existence;
    
    if (!argv || !envi || !(*envi))
        return (NULL);
    
    env = env_list_to_array(*envi);
    if (!env)
        return (NULL);
    
    i = 0;
    while (env[i] && ft_strncmp(env[i], "PATH=", 5))
        i++;
    
    if (!env[i])
    {
        free(env);
        return (NULL);
    }
    
    dir = ft_split_exe(env[i] + 5, ':');
    free(env);
    if (!dir)
        return (NULL);
    
    i = 0;
    while (dir[i])
    {
        temp = ft_strjoin(dir[i], "/");
        if (!temp)
        {
            freeee(dir);
            return (NULL);
        }
        temp2 = ft_strjoin(temp, argv);
        free(temp);
        if (!temp2)
        {
            freeee(dir);
            return (NULL);
        }
        free(dir[i]);
        dir[i] = temp2;
        existence = access(dir[i], X_OK);
        if (existence == 0)
        {
            path = ft_strdup(dir[i]);
            freeee(dir);
            return (path);
        }
        i++;
    }
    freeee(dir);
    return (NULL);
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

char *get_full_path_f(char *argv, t_env **env)
{
    if (!argv)
        return (NULL);
    if (argv && argv[0] == '\0')
        return (ft_strdup(""));
    if (strncmp(argv, "/", 1) == 0)
    {
        ft_putstr_fd("minishell: /: Is a directory\n", 2);
        exit(set_exit_status(126, 1337));
    }
    if (strchr(argv, '/'))
    {
        if (access(argv, X_OK) == 0)
            return (ft_strdup(argv));
        else
            return (NULL);
    }
    else
        return (get_full_path(argv, env));
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

void setup_first_command_io(t_exee *exee, t_exec **cmd, int *cmd_infile, int *cmd_outfile)
{
    if (!cmd || !(*cmd) || !exee)
        return;
        
    if ((*cmd)->redirections)
    {
        if (open_in_out(cmd) == -1)
        {
            *cmd_infile = -1;
            *cmd_outfile = -1;
            return;
        }
        if ((*cmd)->infd != -2)
            *cmd_infile = (*cmd)->infd;
        // if (!isatty((*cmd)->outfd))
        //     *cmd_outfile = -1;
        else if ((*cmd)->outfd != -2)
            *cmd_outfile = (*cmd)->outfd;
    }
    if ((*cmd)->infd == -2)
        *cmd_infile = exee->infile;
    if (exee->cmd_count > 1 && (*cmd)->outfd == -2)
        *cmd_outfile = exee->pipes[0][1];
    else if ((*cmd)->outfd == -2)
        *cmd_outfile = exee->outfile;
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

void setup_middle_command_io(t_exee *exee, t_exec **cmd, int cmd_index, int *cmd_infile, int *cmd_outfile)
{
    if (!cmd || !(*cmd) || !exee)
        return;
        
    if ((*cmd)->redirections)
    {
        if (open_in_out(cmd) == -1)
        {
            *cmd_infile = -1;
            *cmd_outfile = -1;
            return;
        }
        if (is_there_any_in((*cmd)->redirections))
        {
            close(exee->pipes[cmd_index - 1][0]);
            *cmd_infile = (*cmd)->infd;
        }
        if (is_there_any_out((*cmd)->redirections))
        {
            close(exee->pipes[cmd_index][1]);
            *cmd_outfile = (*cmd)->outfd;
        }
    }
    if (!is_there_any_in((*cmd)->redirections))
        *cmd_infile = exee->pipes[cmd_index - 1][0];
    if (!is_there_any_out((*cmd)->redirections))
        *cmd_outfile = exee->pipes[cmd_index][1];
}

void setup_last_command_io(t_exee *exee, t_exec **cmd, int cmd_index, int *cmd_infile, int *cmd_outfile)
{
    if (!cmd || !(*cmd) || !exee)
        return;
        
    if ((*cmd)->redirections)
    {
        if (open_in_out(cmd) == -1)
        {
            *cmd_infile = -1;
            *cmd_outfile = -1;
            return;
        }
        if (is_there_any_in((*cmd)->redirections))
        {
            close(exee->pipes[cmd_index - 1][0]);
            *cmd_infile = (*cmd)->infd;
        }
        if (is_there_any_out((*cmd)->redirections))
        {
            *cmd_outfile = (*cmd)->outfd;
            if (*cmd_outfile == -2)
                *cmd_outfile = 1;
        }
    }
    if (!is_there_any_in((*cmd)->redirections))
        *cmd_infile = exee->pipes[cmd_index - 1][0];
    if (!is_there_any_out((*cmd)->redirections))
        *cmd_outfile = exee->outfile;
}

void setup_command_io(t_exee *exee, t_exec **cmd, int cmd_index, int *cmd_infile, int *cmd_outfile)
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
    if (!args || !args[0])
        return (-1);
    if (!ft_strcmp(args[0], "echo"))
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
        char **env_array = env_list_to_array(*env);
        if (!env_array)
            exit(set_exit_status(1, 1337));
        if (execve(str, args, env_array) == -1)
        {
            perror(str);
            free(env_array);
            exit(set_exit_status(127, 1337));
        }
        free(env_array);
        return (0);
    }
    if (exe->cmd_count != 1)
        exit(set_exit_status(0, 1337));
    return (0);
}

void execute_child_process(t_exee *exee, t_exec **cmd, int cmd_infile, int cmd_outfile, t_env **env)
{
    char *str = NULL;

    if (!cmd || !(*cmd))
        exit(set_exit_status(1, 1337));
        
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if (!(*cmd)->cmd)
        exit(set_exit_status(0, 1337));
        
    if (cmd_infile != STDIN_FILENO)
    {
        if (dup2(cmd_infile, STDIN_FILENO) == -1)
        {
            perror("dup2 input");
            exit(set_exit_status(1, 1337));
        }
    }
    if (cmd_outfile != STDOUT_FILENO)
    {
        if (dup2(cmd_outfile, STDOUT_FILENO) == -1)
        {
            perror("dup2 output");
            exit(set_exit_status(1, 1337));
        }
    }
    (*cmd)->outfd = cmd_outfile;
    (*cmd)->infd = cmd_infile;
    if (exee->cmd_count > 1)
    {
        close_all_pipes(exee);
        if (cmd_infile > 2)
            close(cmd_infile);
        if (cmd_outfile > 2)
            close(cmd_outfile);
    }
    
    if (is_built_in((*cmd)->args[0]))
        custom_execve(str, (*cmd)->args, env, exee);
    else
    {
        str = get_full_path_f((*cmd)->cmd, env);
        if (!str || (str && str[0] == '\0'))
        { 
            if (str && str[0] == '\0')
            {
                write(2, "Command '' not found\n", 22);
                free(str);
            }
            else
                fprintf(stderr, "%s: Command not found\n", (*cmd)->cmd);
            exit(set_exit_status(127, 1337));
        }
        custom_execve(str, (*cmd)->args, env, exee);
    }
    free(str);
}

void handle_single_io(t_exee *exee, t_exec **cmd, int *in, int *out)
{
    setup_command_io(exee, cmd, 0, in, out);
}

void handle_single_command(t_exee *exee, t_exec **cmd, t_env **env)
{
    int in = STDIN_FILENO;
    int out = STDOUT_FILENO;
    pid_t pid;
    int status;
    
    if (!cmd || !(*cmd))
        return;
    handle_single_io(exee, cmd, &in, &out);
    if ((*cmd) && !(*cmd)->cmd)
    {
        if ((*cmd)->redirections)
        {
            if (open_in_out(cmd) == -1)
            {
                set_exit_status(1, 1337);
                return;
            }
        }
        return;
    }
    if (in == -1 || out == -1)
    {
        set_exit_status(1, 1337);
        return;
    }
    (*env)->fd_in = in;
    (*env)->fd_out = out;
    if (is_built_in((*cmd)->cmd))
    {
        int saved_in = dup(STDIN_FILENO);
        int saved_out = dup(STDOUT_FILENO);
        if (in != STDIN_FILENO)
            dup2(in, STDIN_FILENO);
        if (out != STDOUT_FILENO)
            dup2(out, STDOUT_FILENO);
        custom_execve(NULL, (*cmd)->args, env, exee);
        dup2(saved_in, STDIN_FILENO);
        dup2(saved_out, STDOUT_FILENO);
        close(saved_in);
        close(saved_out);
        if (in > 2)
            close(in);
        if (out > 2)
            close(out);
    }
    else
    {
        pid = fork();
        if (pid == 0) 
            execute_child_process(exee, cmd, in, out, env);
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                set_exit_status(WEXITSTATUS(status), 0);
            if (WIFSIGNALED(status))
                set_exit_status(WTERMSIG(status) + 128, 0);
        }
        else
        {
            perror("fork");
            set_exit_status(1, 1337);
        }
    }
}

void handle_pipeline(t_exee *exee, t_exec **cmds, t_env **env)
{
    t_exec *cmd;
    t_exec *temp_cmd;
    int i = 0, in, out;
    
    if (!cmds || !(*cmds))
        return;
    
    setup_pipes(exee);
    cmd = *cmds;
    
    while (i < exee->cmd_count && cmd)
    {
        in = STDIN_FILENO;
        out = STDOUT_FILENO;
        temp_cmd = cmd;
        setup_command_io(exee, &temp_cmd, i, &in, &out);
        
        if (in == -1 || out == -1)
        {
            exee->pids[i] = 0;
            set_exit_status(1, 1337);
        }
        else if (cmd->cmd && (exee->pids[i] = fork()) == 0)
        {
            signal(SIGINT, SIG_DFL);
            execute_child_process(exee, &temp_cmd, in, out, env);
        }
        else if (!cmd->cmd)
            exee->pids[i] = 0;
        cmd = cmd->next;
        i++;
    }
    close_all_pipes(exee);
}

void execute_commands(t_exee *exee, t_exec **cmds, t_env **env)
{
    if (!exee || !cmds)
        return;
    if (exee->cmd_count == 1)
        handle_single_command(exee, cmds, env);
    else
        handle_pipeline(exee, cmds, env);
        
    if (exee->infile > 2)
        close(exee->infile);
    if (exee->outfile > 2)
        close(exee->outfile);
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

void execution(t_exec **commands, t_env **envi)
{
    t_exee *exe;
    int status;

    if (!commands || !(*commands) || !envi || !(*envi))
        return;
    exe = init_execution(commands);
    if (!exe)
    {
        set_exit_status(1, 1337);
        return;
    }
    execute_commands(exe, commands, envi);
    if (exe->cmd_count == 1 && (*commands)->cmd && is_built_in((*commands)->cmd))
    {
        cleanup_exe(exe);
        return;
    }
    while (waitpid(-1, &status, 0) != -1)
    {
        if (WIFEXITED(status))
            set_exit_status(WEXITSTATUS(status), 0);
        if (WIFSIGNALED(status))
            set_exit_status(WTERMSIG(status) + 128, 0);
    }
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
    cleanup_exe(exe);
    closeallfiles(commands);
}
