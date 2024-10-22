/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:06:48 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:08:06 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (0 <= c && c <= 127)
		return (1);
	return (0);
}

// #include <ctype.h>
// #include<stdio.h>
// int main()
// {
// 	printf("%d\n",isascii(1222));
// 	printf("%d\n",isascii('\n'));
// 	printf("%d\n",ft_isascii(1222));
// 	printf("%d\n",ft_isascii('\n'));
// }