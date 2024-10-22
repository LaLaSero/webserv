/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 10:18:07 by marvin            #+#    #+#             */
/*   Updated: 2023/10/12 14:09:38 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t			len;
	unsigned int	i;

	if (s == NULL)
		return ;
	len = ft_strlen(s);
	i = (unsigned int)len;
	write(fd, s, i);
	return ;
}
