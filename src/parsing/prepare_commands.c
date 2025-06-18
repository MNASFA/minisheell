/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:44:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 13:32:07 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	copy_without_quotes(const char *src, char *dst, int *removed)
{
	int	i;
	int	j;
	int	quote;

	i = 0;
	j = 0;
	quote = 0;
	while (src[i])
	{
		if ((src[i] == '\'' || src[i] == '\"') && quote == 0)
		{
			quote = src[i++];
			*removed = 1;
		}
		else if (src[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
}

static int	remove_quotes(char **str_ptr, char **new_str)
{
	char	*str;
	int		quotes_removed;

	if (!str_ptr || !new_str)
		return (0);
	str = *str_ptr;
	quotes_removed = 0;
	if (!str)
	{
		*new_str = NULL;
		return (0);
	}
	*new_str = malloc(ft_strlen(str) + 1);
	if (!*new_str)
		return (0);
	copy_without_quotes(str, *new_str, &quotes_removed);
	return (quotes_removed);
}

static void	process_expansion(t_token *tokens, t_env *env)
{
	t_token		*current;
	char		*expanded_value;
	char		*quote_processed;

	detect_delimiter(tokens);
	current = tokens;
	while (current)
	{
		if (current->type == WORD)
		{
			expanded_value = expand_variables(current->value, env, current);
			if (expanded_value)
			{
				current->quoted_flag = remove_quotes(&expanded_value,
						&quote_processed);
				if (quote_processed)
				{
					free(current->value);
					current->value = quote_processed;
				}
				free(expanded_value);
			}
		}
		current = current->next;
	}
}

static void	process_heredoc(t_token *tokens)
{
	t_token	*current;
	char	*new_del;

	current = tokens;
	while (current)
	{
		if (current->type == HEREDOC_DELIMITER)
		{
			if (!current->quoted_flag)
			{
				current->quoted_flag = remove_quotes(&current->value, &new_del);
				free(current->value);
				current->value = new_del;
			}
		}
		current = current->next;
	}
}

void	print_token_list(t_token *head)
{
	while (head)
	{
		// if (head->value[0] == '\0')
		// 	head->value = "test";
		printf("  Value : [%s]\n", head->value);
		// printf("  quoted : %d\n", head->quoted_flag);
		head = head->next;
	}
}

void	print_cmd_list(t_cmd *cmds)
{
	while (cmds)
	{
		printf("------------- cmd --------------\n");
		if (cmds->token->value[0] == '\0')
			cmds->token->value = "test";
		printf("  Value : [%s]\n", cmds->token->value);
		cmds = cmds->next;
	}
}

t_cmd	*prepare_commands(char *input, t_env *env)
{
	t_token	*tokens;
	t_cmd	*cmds;
	t_token	*tokens2;

	input = check_unclosed_quotes(input);
	if (!input)
		return (NULL);
	tokens = tokenizer(input, 0);
	if (!tokens)
		return (NULL);
	print_token_list(tokens);
	detect_delimiter(tokens);
	if (check_errors(tokens, 0, NULL, 0) == 1)
	{
		free_token_list(tokens);
		return (NULL);
	}
	process_expansion(tokens, env);
	tokens2 = split_token_quotes(tokens);
	if (!tokens2)
		return (NULL);
	print_token_list(tokens2);
	detect_delimiter(tokens2);
	process_heredoc(tokens2);
	cmds = split_by_pipe(tokens2);
	print_cmd_list(cmds);
	free_token_list(tokens2);
	return (cmds);
}
