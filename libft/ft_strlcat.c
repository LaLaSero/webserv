/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:40:10 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:10:02 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	k = 0;
	if (dst == NULL && dstsize == 0)
		return (ft_strlen(src));
	while (dst[i] != '\0')
		i++;
	while (src[j] != '\0')
		j++;
	if (dstsize <= i)
		return (j + dstsize);
	j += i;
	while (src[k] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[k];
		i++;
		k++;
	}
	dst[i] = '\0';
	return (j);
}

// #include<stdio.h>
// #include<string.h>
// int main()
// {
// 	char dest[20] = "Hello";
// 	char src[20] = "World";
// 	int i = 0;
// 	// printf("%zu\n", ft_strlcat(NULL, src, i));
// 	printf("%zu\n", strlcat(NULL, src, i));
// 	printf("%s\n", dest);
// 	return (0);
// }