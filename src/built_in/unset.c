/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:02:12 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/04 14:58:20 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"

static void remove_from_env(t_env **env, char *to_delete)
{
    if (!env || !*env || !to_delete)
        return;
        
    t_env *current = *env;
    t_env *prev = NULL;
    
    while (current)
    {
        if (strcmp(current->key, to_delete) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next; 
            free(current->key);
            free(current->value);
            free(current->full);
            free(current);
            if (prev)
                current = prev->next;
            else
                current = *env;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}


void ft_unset(t_env **env, char **args)
{
    int i;

    i = 1;
    if (!args || !env)
        return;
    while(args[i])
    {
        remove_from_env(env, args[i]);
        i++;
    }
    return;
}


