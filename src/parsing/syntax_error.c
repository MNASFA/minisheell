/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:20:40 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/26 20:53:01 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	track_quotes(char *input)
{
	int	in_double;
	int	in_single;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (input[i])
	{
		quotes_state(input[i], &in_single, &in_double);
		i++;
	}
	if (in_single || in_double)
		return (1);
	return (0);
}

char	*check_unclosed_quotes(char *input)
{
	int	track;

	track = track_quotes(input);
	if (track != 0)
	{
		write(2, "minishell: syntax error unclosed quotes\n", 41);
		set_exit_status(2, 1337);
		return (NULL);
	}
	return (input);
}

int	check_redir(int flag, t_token *tokens)
{
	if (!is_redir(tokens))
		return (0);
	if (!tokens->next)
	{
		if (flag)
			write(2, "minishell: syntax error near unexpected token\n", 47);
		set_exit_status(2, 1337);
		return (1);
	}
	if (is_redir(tokens->next)
		|| (tokens->next->type != WORD
			&& tokens->next->type != HEREDOC_DELIMITER))
	{
		if (flag)
			write(2, "minishell: syntax error near unexpected token\n", 47);
		set_exit_status(2, 1337);
		return (1);
	}
	return (0);
}

int	check_pipe(int flag, t_token *tokens, t_token *prev)
{
	if (tokens->type == PIPE)
	{
		if (!prev || !tokens->next || tokens->next->type == PIPE)
		{
			if (flag)
				write(2, "minishell: syntax error \
near unexpected token '|'\n", 51);
			set_exit_status(2, 1337);
			return (1);
		}
	}
	return (0);
}

int	check_errors(t_token *tokens, int check, t_token *prev, int her_count)
{
	t_token	*copy;

	copy = tokens;
	while (copy)
	{
		if (copy->type == HEREDOC)
			her_count++;
		if (her_count > 16)
		{
			write(2, "minishell: maximum here-document count exceeded\n", 49);
			exit(2);
		}
		if (check_pipe(1, copy, prev) || check_redir(1, copy))
			check = 1;
		if (check)
		{
			double_check_errors(tokens);
			return (1);
		}
		prev = copy;
		copy = copy->next;
	}
	return (0);
}
