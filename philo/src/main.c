/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masolet- <masolet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 14:32:41 by masolet-          #+#    #+#             */
/*   Updated: 2026/09/03 14:56:22 by masolet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
// Creates a thread for each philosopher and starts the simulation
*/
static int	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_current_time();
	while (i < data->philo_count)
	{
		data->philos[i].last_meal_ms = data->start_time;
		if (pthread_create(&data->philos[i].thread_id, NULL,
				philosopher_routine, &data->philos[i]) != 0)
			return (i);
		i++;
	}
	pthread_mutex_lock(&data->data_mutex);
	data->is_started = true;
	pthread_mutex_unlock(&data->data_mutex);
	return (-1);
}

/*
// Waits for all philosopher threads to finish their execution
*/
static void	join_threads(t_data *data, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		i++;
	}
}

/* 
// Main entry point: initializes data, starts simulation, 
// and monitors until end
*/
int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	if (!init_data(&data, ac, av))
		return (write(2, "Error: problem during init\n", 27), 1);
	i = start_simulation(&data);
	if (i != -1)
	{
		pthread_mutex_lock(&data.data_mutex);
		data.is_ended = true;
		pthread_mutex_unlock(&data.data_mutex);
		join_threads(&data, i);
		cleanup_data(&data);
		return (1);
	}
	monitor_routine(&data);
	join_threads(&data, data.philo_count);
	cleanup_data(&data);
	return (0);
}
