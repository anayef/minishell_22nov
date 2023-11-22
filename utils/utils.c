/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anayef <anayef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:50:04 by m_kamal           #+#    #+#             */
/*   Updated: 2023/11/22 21:40:30 by anayef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	write_err(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		write(2, &str[i], 1);
}

void	exit_errno(char *errfile, int errnb)
{
	if (errnb == EACCES)
		ft_error(errfile, 31, 1);
	else if (errnb == ENOENT)
		ft_error(errfile, 32, 1);
	else
		ft_error(errfile, 15, 127);
}

void	free_arr(void **split_arr)
{
	int	i;

	i = -1;
	if (!split_arr)
		return ;
	while (split_arr[++i])
		free(split_arr[i]);
	free(split_arr);
}

char	**ft_arrdup(char **arr)
{
	char	**rtn;
	size_t	i;
	i = 0;
	while (arr[i] != NULL)
		i++;
	rtn = ft_calloc(sizeof(char *), i + 1);
	if (!rtn)
		return (NULL);
	i = 0;
	while (arr[i] != NULL)
	{
		rtn[i] = ft_strdup(arr[i]);
		if (rtn[i] == NULL)
		{
			free_arr((void **)rtn);
			return (rtn);
		}
		i++;
	}
	return (rtn);
}
