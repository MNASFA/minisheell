/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:24 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/18 16:20:27 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

static int	handle_invalid_name(char *arg, char **str)
{
	write(2, "minishell: export: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	set_exit_status(1, 1337);
	if (str)
		freeee(str);
	return (0);
}

static int	process_equal_case(char **args, t_env **env, int i)
{
	char	**str;
	t_env	*new;

	str = splite_exp(args[i], '=');
	if (!str || !is_valid_name(str[0]))
		return (handle_invalid_name(args[i], str));
	new = malloc(sizeof(t_env));
	if (!new)
	{
		freeee(str);
		return (0);
	}
	new->key = ft_strdup(str[0]);
	new->value = ft_strdup(str[1]);
	new->full = ft_strdup(args[i]);
	new->is_print = 1;
	new->next = NULL;
	delete_if_exist(env, str[0]);
	ft_lstadd_back_ex(env, new);
	freeee(str);
	return (1);
}

static int	process_plus_case(char **args, t_env **env, int i)
{
	char	**str;

	str = splite_exp(args[i], '=');
	if (!is_valid_name(str[0]))
		return (handle_invalid_name(args[i], str));
	freeee(str);
	str = splite_exp(args[i], '+');
	update(str[0], env, str[1] + 1);
	freeee(str);
	return (1);
}

static int	process_no_equal_case(char **args, t_env **env, int i)
{
	t_env	*new;

	if (!is_valid_name(args[i]) || !args[i][0])
		return (handle_invalid_name(args[i], NULL));
	if (is_in_env(*env, args[i]) == 1)
		return (0);
	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->key = ft_strdup(args[i]);
	new->full = ft_strdup(args[i]);
	new->value = NULL;
	new->next = NULL;
	new->is_print = 0;
	delete_if_exist(env, args[i]);
	ft_lstadd_back_ex(env, new);
	return (1);
}

void	ft_export(char **args, t_env **env)
{
	int	i;
	int	equal_type;

	i = 1;
	if (!args || !env)
		return ;
	if (!check_printable(args))
	{
		print_sorted(env);
		return ;
	}
	while (args[i])
	{
		equal_type = is_there_equal(args[i]);
		if (equal_type == 1)
			process_equal_case(args, env, i);
		else if (equal_type == 2)
			process_plus_case(args, env, i);
		else
			process_no_equal_case(args, env, i);
		i++;
	}
}
