/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevso <kevso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:51:01 by kevso             #+#    #+#             */
/*   Updated: 2025/02/25 15:27:41 by kevso            ###   ########.fr       */
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

int	ft_atol(const char *str);
int	ft_atoi(const char *str);
int	parsing(int ac, char **av);

#endif