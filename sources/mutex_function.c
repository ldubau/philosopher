/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_function.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldubau <ldubau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:54:00 by ldubau            #+#    #+#             */
/*   Updated: 2026/09/29 15:53:30 by ldubau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	mtx_full(t_philo *philo)
{
	bool	ret;

	pthread_mutex_lock(&philo->philo_mutex);
	ret = philo->full;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (ret);
}

bool	mtx_sim(t_table *table)
{
	bool	ret;

	pthread_mutex_lock(&table->sim_mutex);
	ret = table->end_simulation;
	pthread_mutex_unlock(&table->sim_mutex);
	return (ret);
}

void	mtx_printf(t_philo *philo, char *msg)
{
	long	time;

	pthread_mutex_lock(&philo->table->print_mutex);
	if (!mtx_sim(philo->table))
	{
		time = get_time_ms() - philo->table->start_simulation;
		printf("%ld %d %s\n", time, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
}
