/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:30:59 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/18 11:00:23 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_herdoc_variables(char *str, t_env *env)
{
	t_expand_vars	vars;
	char			*result;

	init_expand_vars(&vars, str, env);
	result = malloc(expanded_length_herdoc(str, env) + 1);
	if (!result)
		return (NULL);
	while (str[vars.i])
	{
		if (str[vars.i] == '$' && !vars.in_single)
			handle_variable_expansion(&vars, result);
		else
			result[vars.j++] = str[vars.i++];
	}
	result[vars.j] = '\0';
	return (result);
	result[vars.j] = '\0';
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*sub;
	size_t			i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	sub = malloc(len + 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*extract_her_delimiter(char *value, int quoted_flag)
{
	int		dollar_count;
	int		i;
	char	*result;
	char	*tmp;

	dollar_count = count_dollars(value, 0);
	i = dollar_count;
	if (quoted_flag)
	{
		if (value[i] == '\0')
		{
			if (dollar_count % 2 == 1)
				dollar_count--;
			return (ft_substr(value, 0, dollar_count));
		}
		else if (dollar_count % 2 == 1)
		{
			tmp = ft_substr(value, 0, dollar_count - 1);
			if (!tmp)
				return (NULL);
			result = ft_strjoin(tmp, value + dollar_count);
			return (free(tmp), result);
		}
	}
	return (ft_strdup(value));
}
