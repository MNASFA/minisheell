/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:09:58 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/06/11 12:37:34 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strcpy(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s2)
		return (NULL);
	while (s2[i] != '\0')
	{
		s1[i] = s2[i];
		i++;
	}
	return (s1);
}

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	unsigned char	*ptr;
	unsigned char	*ptr1;
	size_t			i;

	ptr = (unsigned char *)str1;
	ptr1 = (unsigned char *)str2;
	if (!str1 || !str2)
		return (-1);
	i = 0;
	if (n == 0)
		return (0);
	while ((str1[i] != '\0' || str2[i] != '\0')
		&& str1[i] == str2[i] && i < n - 1)
		i++;
	return (ptr[i] - ptr1[i]);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

t_token	*ft_lstlast(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst -> next != NULL)
		lst = lst -> next;
	return (lst);
}

void	ft_lstadd_back_tpk(t_token **lst, t_token *new)
{
	if (!new)
		return ;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
}
