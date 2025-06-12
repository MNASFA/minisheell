/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:24 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/06/12 15:31:13 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

// static t_env	*ft_lstlast_ex(t_env *lst)
// {
// 	if (!lst)
// 		return (NULL);
// 	while (lst && lst->next)
// 		lst = lst->next;
// 	return (lst);
// }

// void	ft_lstadd_back_ex(t_env **lst, t_env *new)
// {
// 	if (!lst || !new)
// 		return ;
// 	if (*lst)
// 		ft_lstlast_ex(*lst)->next = new;
// 	else
// 		*lst = new;
// }

// t_env	*free_all_env(t_env *to_free)
// {
// 	t_env	*next_node;

// 	next_node = to_free->next;
// 	free(to_free->key);
// 	free(to_free->value);
// 	free(to_free->full);
// 	free(to_free);
// 	return (next_node);
// }

// void	delete_if_exist(t_env **env, char *to_delete)
// {
// 	t_env	*current;
// 	t_env	*prev;

// 	if (!env || !*env || !to_delete)
// 		return ;
// 	current = *env;
// 	prev = NULL;
// 	while (current)
// 	{
// 		if (ft_strcmp(current->key, to_delete) == 0)
// 		{
// 			if (prev)
// 				prev->next = current->next;
// 			else
// 				*env = current->next;
// 			current = free_all_env(current);
// 		}
// 		else
// 		{
// 			prev = current;
// 			current = current->next;
// 		}
// 	}
// }

// int	is_there_equal(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '=')
// 		{
// 			if (i > 0 && str[i - 1] && str[i - 1] == '+')
// 				return (2);
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

// int	find_next_min(t_env **env, int *printed, int size)
// {
// 	int	min_idx;
// 	int	j;

// 	min_idx = -1;
// 	j = 0;
// 	while (j < size)
// 	{
// 		if (!printed[j])
// 		{
// 			if (min_idx == -1
// 				|| ft_strcmp(env[j]->full, env[min_idx]->full) < 0)
// 				min_idx = j;
// 		}
// 		j++;
// 	}
// 	return (min_idx);
// }

// int	env_count_hh(t_env *tmp)
// {
// 	int	count;

// 	count = 0;
// 	if (!tmp)
// 		return (0);
// 	while (tmp)
// 	{
// 		count++;
// 		tmp = tmp->next;
// 	}
// 	return (count);
// }

// t_env	**env_t_array(t_env *env, int count)
// {
// 	t_env	**env_array;
// 	int		i;

// 	i = 0;
// 	env_array = malloc(count * sizeof(t_env *));
// 	if (!env_array)
// 		return (NULL);
// 	while (i < count)
// 	{
// 		env_array[i] = env;
// 		env = env->next;
// 		i++;
// 	}
// 	return (env_array);
// }

// int	*printed_tracker(t_env **env_array, int count)
// {
// 	int	*printed;
// 	int	i;

// 	i = 0;
// 	printed = malloc(count * sizeof(int));
// 	if (!printed)
// 	{
// 		free(env_array);
// 		return (NULL);
// 	}
// 	while (i < count)
// 	{
// 		printed[i] = 0;
// 		i++;
// 	}
// 	return (printed);
// }

// void	printtt(t_env **env_array, int *printed, int count)
// {
// 	int	i;
// 	int	min_idx;

// 	i = 0;
// 	while (i < count)
// 	{
// 		min_idx = find_next_min(env_array, printed, count);
// 		if (min_idx != -1 && min_idx < count)
// 		{
// 			printf("declare -x %s", env_array[min_idx]->key);
// 			if (env_array[min_idx]->value)
// 				printf("=\"%s\"\n", env_array[min_idx]->value);
// 			else if (!env_array[min_idx]->value
// 				&& env_array[min_idx]->is_print == 1)
// 			{
// 				printf("=\"\"\n");
// 			}
// 			else
// 				printf("\n");
// 			printed[min_idx] = 1;
// 		}
// 		i++;
// 	}
// }

// void	print_sorted(t_env **env)
// {
// 	int		count;
// 	t_env	*temp;
// 	t_env	**env_array;
// 	int		*printed;

// 	temp = *env;
// 	count = env_count_hh(temp);
// 	if (count == 0)
// 		return ;
// 	env_array = env_t_array(*env, count);
// 	if (!env_array)
// 		return ;
// 	printed = printed_tracker(env_array, count);
// 	if (!printed)
// 		return ;
// 	printtt(env_array, printed, count);
// 	free(printed);
// 	free(env_array);
// }

// static void	update_value(t_env *env, char *new)
// {
// 	char	*tmp;

// 	tmp = ft_strdup(env->value);
// 	free(env->value);
// 	env->value = ft_strjoin(tmp, new);
// 	free(tmp);
// }

// static int	update_full(t_env *env, char *new)
// {
// 	char	*tmp;

// 	if (env->is_print == 0)
// 	{
// 		tmp = ft_strjoin(env->key, "=");
// 		if (!tmp)
// 			return (0);
// 		free(env->full);
// 		env->full = ft_strjoin(tmp, env->value);
// 		free(tmp);
// 		env->is_print = 1;
// 	}
// 	else
// 	{
// 		tmp = ft_strdup(env->full);
// 		if (!tmp)
// 			return (0);
// 		free(env->full);
// 		env->full = ft_strjoin(tmp, new);
// 		free(tmp);
// 	}
// 	return (1);
// }

// void	update(char *key, t_env **env, char *new)
// {
// 	t_env	*copy;

// 	copy = *env;
// 	while (copy && copy->key)
// 	{
// 		if (!ft_strcmp(key, copy->key))
// 		{
// 			update_value(copy, new);
// 			if (!copy->value)
// 				return ;
// 			if (!update_full(copy, new))
// 				return ;
// 			break ;
// 		}
// 		copy = copy->next;
// 	}
// }

// int	is_in_env(t_env *env, char *key)
// {
// 	while (env)
// 	{
// 		if (!ft_strcmp(env->key, key))
// 			return (1);
// 		env = env->next;
// 	}
// 	return (0);
// }

// int	is_valid_name(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (!str || !str[0])
// 		return (0);
// 	while (str[i])
// 	{
// 		if (!((str[i] >= 65 && str[i] <= 90)
// 				|| (str[i] >= 97 && str[i] <= 122) || str[i] == 95))
// 		{
// 			if (str[i] >= 48 && str[i] <= 57 && i > 0)
// 			{
// 				i++;
// 				continue ;
// 			}
// 			return (0);
// 		}
// 		i++;
// 	}
// 	return (1);
// }

// int	check_printable(char **args)
// {
// 	int	i;

// 	i = 1;
// 	while (args[i])
// 	{
// 		if (args[i][0] != '\0')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

static int	handle_invalid_name(char *arg, char **str)
{
	printf("minishell: export: `%s': not a valid identifier\n", arg);
	set_exit_status(1, 1337);
	if (str)
		freeee(str);
	return (0);
}

static int	process_equal_case(char **args, t_env **env, int i)
{
	char	**str;
	t_env	*new;

	str = ft_split_exe(args[i], '=');
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

	str = ft_split_exe(args[i], '+');
	if (!is_valid_name(str[0]))
		return (handle_invalid_name(args[i], str));
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
