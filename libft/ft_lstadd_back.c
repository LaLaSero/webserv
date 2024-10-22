/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 15:14:59 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:08:26 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last_p;

	if (lst == NULL || new == NULL)
		return ;
	last_p = ft_lstlast(*lst);
	if (last_p == NULL)
		*lst = new;
	else
		last_p -> next = new;
	return ;
}
