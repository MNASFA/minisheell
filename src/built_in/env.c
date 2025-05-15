/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:06:53 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/14 14:25:21 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"



void print_env(t_env *env)
{
    // int i;

    // i=0;
    while(env && env->full)
    {
        printf("%s\n", env->full);
        env = env->next;
    }
    return;
}