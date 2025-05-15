/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:42:25 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/14 22:43:08 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

int is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
int word_len(char *str)
{
	int i = 0;
	
	if (is_special_char(str[i]))
	{
		if ((str[i] == '<' || str[i] == '>') && str[i + 1] == str[i])
			return (2);
		return (1);
	}
	while (str[i] && !is_whitespace(str[i]) && !is_special_char(str[i]))
		i++;
	return (i);
}

int count_words(char *input)
{
	int i = 0;
	int count = 0;
	
	while (input[i])
	{
		while (is_whitespace(input[i]))
			i++;
		if (input[i])
		{
			count++;
			i += word_len(&input[i]);
		}
	}
	return (count);
}

void free_tokens(char **tokens, int count)
{
	int	i;
	
	i = 0;
	while (i < count)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

char **ft_split(char *input)
{
	char	**tokens;
	int		count;
	int		i;
	int		len;

	i = 0;
	count = 0;
	tokens = malloc(sizeof(char *) * (count_words(input) + 1));
	if (!tokens)
		return (NULL);
	
	while (input[i])
	{
		while (is_whitespace(input[i]))
			i++;
		if (!input[i])
			break;
		len = word_len(&input[i]);
		tokens[count] = malloc(len + 1);
		if (!tokens[count])
		{
			free_tokens(tokens, count);
			return (NULL);
		}
		ft_memcpy(tokens[count], &input[i], len);
		tokens[count][len] = '\0';

		count++;
		i += len;
	}
	tokens[count] = NULL;
	return (tokens);
}
