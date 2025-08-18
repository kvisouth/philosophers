/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevisout <kevisout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:06:24 by kevisout          #+#    #+#             */
/*   Updated: 2025/08/16 22:23:25 by kevisout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	time_now(void)
{
	long int			time;
	struct timeval		current_time;

	time = 0;
	if (gettimeofday(&current_time, NULL) == -1)
		return (0);
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}

void	init_arg(int argc, char **argv, t_struct *st)
{
	st->arg.nb_philo = ft_atoi(argv[1]);
	st->arg.time2die = ft_atoi(argv[2]);
	st->arg.time2eat = ft_atoi(argv[3]);
	st->arg.time2sleep = ft_atoi(argv[4]);
	st->arg.meals_to_eat = -1;
	if (argc == 6)
		st->arg.meals_to_eat = ft_atoi(argv[5]);
}

int	check_death(t_philo *philo, int i)
{
	int	flag_value;

	pthread_mutex_lock(&philo->sarg->mtx_flag);
	if (i != 0)
		philo->sarg->flag = i;
	flag_value = philo->sarg->flag;
	pthread_mutex_unlock(&philo->sarg->mtx_flag);
	if (flag_value != 0)
		return (1);
	return (0);
}

int	check_death_iter(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->sarg->nb_philo)
	{
		pthread_mutex_lock(&philo->sarg->mtx_flag);
		if (philo->sarg->flag != 0)
		{
			pthread_mutex_unlock(&philo->sarg->mtx_flag);
			return (1);
		}
		pthread_mutex_unlock(&philo->sarg->mtx_flag);
		i++;
	}
	return (0);
}

void	print_status(char *str, t_philo *philo)
{
	long int		time;

	time = time_now() - philo->sarg->time_start;
	if (!check_death(philo, 0))
	{
		pthread_mutex_lock(&philo->sarg->mtx_print_status);
		printf("%ld %d %s", time, philo->id, str);
		pthread_mutex_unlock(&philo->sarg->mtx_print_status);
	}
}

void	init_mutex(t_struct *st)
{
	pthread_mutex_init(&st->arg.mtx_print_status, NULL);
	pthread_mutex_init(&st->arg.mtx_print, NULL);
	pthread_mutex_init(&st->arg.mtx_flag, NULL);
	pthread_mutex_init(&st->arg.mtx_time_eat, NULL);
	pthread_mutex_init(&st->arg.mtx_finish, NULL);
}

int	init_philo(t_struct *st)
{
	int	i;

	i = 0;
	st->arg.time_start = time_now();
	st->arg.flag = 0;
	st->arg.satiated_philos = 0;
	init_mutex(st);
	while (i < st->arg.nb_philo)
	{
		st->philo[i].id = i + 1;
		st->philo[i].last_eat = st->arg.time_start;
		st->philo[i].meals_eaten = 0;
		st->philo[i].finish = 0;
		st->philo[i].right_fork = NULL;
		pthread_mutex_init(&st->philo[i].left_fork, NULL);
		if (st->arg.nb_philo == 1)
			return (1);
		if (i == st->arg.nb_philo - 1)
			st->philo[i].right_fork = &st->philo[0].left_fork;
		else
			st->philo[i].right_fork = &st->philo[i + 1].left_fork;
		i++;
	}
	return (1);
}

void	free_all(t_struct *st)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&st->arg.mtx_flag);
	while (st->arg.flag == 0)
	{
		pthread_mutex_unlock(&st->arg.mtx_flag);
		ft_usleep(1);
		pthread_mutex_lock(&st->arg.mtx_flag);
	}
	pthread_mutex_unlock(&st->arg.mtx_flag);
	while (i < st->arg.nb_philo)
	{
		pthread_join(st->philo[i].thread_id, NULL);
		i++;
	}
	pthread_mutex_destroy(&st->arg.mtx_print);
	i = 0;
	while (i < st->arg.nb_philo)
	{
		pthread_mutex_destroy(&st->philo[i].left_fork);
		i++;
	}
	free(st->philo);
}

void	dying(t_philo *philo)
{
	pthread_mutex_unlock(&philo->sarg->mtx_time_eat);
	pthread_mutex_unlock(&philo->sarg->mtx_finish);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	print_status("died 💀💀 👎😹👎 😂🤣\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	check_death(philo, 1);
	pthread_mutex_lock(&philo->sarg->mtx_time_eat);
	pthread_mutex_lock(&philo->sarg->mtx_finish);
}

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_death_iter(philo))
		print_status("is thinking\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_death_iter(philo))
		print_status("is sleeping\n", philo);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	ft_usleep(philo->sarg->time2sleep);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(&philo->sarg->mtx_print);
	if (!check_death_iter(philo))
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
	philo->last_eat = time_now();
	pthread_mutex_unlock(&philo->sarg->mtx_time_eat);
	pthread_mutex_unlock(&philo->sarg->mtx_print);
	ft_usleep(philo->sarg->time2eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void	routine(t_philo *philo)
{
	eating(philo);
	sleeping(philo);
	thinking(philo);
}

void	monitor_flagger(t_struct *st, t_philo *philo)
{
	pthread_mutex_unlock(&philo->sarg->mtx_flag);
	pthread_mutex_lock(&st->arg.mtx_time_eat);
	pthread_mutex_lock(&st->arg.mtx_finish);
	if (!check_death(philo, 0) && !philo->finish
		&& (time_now() - philo->last_eat)
		>= (long)(st->arg.time2die))
	{
		dying(philo);
	}
	pthread_mutex_unlock(&st->arg.mtx_time_eat);
	pthread_mutex_unlock(&st->arg.mtx_finish);
}

void	*monitoring(void *data)
{
	t_struct	*st;
	t_philo		*philo;
	int			i;

	st = (t_struct *)data;
	i = 0;
	while (i < st->arg.nb_philo)
	{
		philo = &st->philo[i];
		while (1)
		{
			ft_usleep(10);
			pthread_mutex_lock(&philo->sarg->mtx_flag);
			if (philo->sarg->flag == 0)
				monitor_flagger(st, philo);
			else
			{
				pthread_mutex_unlock(&philo->sarg->mtx_flag);
				break ;
			}
		}
		i++;
	}
	return (0);
}

void	*phithread(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(philo->sarg->time2eat / 10);
	while (!check_death(philo, 0))
	{
		routine(philo);
		if (++philo->meals_eaten == philo->sarg->meals_to_eat)
		{
			pthread_mutex_lock(&philo->sarg->mtx_finish);
			philo->finish = 1;
			philo->sarg->satiated_philos++;
			if (philo->sarg->satiated_philos == philo->sarg->nb_philo)
			{
				pthread_mutex_unlock(&philo->sarg->mtx_finish);
				check_death(philo, 2);
				pthread_mutex_lock(&philo->sarg->mtx_finish);
			}
			pthread_mutex_unlock(&philo->sarg->mtx_finish);
			return (0);
		}
	}
	return (0);
}

int	threading(t_struct *st)
{
	int			i;
	pthread_t	monithread;

	i = 0;
	while (i < st->arg.nb_philo)
	{
		st->philo[i].sarg = &st->arg;
		if (pthread_create(&st->philo[i].thread_id, NULL,
				phithread, &st->philo[i]) != 0)
			return (0);
		i++;
	}
	pthread_create(&monithread, NULL, monitoring, &st->philo);
	pthread_join(monithread, NULL);
	return (1);
}

int	main(int ac, char **av)
{
	t_struct		st;

	if (parse_args(ac, av) == 0)
		return (0);
	init_arg(ac, av, &st);
	st.philo = malloc(sizeof(t_philo) * st.arg.nb_philo);
	if (!st.philo)
		return (0);
	init_philo(&st);
	if (!threading(&st))
		return (free(st.philo), 0);
	return (free_all(&st), 0);
}
