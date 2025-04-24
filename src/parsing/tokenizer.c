/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 20:05:47 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/23 20:05:21 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token_type get_token_type(char *token)
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

t_token *create_token(char *content)
{
	t_token *new_token;
	
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
		
	new_token->value = ft_strdup(content);
	new_token->type = get_token_type(content);
	new_token->next = NULL;
	
	return (new_token);
}

t_token *tokenizer(char *input)
{
	char	**tokens;
	int		i;
	int		j;
	t_token *new_token;

	i = 0;
	tokens = ft_split(input);
	if (!tokens)
		return (NULL);
	t_token	*head = NULL;
	t_token	*current = NULL;
	while (tokens[i])
	{
		new_token = create_token(tokens[i]);
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
		i++;
	}
	j = 0;
	while (tokens[j])
	{
		free(tokens[j]);
		j++;
	}
	free(tokens);
	return (head);
}
