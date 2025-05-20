/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 20:05:47 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/18 15:33:08 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token_type	get_token_type(char *token)
{
	if (!ft_strcmp(token, "|"))
		return (PIPE);
	if (!ft_strcmp(token, "<"))
		return (REDIR_IN);
	if (!ft_strcmp(token, ">"))
		return (REDIR_OUT);
	if (!ft_strcmp(token, "<<"))
		return (HEREDOC);
	if (!ft_strcmp(token, ">>"))
		return (APPEND);
	return (WORD);
}

t_token	*create_token(char *content)
{
	t_token	*new_token;
	size_t len;

	len = ft_strlen(content);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(content);
	new_token->type = get_token_type(content);
	new_token->quoted_flag = 0;
	new_token->var_in_quotes = 0;
	if (len >=2 && content[0] == '\"' && content[len - 1] == '\"')
		new_token->var_in_quotes = 1;
	new_token->next = NULL;
	return (new_token);
}

int	is_in_quotes(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '\"' && !(*in_single))
		*in_double = !(*in_double);
	return (*in_single || *in_double);
}

int	get_token_length(char *input, int start, int in_single, int in_double)
{
	int	i;

	i = start;
	if ((input[i] == '|' || input[i] == '<' || input[i] == '>')
		&& !in_single && !in_double)
	{
		if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
			return (2);
		return (1);
	}
	while (input[i])
	{
		is_in_quotes(input[i], &in_single, &in_double);
		if (!in_single && !in_double
			&& (is_whitespace(input[i]) || input[i] == '|'
				|| input[i] == '<' || input[i] == '>'))
			break ;
		i++;
	}
	return (i - start);
}

static t_token	*ft_new_token(char *input, int *i)
{
	t_token	*new_token;
	char	*token_content;
	int		token_len;

	token_len = get_token_length(input, *i, 0, 0);
	token_content = malloc(token_len + 1);
	if (!token_content)
		return (NULL);
	ft_memcpy(token_content, &input[*i], token_len);
	token_content[token_len] = '\0';
	new_token = create_token(token_content);
	free(token_content);
	if (!new_token)
		return (NULL);
	*i += token_len;
	return (new_token);
}

t_token	*tokenizer(char *input)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		new_token = ft_new_token(input, &i);
		if (!new_token)
			return (free_token(head), NULL);
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
	}
	return (head);
}
