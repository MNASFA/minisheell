/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:04:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/07 14:16:02 by aboukhmi         ###   ########.fr       */
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

// void	sigint_handler(int sig)
// {
// 	(void) sig;

// 	write(1, "\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

// void	setup_signals(void)
// {
// 	struct sigaction	sa;

// 	sa.sa_handler = sigint_handler;
// 	SIGEMPTYSET(&sa.sa_mask);
// 	sa_sa_flags = SA_RESTART;
// 	sigaction(SIGINT, &sa, NULL);

// 	// Ignore SIGQUIT (CTRL + \)
// 	sa.sa_handler = SIG_IGN;
// 	sigaction(SIGQUIT, &sa, NULL);
// }

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
			in = in->next;
		}
		
		
		while (out)
		{
			printf("	outfile: %s (append: %d)\n", out->filename, out->append);
			out = out->next;
		}
		
		if (current->heredoc)
			printf("	heredoc	: << %s\n", current->delimiter);
		current = current->next;
	}
}

int main(int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;

	char	*input;
	t_env	*env;
	
	// setup_signals();
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
		execution(execs, env);
		free_exec_list(execs);
		free(input);
	}
	return (0);
}
