/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 19:35:09 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/06/09 16:48:44 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

# define BUFFER_SIZE 1024

typedef struct s_list
{
	int				content;
	struct s_list	*next;
}	t_list;

long	ft_atoi(const char *str);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_isdigit(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_isprint(int c);
int		ft_lstsize(t_list *lst);
int		ft_putstri(int fd, char const *s);
int		ft_putchari(int fd, char c);
int		ft_putnbri(int fd, int n);
int		ft_dprintf(int fd, const char *format, ...);
int		ft_putnbrui(int fd, unsigned int n);
int		ft_putnbrhexmin(int fd, int n);
int		ft_putnbrhexmaj(int fd, int n);
int		ft_putptr(int fd, void	*ptr);
int		ft_strcmp(const char *s1, const char *s2);

void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char const *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char const *s, int fd);
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst);
void	ft_lstclear(t_list **lst);
void	ft_bzero(void *s, size_t n);

void	*ft_memcpy(void *dest, const void *src, size_t n);

char	*ft_itoa(int n);
char	*ft_strdup(const char *s1);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*get_next_line(int fd);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2);

char	**ft_split(char const *s, char c);

size_t	ft_strlen(const char *s);

t_list	*ft_lstnew(int content);
t_list	*ft_lstlast(t_list *lst);

#endif
