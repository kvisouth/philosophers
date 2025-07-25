/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:51:01 by kevso             #+#    #+#             */
/*   Updated: 2025/07/24 20:24:01 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_arg
{
	int						nb_philo;
	int						time2die;
	int						time2eat;
	int						time2sleep;
	int						meal2eat;
	int						satiated_philos;
	long int				time_start;
	int						flag;
	pthread_mutex_t			mtx_print_status;
	pthread_mutex_t			mtx_print;
	pthread_mutex_t			mtx_flag;
	pthread_mutex_t			mtx_time_eat;
	pthread_mutex_t			mtx_finish;
}							t_info;

typedef struct s_philo
{
	int						id;
	int						finish;
	int						meals_eaten;
	long int				last_eat;
	pthread_t				thread_id;
	pthread_mutex_t			*right_fork;
	pthread_mutex_t			left_fork;
	t_info					*info;
}							t_philo;

typedef struct s_struct
{
	t_philo					*philo;
	t_info					arg;
}							t_struct;

int	ft_atol(const char *str);
int	ft_atoi(const char *str);
int	parsing(int ac, char **av);

#endif