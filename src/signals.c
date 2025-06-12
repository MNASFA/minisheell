/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:36:25 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/11 16:27:18 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signum = 0;

void	sigint_handler_her(int sig)
{
	const char	c = '\n';

	(void)sig;
	rl_redisplay();
	rl_on_new_line();
	rl_replace_line("\n", 0);
	ioctl(0, TIOCSTI, &c);
	g_signum = 130;
}

void	sigint_handler_main(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	if (waitpid(-1, NULL, WNOHANG) == 0)
		return ;
	set_exit_status(130, 1337);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler_re(int sign)
{
	(void)sign;
	write(1, "\n", 1);
	set_exit_status(130, 488);
}
