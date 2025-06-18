/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:04:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 14:51:30 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_input_herdoc(t_exec *execs, t_env *env)
{
	handle_all_herdocs(execs, env);
	if (g_signum == 130)
	{
		g_signum = 0;
		return (1);
	}
	return (0);
}

static int	should_skip_input(char *input)
{
	if (!ft_strcmp(input, "\n"))
	{
		set_exit_status(0, 1337);
		free(input);
		return (1);
	}
	if (input[0] != '\0')
		add_history(input);
	return (0);
}

static int	handle_exec_block(char *input, t_env **env)
{
	t_exec	*execs;

	execs = build_exec_list(input, *env);
	if (!execs)
	{
		free(input);
		return (0);
	}
	if (handle_input_herdoc(execs, *env))
	{
		free_exec_list(execs);
		free(input);
		return (0);
	}
	signal(SIGINT, sigint_handler_re);
	execution(&execs, env);
	free_exec_list(execs);
	return (1);
}

static void	run_minishell_loop(t_env **env)
{
	char	*input;

	while (1)
	{
		signal(SIGINT, sigint_handler_main);
		input = readline("minishell$ ");
		if (!input)
			return ((void)printf("exit\n"));
		if (should_skip_input(input))
			continue ;
		if (!handle_exec_block(input, env))
			continue ;
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;

	(void) ac;
	(void) av;
	if (!isatty(0))
		return (ft_putstr_fd("input is not a terminal\n", 2), 2);
	signal(SIGQUIT, SIG_IGN);
	env = init_env(envp, 0, 0);
	run_minishell_loop(&env);
	clear_history();
	free_envir(env);
	return (set_exit_status(12, -1));
}
