/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonpouet <leonpouet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:09 by leonpouet         #+#    #+#             */
/*   Updated: 2026/10/03 19:20:41 by leonpouet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
		return (1);
	if (!parsing(&table, av))
		return (1);
	if (!init_data(&table))
		return (1);
	dinner_start(&table);
	free_all(&table, table.philo_nbr, table.philo_nbr); // clean_table / add all mutex
	return (0);
}
