/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masolet- <masolet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 14:32:45 by masolet-          #+#    #+#             */
/*   Updated: 2026/09/03 14:57:00 by masolet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
// Safely checks if the simulation has ended (death or meals reached)
*/
bool	is_simulation_stopped(t_data *data)
{
	bool	stopped;

	pthread_mutex_lock(&data->data_mutex);
	stopped = data->is_ended;
	pthread_mutex_unlock(&data->data_mutex);
	return (stopped);
}

/*
// Busy waits until the main thread signals that all threads are created
*/
bool	wait_for_start(t_data *data)
{
	while (1)
	{
		pthread_mutex_lock(&data->data_mutex);
		if (data->is_started)
		{
			pthread_mutex_unlock(&data->data_mutex);
			return (true);
		}
		if (data->is_ended)
		{
			pthread_mutex_unlock(&data->data_mutex);
			return (false);
		}
		pthread_mutex_unlock(&data->data_mutex);
		usleep(200);
	}
}

/*
// Checks if a single philosopher has starved to death
*/
static int	check_philo_death(t_data *data, int i)
{
	pthread_mutex_lock(&data->philos[i].state_mutex);
	if (get_current_time() - data->philos[i].last_meal_ms >= data->time_to_die
		&& data->philos[i].is_alive)
	{
		pthread_mutex_lock(&data->write_mutex);
		pthread_mutex_lock(&data->data_mutex);
		data->is_ended = true;
		pthread_mutex_unlock(&data->data_mutex);
		printf("%ld %d %s", get_elapsed_time(data->start_time),
			data->philos[i].id + 1, "died\n");
		pthread_mutex_unlock(&data->write_mutex);
		pthread_mutex_unlock(&data->philos[i].state_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->philos[i].state_mutex);
	return (0);
}

/*
// Continuously monitors all philosophers for death or completion
*/
void	monitor_routine(t_data *data)
{
	int	i;

	i = 0;
	while (!is_simulation_stopped(data))
	{
		pthread_mutex_lock(&data->data_mutex);
		if (data->finished_count >= data->philo_count)
		{
			data->is_ended = true;
			pthread_mutex_unlock(&data->data_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->data_mutex);
		if (check_philo_death(data, i))
			break ;
		i = (i + 1) % data->philo_count;
		if (i == 0)
			usleep(1000);
	}
}
