/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anayef <anayef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:27:14 by m_kamal           #+#    #+#             */
/*   Updated: 2023/11/21 20:12:02 by anayef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env *creat_variable_value(char *variable, char *value)
{
	t_env *ptr;
	ptr = (t_env *)malloc(sizeof(t_env));
	if (!ptr)
		return (NULL);
	ptr->variable = variable;
	if (!ptr->variable)
		return (NULL);
	ptr->value = value;
	if (!ptr->variable)
		return (NULL);
	ptr->next = NULL;
	return (ptr);
}

void add_variable_value(t_env **head, char *variable, char *value)
{
	t_env *node;
	t_env *current;
	node = creat_variable_value(variable,value);
	if (node)
	{
		if (*head == NULL)
			*head = node;
		else
		{
			current = *head;
			while (current->next != NULL)
				current = current->next;
			current->next = node;
		}
	}
}


void free_variables(t_env *head)
{
	t_env *current;
	while (head != NULL)
	{
		current = head;
		head = head->next;
		free(current->variable);
		free(current->value);
		free(current);
	}
}


void builtin_export(t_env **head, char *argv[])
{
	char *variable;
	char *value;
	char *eq;
	int i;
	
	i = 1;
	if (argv[i] != NULL)
	{
		while (argv[i] != NULL)
		{
			eq = strchr(argv[i], '=');
			if (eq)
			{
				*eq = '\0';
				variable = strdup(argv[i]);
				value = strdup(eq + 1);
				add_variable_value(head,variable, value);
			}
			else
				add_variable_value(head,argv[i], "");
			i++;
		}
	}
	else
		print_variables(*head);
}
