/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:47:51 by saalarco          #+#    #+#             */
/*   Updated: 2024/09/01 16:47:06 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int	switch_format_specifier(va_list args, char const *format, int p_count)
{
	size_t	pointer;

	if (*format == 's')
		ft_putstr_count(va_arg(args, char *), &p_count);
	else if (*format == 'd' || *format == 'i')
		ft_putnbr_count(va_arg(args, int), &p_count);
	else if (*format == 'c')
		ft_putchar_count((char)va_arg(args, int), &p_count);
	else if (*format == '%')
		ft_putchar_count('%', &p_count);
	else if (*format == 'x')
		ft_putbase_count(va_arg(args, unsigned int), HEX_L, &p_count, 0);
	else if (*format == 'X')
		ft_putbase_count(va_arg(args, unsigned int), HEX_U, &p_count, 0);
	else if (*format == 'p')
	{
		pointer = (size_t)va_arg(args, void *);
		ft_putbase_count(pointer, HEX_L, &p_count, 1);
	}
	else if (*format == 'u')
		ft_putbase_count(va_arg(args, unsigned int), DEC, &p_count, 0);
	return (p_count);
}

int	ft_printf(char const *format, ...)
{
	va_list	args;
	int		printed_count;
	int		i;

	va_start(args, format);
	printed_count = 0;
	i = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%' && format[i + 1] != '\0')
		{
			printed_count = switch_format_specifier(args, &format[++i],
					printed_count);
		}
		else if (format[i] == '%' && format[i + 1] == '\0')
			return (printed_count);
		else
			ft_putchar_count(format[i], &printed_count);
		i++;
	}
	va_end(args);
	return (printed_count);
}
