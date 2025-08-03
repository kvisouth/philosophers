/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:51:01 by kevso             #+#    #+#             */
/*   Updated: 2025/08/03 20:19:02 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_arg
{
	int						nb_philo;
	int						time2die;
	int						time2eat;
	int						time2sleep;
	int						meals_to_eat;
	int						satiated_philos;
	int						flag;
	long int				time_start;
	pthread_mutex_t			mtx_print_status;
	pthread_mutex_t			mtx_print;
	pthread_mutex_t			mtx_flag;
	pthread_mutex_t			mtx_time_eat;
	pthread_mutex_t			mtx_finish;
}							t_arg;

typedef struct s_philo
{
	int						id;
	int						finish;
	int						meals_eaten;
	long int				last_eat;
	pthread_t				thread_id;
	pthread_mutex_t			*right_fork;
	pthread_mutex_t			left_fork;
	t_arg					*sarg;
}							t_philo;

typedef struct s_struct
{
	t_philo					*philo;
	t_arg					arg;
}							t_struct;

#endif