/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonpouet <leonpouet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:50:02 by ldubau            #+#    #+#             */
/*   Updated: 2026/09/28 12:15:16 by leonpouet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal = get_time_ms();
	mtx_printf(philo, "is eating");
	ft_usleep(philo->table->time_to_eat);
	philo->nbr_meal++;
	if (philo->nbr_meal == philo->table->max_meals)
		philo->full = true;
	pthread_mutex_unlock(&philo->philo_mutex);

	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
}

void	dinner_start(t_table *table)
{
	int	i;

	i = 0;
	table->start_simulation = get_time_ms();
	while (i < table->philo_nbr)
		table->philos[i++].last_meal = table->start_simulation;
}
