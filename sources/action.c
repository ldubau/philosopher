/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonpouet <leonpouet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 14:09:03 by ldubau            #+#    #+#             */
/*   Updated: 2026/10/04 11:52:37 by leonpouet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	meal(t_philo *philo, t_mtx *first, t_mtx *second)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->philo_mutex);
	mtx_printf(philo, "is eating");
	ft_usleep(philo->table->time_to_eat);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->nbr_meal++;
	if (philo->nbr_meal == philo->table->max_meals)
		philo->full = true;
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
}

void	eat(t_philo *philo)
{
	t_mtx	*first;
	t_mtx	*second;

	if (philo->id % 2 == 1)
	{
		first = &philo->right_fork->fork;
		second = &philo->left_fork->fork;
	}
	else
	{
		first = &philo->left_fork->fork;
		second = &philo->right_fork->fork;
	}
	pthread_mutex_lock(first);
	mtx_printf(philo, "has taken a fork");
	pthread_mutex_lock(second);
	mtx_printf(philo, "has taken a fork");
	meal(philo, first, second);
}

void	go_sleep(t_philo *philo)
{
	mtx_printf(philo, "is sleeping");
	ft_usleep(philo->table->time_to_sleep);
}

void	think(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	mtx_printf(philo, "is thinking");
	if (table->time_to_eat * 2 - table->time_to_sleep
		&& table->philo_nbr % 2 == 1)
		ft_usleep(table->time_to_eat * 2 - table->time_to_sleep);
}
