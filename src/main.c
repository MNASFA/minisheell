/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:04:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/21 20:02:34 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

// void print_cmd_list(t_cmd *cmd_list)
// {
//     t_cmd *current = cmd_list;
//     while (current)
//     {
//         t_token *token = current->token;
//         while (token)
//         {
//             printf("%s ==> %d    ", token->value, token->type);
//             token = token->next;
//         }
//         printf("\n");
//         current = current->next;
//     }
// }

void	sigint_handler(int sig)
{
	(void) sig;
	
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

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
		printf("	flag_double_quotes :%d\n", current->var_in_quotes);
		
		printf("	args 	: ");
		int j = 0;
		if (current->args[j])
		{
			while (current->args[j])
			{
				printf("%s ", current->args[j]);
				j++;
			}
		}
		printf("\n");
		t_redir *out = current->outfiles;
		t_redir	*in = current->infiles;
		while (in)
		{
			printf("	infiles: %s \n", in->filename);
			printf("	quotes: %d \n", in->quoted_flag);
			in = in->next;
		}
		while (out)
		{
			printf("	outfile: %s (append: %d)\n", out->filename, out->append);
			out = out->next;
		}
		if (current && current->infiles && current->infiles->is_herdoc)
		{
			// printf("	heredoc	: << %s\n", current->infiles->delimiter);
			printf("	flag : %d \n", current->infiles->quoted_flag);
			printf("	count : %d \n", current->infiles->heredoc_count);
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
	(void) ac;
	(void) av;

	char	*input;
	t_env	*env;
	
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	env = init_env(envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);
		t_exec *execs = build_exec_list(input, env);
		handle_all_herdocs(execs, env);
		print_exec_list(execs);
		execution(execs, &env);
		free_exec_list(execs);
		free(input);
	}
	free_envir(env);
	return (0);
}
