/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:50:06 by saalarco          #+#    #+#             */
/*   Updated: 2024/07/12 19:57:47 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft.h"
# include <stdarg.h>
# include <unistd.h>

# define HEX_L "0123456789abcdef"
# define HEX_U "0123456789ABCDEF"
# define DEC "0123456789"

int		ft_printf(char const *str, ...);
void	ft_putstr_count(char *s, int *i);
void	ft_putnbr_count(int s, int *i);
void	ft_putchar_count(char c, int *i);
void	ft_putbase_count(size_t nbr, char *base, int *i, int pointer);

#endif
