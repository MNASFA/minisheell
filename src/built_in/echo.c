/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:39:58 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/05/10 21:29:43 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdlib.h>
#include <string.h>

int	check_flag(char *str)
{
	if (*str != '-')
		return (0);
	str++;
	while (*str)
	{
		if (*str != 'n')
			return (0);
		str++;
	}
	return (1);
}

void	print_args(char **arg)
{
	int	i;
	
	i = 0;
	while (arg[i])
	{
		printf("%s ", arg[i]);
		if (arg[i + 1])
			printf(" ");
		i++;
	}
}

void	ft_echo(char **arg)
{
	int	i;
	int	n_flag;
	
	i = 1;
	if (!arg[1])
	{
		printf("\n");
		return;
	}
	if (check_flag(arg[1]))
		n_flag = 1;
	else
		n_flag = 0;
	if (n_flag)
		i++;
	print_args(&arg[i]);
	if (!n_flag)
		printf("\n");
}


// int main()
// {
// 	char **args;
// 	args = (char **)malloc(sizeof(char *) * 4);
// 	args[0]= strdup("echo");
// 	args[1] = strdup("-nnnnnnnnnnnnnnnnnnnnnnnn");
// 	args[2] = strdup("-n -nhello world");
// 	args[3]= NULL;
// 	ft_echo(args);
// }