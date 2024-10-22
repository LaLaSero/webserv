/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:26:54 by marvin            #+#    #+#             */
/*   Updated: 2023/10/12 14:09:03 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*p;

	p = (unsigned char *) s;
	i = 0;
	while (i < n)
	{
		if (p[i] == (unsigned char) c)
			return ((void *) &p[i]);
		i++;
	}
	return (NULL);
}

// #include <string.h>
// #include<stdio.h>
// int main()
// {
// 	char buf[] = "ABCD5EFGH";
// 	char *ch;
// 	char *ft_ch;
// 	char c = 'Z';
// 	ch = (char*)memchr(buf,c,sizeof(buf));
// 	ft_ch = (char*)memchr(buf,c,sizeof(buf));
// 	printf("%s\n",ch);
// 	printf("%s\n",ft_ch);
// }