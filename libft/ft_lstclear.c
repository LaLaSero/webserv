/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:08:10 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/16 18:16:27 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	**tmp;
	t_list	*tmp2;

	if (lst == NULL || del == NULL)
		return ;
	tmp = lst;
	while (*tmp != NULL)
	{
		tmp2 = *tmp;
		del ((*tmp)-> content);
		*tmp = (*tmp)-> next;
		free(tmp2);
	}
	*tmp = NULL;
}
