/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 09:37:06 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/11/19 13:33:07 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>

# define BUFF_SIZE 4096

int				get_next_line(const int fd, char **line);

typedef struct	s_buff {
	char			data[BUFF_SIZE];
	size_t			len;
	struct s_buff	*prev;
}				t_buff;

t_buff			**get_next_line_buff(const int fd);

#endif
