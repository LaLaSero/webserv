/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:40:16 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/17 14:41:16 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*tmp_to_attach;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	new_lst = ft_lstnew(f(lst -> content));
	if (new_lst == NULL)
		return (NULL);
	lst = lst -> next;
	while (lst != NULL)
	{
		tmp_to_attach = ft_lstnew(f(lst -> content));
		if (tmp_to_attach == NULL)
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, tmp_to_attach);
		lst = lst -> next;
	}
	return (new_lst);
}
