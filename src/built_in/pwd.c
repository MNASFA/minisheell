/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:27 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/05 14:41:43 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"
#include <limits.h>


void pwd(t_env *env)
{

    while(env && env->key)
    {
        if (strcmp(env->key, "PWD") == 0)
            printf("%s\n", env->value);
        env = env->next;  
    }
}
