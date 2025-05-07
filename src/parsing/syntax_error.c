/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:20:40 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/26 09:40:51 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int track_quotes(char *input)
{
	int track_d_quotes = 0;
	int track_s_quotes = 0;
	int i = 0;
	
	while (input[i])
	{
		if(input[i] == '\"' && track_s_quotes == 0)
			track_d_quotes++;
		else if (input[i] == '\'' && track_d_quotes == 0)
			track_s_quotes++;
		i++;	
	}
	if (track_d_quotes != 0 && track_d_quotes % 2 != 0)
		return (2);
	else if (track_s_quotes % 2 != 0 && track_s_quotes != 0)
		return (1);
	else
		return(0);
}

char	*check_unclosed_quotes(char *input)
{
	int track;

	track = track_quotes(input);
	if (track != 0)
	{
		printf("minishell: syntax error unclosed quotes\n");
		return (NULL);
	}
	return (input);
}

int check_redirection_err(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT || tokens->type == HEREDOC 
			|| tokens->type == APPEND)
		{
			if (!tokens->next || tokens->next->type != WORD)
			{
				if (tokens->next)
					printf("minishell: syntax error near unexpected token `%s'\n", tokens->next->value);
				else
					printf("minishell: syntax error near unexpected token `newline'\n");
				return (1);
			}
			if (tokens->next && (tokens->next->type == REDIR_IN || tokens->next->type == REDIR_OUT 
								|| tokens->next->type == HEREDOC || tokens->next->type == APPEND))
			{
				printf("minishell: syntax error near unexpected token `%s'\n", tokens->next->value);
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}

int check_two_pipes(char *input)
{
	int i = 0;
	int j;
	while (input[i])
	{
		if(input[i] == '|')
		{
			j = 1;
			while(input[i+j] && (input[i + j] == 32 || input[i+j] == '\t'))
				j++;
			if(input[i + j] == '|')
				return(0);
		}
		i++;
	}
	return(1);
}

int is_end(char *input)
{
	int len;

	len = ft_strlen(input);
	len--;
	while(input[len] && (input[len] == 32 || input[len] == '\t'))
		len--;
	if(input[len] == '|')
		return(0);
	return(1);
}

int	is_pipe_at_start(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '|')
		return (1);
	return(0);
}

char *handle_pipe_end(char *input)
{
	int		check;
	char	*str;

	check = is_end(input);
	while(!check)
	{
		str = readline("> ");
		if (!str)
		{
			printf("\nminishell: unexpected EOF while looking for matching quote\n");
			return (NULL);
		}
		input = ft_strjoin(input, str);
		free(str);
		check = is_end(input);
	}
	return(input);
}
