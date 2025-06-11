/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:04:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/11 22:20:09 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	print_exec_list(t_exec *execs)
{
	int i = 0;
	
	if (!execs)
		return;
	t_exec	*current = execs;
	while (current)
	{
		printf("Command %d:\n", ++i);
		printf("	cmd	: %s\n", current->cmd);
		printf("	args 	: ");
		int j = 0;
		if (current->args[j])
		{
			while (current->args[j])
			{
				printf("%s ,", current->args[j]);
				j++;
			}
		}
		printf("\n");
		t_redir *redir = current->redirections;
		if (redir)
			printf("Redirections list : \n");
		while (redir)
		{
			printf("	Filename : %s -----\n", redir->filename);
			if (redir->type == REDIR_IN)
				printf("	Type : REDIR_IN -----\n");
			else if (redir->type == REDIR_OUT)
				printf("	Type : REDIR_OUT -----\n");
			else if (redir->type == APPEND)
				printf("	Type : APPEND -----\n");
			else if (redir->type == HEREDOC)
			{
				printf("	Type : HEREDOC -----\n");
				printf("	Delimiter : %s -----\n", redir->delimiter);
				printf("	Herdoc_fd : %d -----\n", redir->herdoc_fd);
			}
			printf("	Herdoc Count : %d -----\n", redir->heredoc_count);
			printf("\n");
			redir = redir->next;
		}
		current = current->next;
	}
}

void free_envir(t_env *head)
{
    t_env *current;
    t_env *next;

    current = head;
    while (current)
    {
        next = current->next;
        if (current->key)
            free(current->key);
        if (current->value)
            free(current->value);
        if (current->full)
            free(current->full);
        free(current);
        current = next;
    }
}

int main(int ac, char **av, char **envp)
{
	char	*input;
	t_env	*env;
	(void) ac;
	(void) av;
	if (!isatty(0))
		return (ft_putstr_fd("input is not a terminal\n", 2), 1);
	
	signal(SIGQUIT, SIG_IGN);
	env = init_env(envp, 0, 0);
	while (1)
	{
		signal(SIGINT, sigint_handler_main);
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (!ft_strcmp(input , "\n"))
			set_exit_status(0, 1337);
		add_history(input);
		t_exec *execs = build_exec_list(input, env);
		if (!execs)
		{
			free(input);
			continue;
		}
		handle_all_herdocs(execs, env);
		if (g_signum == 130)
		{
			g_signum = 0;
			free_exec_list(execs);
			free(input);
			continue;
		}
		// print_exec_list(execs);
		signal(SIGINT, sigint_handler_re);
		execution(&execs, &env);
		free_exec_list(execs);
		free(input);
	}
	clear_history();
	free_envir(env);
	return (set_exit_status(12, -1));
}
