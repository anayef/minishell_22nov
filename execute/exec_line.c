/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anayef <anayef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:38:02 by m_kamal           #+#    #+#             */
/*   Updated: 2023/11/22 22:28:24 by anayef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	EXEC_LINE
**	---------
**	DESCRIPTION
**	The main workflow for the execution of all commands passed by the user through
**	the prompt line. It first opens all the pipes, and then it reads and applies
**	the redirects and heredocs (since they have precedence over pipes). Lastly, 
**	it executes the commands by priority indentation.
**	PARAMETERS
**	-
**	RETURN VALUES
**	-
*/

char **convert_tokens_to_char_array(t_token *tokens) //*new//
{
	int count;
	t_token *temp;
	int i; 
	int j;
	
	count = 0;
	temp = tokens;
	i = 0;
	while (temp != NULL)
	{
		count++;
		temp = temp->next;
	}
	char **args = malloc((count + 1) * sizeof(char *));
	if (args == NULL) 
		return NULL;
	temp = tokens;
	while (temp != NULL) {
		args[i] = ft_strdup(temp->tkn);
		if (args[i] == NULL)
		{
			while(j < i) 
			{
				free(args[j]);
				j++;
			}
			free(args);
			return NULL;
		}
		temp = temp->next;
		i++;
	}
	args[count] = NULL;
	return args;
}


void builtin_echo(char **args)//*new//
{
	int i;
	
	i = 1;
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL) {
			printf(" ");
		}
		i++;
	}
	printf("\n");
}

void builtin_pwd()//*new//
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd); 
	} 
	else 
		return;
}

void builtin_cd(char **args)//*new//
{
	if (args[1] == NULL) {
		return;//ERR//
	} else {
		if (chdir(args[1]) != 0)
			printf("bash: cd: %s : No such file or directory\n",args[1]);//ERR//
	}
}

void	exec_line(t_mini *minishell)
{
	// open_pipes();
	// if (!define_redirects())
	// 	return ;
	exec_cmds(minishell);
}


t_env *create_env_node(const char *variable, const char *value)//*new//
{
	t_env *new_node = malloc(sizeof(t_env));
	if (new_node)
	{
		new_node->variable = ft_strdup(variable);
		new_node->value = ft_strdup(value);
		new_node->next = NULL;
	}
	return (new_node);
}

void add_env_variable(t_env **env_list, const char *variable, const char *value)//*new//
{
	t_env *new_node = create_env_node(variable, value);
	if (new_node)
	{
		new_node->next = *env_list;
		*env_list = new_node;
	}
}

void free_env_list(t_env *env_list)//*new//
{
	while (env_list)
	{
		t_env *temp = env_list;
		env_list = env_list->next;
		free(temp->variable);
		free(temp->value);
		free(temp);
	}
}

void builtin_export(char **args, t_env **env_list)//*new//
{
	char *variable;
	char *value;
	
	if (args[1] == NULL)
	{
		t_env *current = *env_list;
		while (current != NULL) {
			printf("%s=\"%s\"\n", current->variable, current->value);
			current = current->next;
		}
	} 
	else
	{
		variable = args[1];
		value = strchr(args[1], '=');
		if (value != NULL)
		{
			*value = '\0';
			value++;
			t_env *current = *env_list;
			while (current != NULL)
			{
				if (strcmp(current->variable, variable) == 0) {
					free(current->value);
					current->value = strdup(value);
					return;
				}
				current = current->next;
			}
		}
		add_env_variable(env_list, variable, value != NULL ? value : "");
	}
}

void builtin_unset(char **args, t_env **env_list)//*new//
{
	if (args[1] == NULL)
		return;//ERR//
	else
	{
		int i = 1;
		while (args[i] != NULL)
		{
			t_env *current = *env_list;
			t_env *prev = NULL;
			while (current != NULL && strcmp(current->variable, args[i]) != 0)
			{
				prev = current;
				current = current->next;
			}
			if (current == NULL) 
				return;//ERR//
			else
			{
				if (prev != NULL)
					prev->next = current->next;
				else
					*env_list = current->next;
				free(current->variable);
				free(current->value);
				free(current);
			}
			i++;
		}
	}
}

void builtin_env(t_env **env_list)//*new//
{
	t_env *current = *env_list;
	while (current != NULL)
	{
		printf("%s=%s\n", current->variable, current->value);
		current = current->next;
	}
}

void exec_cmds(t_mini *minishell)//*new//
{
	t_cmd *cmd = minishell->cmd;

	while (cmd) {
		char **args = convert_tokens_to_char_array(cmd->commands);
		if (args != NULL) {
			if (strcmp(args[0], "echo") == 0)
				builtin_echo(args);
			else if (strcmp(args[0], "pwd") == 0)
				builtin_pwd();
			else if (strcmp(args[0], "cd") == 0)
				builtin_cd(args);
			else if (strcmp(args[0], "export") == 0)
				builtin_export(args, &(minishell->env_list));
			else if (strcmp(args[0], "unset") == 0)
				builtin_unset(args, &(minishell->env_list));
			else if (strcmp(args[0], "env") == 0)
				builtin_env(&(minishell->env_list));
			else
				printf("Command not recognized or not implemented yet\n"); //to be updated for the external//
		}
		cmd = cmd->next;
	}
}


