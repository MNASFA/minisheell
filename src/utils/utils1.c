/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:05:08 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/11 12:16:08 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_redir(t_token *tokens)
{
	return (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
		|| tokens->type == HEREDOC || tokens->type == APPEND);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*buffer_dest;
	unsigned char	*buffer_src;

	if (dest == src)
		return (dest);
	buffer_dest = (unsigned char *) dest;
	buffer_src = (unsigned char *) src;
	i = 0;
	while (i < n)
	{
		buffer_dest[i] = buffer_src[i];
		i++;
	}
	return (dest);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (1);
	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

char	*ft_strdup(const char *str)
{
	char	*ptr;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen (str);
	ptr = (char *) malloc((len + 1) * sizeof (char));
	if (ptr == NULL)
		return (NULL);
	ft_memcpy(ptr, (char *)str, len + 1);
	return (ptr);
}
