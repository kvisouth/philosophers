/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:14:33 by kevisout          #+#    #+#             */
/*   Updated: 2025/08/19 14:42:09 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	dying(t_philo *philo)
{
	pthread_mutex_unlock(&philo->sarg->mtx_time_eat);
	pthread_mutex_unlock(&philo->sarg->mtx_finish);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	print_status("died\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	check_death(philo, 1);
	pthread_mutex_lock(&philo->sarg->mtx_time_eat);
	pthread_mutex_lock(&philo->sarg->mtx_finish);
}

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_stop_flag(philo))
		print_status("is thinking\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_stop_flag(philo))
		print_status("is sleeping\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	ft_usleep(philo->sarg->time2sleep);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_stop_flag(philo))
		print_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	if (!philo->right_fork)
	{
		ft_usleep(philo->sarg->time2die * 2);
		if (philo->sarg->nb_philo == 1)
			pthread_mutex_unlock(&philo->left_fork);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	print_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	print_status("is eating\n", philo);
	pthread_mutex_lock(&philo->sarg->mtx_time_eat);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->sarg->mtx_time_eat);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	ft_usleep(philo->sarg->time2eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void	print_status(char *str, t_philo *philo)
{
	long int		time;

	time = get_time_in_ms() - philo->sarg->time_start;
	if (!check_death(philo, 0))
	{
		pthread_mutex_lock(&philo->sarg->mtx_print_status);
		printf("%ld %d %s", time, philo->id, str);
		pthread_mutex_unlock(&philo->sarg->mtx_print_status);
	}
}
