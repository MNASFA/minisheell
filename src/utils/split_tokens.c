/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:33:27 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/12 11:47:59 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	copy_token_data(t_token *dest, t_token *src)
{
	dest->expanded_flag = src->expanded_flag;
	dest->var_in_quotes = src->var_in_quotes;
	dest->type = src->type;
}

static t_token	*handle_expanded_token(t_token *head, t_token **new_tokens)
{
	t_token	*split_tokens;
	t_token	*temp;

	split_tokens = tokenizer(head->value, 0);
	temp = split_tokens;
	while (temp)
	{
		copy_token_data(temp, head);
		temp = temp->next;
	}
	ft_lstadd_back_tpk(new_tokens, split_tokens);
	return (head->next);
}

static t_token	*handle_regular_token(t_token *head, t_token **new_tokens)
{
	t_token	*new_node;

	new_node = create_token(head->value);
	copy_token_data(new_node, head);
	ft_lstadd_back_tpk(new_tokens, new_node);
	return (head->next);
}

t_token	*split_token_quotes(t_token *token_origin)
{
	t_token	*new_tokens;
	t_token	*head;

	new_tokens = NULL;
	if (!token_origin)
		return (NULL);
	head = token_origin;
	while (head)
	{
		if (head->expanded_flag && !head->var_in_quotes && head->value[0])
			head = handle_expanded_token(head, &new_tokens);
		else
			head = handle_regular_token(head, &new_tokens);
	}
	free_token_list(token_origin);
	return (new_tokens);
}
