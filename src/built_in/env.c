/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:06:53 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/15 10:16:11 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"



void print_env(t_env *env)
{
    while(env && env->full && env->is_print == 1)
    {
        printf("%s\n", env->full);
        env = env->next;
    }
    return;
}