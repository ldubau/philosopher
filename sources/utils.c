/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonpouet <leonpouet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 18:19:58 by leonpouet         #+#    #+#             */
/*   Updated: 2026/10/04 11:58:54 by leonpouet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

long	ft_atol(char *str)
{
	long	nbr;
	int		i;

	i = 0;
	nbr = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		nbr *= 10;
		nbr += (str[i] - '0');
		if (nbr > INT_MAX)
			return (-1);
		i++;
	}
	return (nbr);
}

int	is_digit(char *s)
{
	int	i;

	i = 0;
	if (!s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	free_all(t_table *table, int n_philo, int n_fork)
{
	int	i;

	i = 0;
	while (i < n_fork)
	{
		pthread_mutex_destroy(&table->forks[i].fork);
		i++;
	}
	i = 0;
	while (i < n_philo)
	{
		pthread_mutex_destroy(&table->philos[i].philo_mutex);
		i++;
	}
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->sim_mutex);
	if (table->philos)
		free(table->philos);
	if (table->forks)
		free(table->forks);
	return (0);
}
