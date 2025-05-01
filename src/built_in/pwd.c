/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:27 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/01 19:07:28 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"
#include <limits.h>


void pwd()
{
    char *path;
    path = (char *)malloc(PATH_MAX);
    getcwd(path, PATH_MAX);
    printf("%s\n", path);
    free(path);
}
