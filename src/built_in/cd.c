#include "../execution/execution.h"

char **parse_args(char *str)
{
	char **args;
	args = ft_split_exe(str, '/');
	return(args);
}
char    *get_path_2(char *str)
{
	int n;
	char *new_path;

	n = ft_strlen(str);
	while (str[n -1] && str[n -1] != '/')
		n--;
	new_path = (char *)malloc(n + 1);
	strncpy(new_path, str, n);
	new_path[n] = '\0';
	free(str);
	return (new_path);
	
}

char *pwd1()
{
	char	*path;

	path = (char *)malloc(PATH_MAX);
	getcwd(path, PATH_MAX);
	return (path);
}

void cd(char *av)
{
	char *path;
	char **arg;
	arg = parse_args(av);
	printf("%s\n",arg[0]);
	int i;
	i = 0;

	while (arg[i])
	{
		if (!strcmp(arg[i], "."))
			;
		else if (!strcmp(arg[i], "~"))
			chdir(getenv("HOME"));
		else if (!ft_strcmp(arg[i], ".."))
		{
			path = pwd1();
			chdir(getenv("HOME"));
			path = get_path_2(path);
			chdir(path);
			pwd1();
			free(path);
		}
		else
			chdir(arg[i]);
		i++;
	}
}


int main() {

	char *path = "../execution/../parsing"; // Absolute path
	// pwd1();
	// printf("Successfully changed directory to: %s\n", path);
	pwd();
	cd(path);
	pwd();
	// cd ("~");
	// // Attempt to change the directory
	// // if (chdir("$~") != 0) {
	// //     perror("Error changing directory"); // Print error message
	// //     return 1;
	// // }
	// pwd();
	// printf("Successfully changed directory to: %s\n", path);
	// return 0;
}