/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 09:45:33 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/11/19 13:29:10 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

static void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const char	*c_src = src;
	char		*c_dst;

	c_dst = dst;
	while (n--)
		*c_dst++ = *c_src++;
	return (dst);
}

static void	*ft_memchr(const void *s, int c, size_t n)
{
	const int		*c_s = s;
	unsigned char	*b_s;
	int				repeated_c;
	int				mask;

	repeated_c = (c & 0xFF) | ((c & 0xFF) << 8);
	repeated_c |= repeated_c << 16;
	while (n >= 4)
	{
		mask = *c_s++ ^ repeated_c;
		if ((mask & 0xff) == 0)
			return ((void *)c_s - 4);
		if ((mask & 0xff00) == 0)
			return ((void *)c_s - 3);
		if ((mask & 0xff0000) == 0)
			return ((void *)c_s - 2);
		if ((mask & 0xff000000) == 0)
			return ((void *)c_s - 1);
		n -= 4;
	}
	b_s = (unsigned char *)c_s;
	while (n--)
		if (*b_s++ == (unsigned char)c)
			return ((void *)b_s - 1);
	return (NULL);
}

t_buff		**get_next_line_buff(const int fd)
{
	static t_buff	*buff[OPEN_MAX];

	return (buff + fd);
}

static int	get_next_line_cpy(t_buff **b, char **line, size_t len, char *pos)
{
	t_buff			*tmp;
	t_buff			*tmp2;
	const size_t	b_len = (*b)->len;

	ft_memcpy(*line + len, (*b)->data, pos - (*b)->data);
	tmp = (*b)->prev;
	(*b)->prev = NULL;
	if ((*b)->len)
		ft_memcpy((*b)->data, pos + 1, (*b)->len -= (pos - (*b)->data + 1));
	if (!(*b)->len)
	{
		free(*b);
		*b = NULL;
	}
	if (b_len == 0 && len == 0)
		return (0);
	while (tmp)
	{
		ft_memcpy(*line + (len -= tmp->len), tmp->data, tmp->len);
		tmp2 = tmp;
		tmp = tmp->prev;
		free(tmp2);
	}
	return (1);
}

int			get_next_line(const int fd, char **line)
{
	t_buff **const	b = get_next_line_buff(fd);
	t_buff			*tmp;
	char			*pos;
	size_t			len;
	ssize_t			r;

	if (!line || fd < 0 || fd >= OPEN_MAX || BUFF_SIZE <= 0)
		return (-1);
	len = 0;
	r = 1;
	pos = *b ? (*b)->data : 0;
	while (!(*b && (r <= 0 || (pos = ft_memchr((*b)->data, '\n', (*b)->len)))))
	{
		len += *b ? (*b)->len : 0;
		tmp = malloc(sizeof(t_buff));
		tmp->prev = *b;
		*b = tmp;
		pos = (*b)->data;
		r = read(fd, (*b)->data, BUFF_SIZE);
		(*b)->len = r > 0 ? r : 0;
	}
	*line = malloc(len + (pos - (*b)->data) + 1);
	(*line)[len + (pos - (*b)->data)] = 0;
	len = get_next_line_cpy(b, line, len, pos);
	return (r < 0 ? -1 : len);
}
