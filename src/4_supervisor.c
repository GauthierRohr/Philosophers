/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_supervisor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:56 by grohr             #+#    #+#             */
/*   Updated: 2025/04/19 18:59:07 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//check_death:
//- Vérifie si un philosophe est mort (temps sans manger > time_to_die).
//- Si mort, arrête la simulation et affiche "died".
//- Utilise death_mutex pour protéger l'accès au temps du dernier repas.
//- Retourne 1 si mort, 0 sinon.
//
int	check_death(t_data *data, int i)
{
	pthread_mutex_lock(&data->death_mutex);
	if (get_time() - data->philos[i].last_meal > data->time_to_die)
	{
		print_msg(&data->philos[i], "died");
		data->simulation_stop = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

//check_all_ate:
//- Vérifie si tous les philosophes ont mangé max_meals fois.
//- Retourne 1 si tous ont fini, 0 sinon.
//- Utilise death_mutex pour protéger l'accès au compteur de repas.
//
int	check_all_ate(t_data *data)
{
	int	i;

	if (data->max_meals <= 0)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->philos[i].meals_eaten < data->max_meals)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return (0);
		}
		pthread_mutex_unlock(&data->death_mutex);
		i++;
	}
	return (1);
}

//supervisor_routine:
//- Routine du thread superviseur.
//- Vérifie en boucle si un philosophe est mort ou si tous ont fini de manger.
//- Arrête la simulation en cas de mort ou de repas complets.
//
void	*supervisor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (check_death(data, i))
				return (NULL);
			i++;
		}
		if (check_all_ate(data))
		{
			pthread_mutex_lock(&data->death_mutex);
			data->simulation_stop = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
