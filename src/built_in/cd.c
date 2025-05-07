/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukhmi <aboukhmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:07:09 by aboukhmi          #+#    #+#             */
/*   Updated: 2025/05/06 20:29:27 by aboukhmi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution/execution.h"
#include <string.h>

static size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (src && size != 0)
	{
		i = 0;
		while (i < size - 1 && src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen(src));
	return (0);
}

char **parse_args(char *str)
{
    return ft_split_exe(str, '/');
}

t_env *find_in_env(char *key, t_env *env)
{
    while (env && ft_strcmp(env->key, key))
        env = env->next;
    return env;
}

void free_array(char **arr)
{
    int i = 0;
    if (!arr)
        return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}

char *get_path_from_env(t_env **env)
{
    t_env *pwd_env = find_in_env("PWD", *env);
    return pwd_env ? ft_strdup(pwd_env->value) : ft_strdup("/");
}

void export_env_var(char *key, char *value, t_env **env)
{
    char *tmp = ft_strjoin(key, "=");
    char *str = ft_strjoin(tmp, value);
    free(tmp);
    
    char **args = malloc(3 * sizeof(char *));
    args[0] = ft_strdup("export");
    args[1] = str;
    args[2] = NULL;
    
    ft_export(args, env);
    free(args[0]);
    free(args[1]);
    free(args);
}

int check_path_valid(char *path)
{
    int ret = chdir(path);
    if (ret == 0)
    {
        char *cwd = getcwd(NULL, 0);
        if (cwd)
        {
            free(cwd);
            return 1;
        }
    }
    return 0;
}

char *get_parent_dir(char *path)
{
    char *last_slash = strrchr(path, '/');

    if (path[0] == '/' && (!last_slash || last_slash == path))
        return (ft_strdup("/"));
    if (last_slash)
    {
        int len = last_slash - path;
        if (len == 0)
            len = 1;
        char *parent = malloc(len + 1);
        ft_strlcpy(parent, path, len + 1);
        if (len == 1 && parent[0] == '/')
            parent[1] = '\0';
        return (parent);
    }
    
    return (ft_strdup("."));
}

void update_pwd_env(t_env **env)
{
    char *real_path = getcwd(NULL, 0);
    t_env *pwd_env = find_in_env("PWD", *env);
    if (real_path)
    {
        if (pwd_env)
        {
            free(pwd_env->value);
            pwd_env->value = real_path;
        }
    }
    else
    {
        pwd_env->full=ft_strjoin(pwd_env->value, "/..");
        export_env_var("PWD", pwd_env->full, env);
    }
}

void process_cd_no_args(t_env **env)
{
    char *old_pwd = get_path_from_env(env);
    char *home = getenv("HOME");
    
    if (chdir(home) == 0)
    {
        export_env_var("OLDPWD", old_pwd, env);
        update_pwd_env(env);
    }
    else
        perror("cd");
    free(old_pwd);
}

void handle_dotdot(t_env **env)
{
    char *old_pwd = get_path_from_env(env);
    
    if (chdir("..") == 0)
    {
        export_env_var("OLDPWD", old_pwd, env);
        update_pwd_env(env);
    }
    else
        perror("cd");
    free(old_pwd);
}

void handle_regular_component(char *component, t_env **env)
{
    char *old_pwd = get_path_from_env(env);
    
    if (chdir(component) == 0)
    {
        export_env_var("OLDPWD", old_pwd, env);
        update_pwd_env(env);
    }
    else
        perror("cd");
    free(old_pwd);
}

void process_path_component(char *component, t_env **env)
{
    if (!ft_strcmp(component, ".."))
        handle_dotdot(env);
    else if (!ft_strcmp(component, "."))
        return;
    else if (!ft_strcmp(component, "~"))
        process_cd_no_args(env);
    else
        handle_regular_component(component, env);
}

void cd(char *av, t_env **env)
{
    if (!av)
    {
        process_cd_no_args(env);
        return;
    }
    
    char **arg = parse_args(av);
    int i = 0;
    while (arg[i])
        process_path_component(arg[i++], env);
    free_array(arg);
}