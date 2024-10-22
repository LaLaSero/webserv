/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:01:36 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:08:11 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

// #include <ctype.h>
// #include<stdio.h>
// int main()
// {
// printf("%d\n",isdigit(1222));
// printf("%d\n",isdigit('3'));
// printf("%d\n",ft_isdigit(1222));
// printf("%d\n",ft_isdigit('3'));
// }
