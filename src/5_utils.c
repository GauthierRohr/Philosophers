/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:58 by grohr             #+#    #+#             */
/*   Updated: 2025/04/18 19:07:50 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//get_time:
//- Récupère le temps actuel en millisecondes avec gettimeofday.
//- gettimeofday fournit heure (microseconde) pour suivre les timestamps
//- Retourne le temps en millisecondes.
//
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

//print_log:
//- Affiche un message d'état avec le timestamp et l'ID du philosophe.
//- Protège l'affichage avec log_mutex pour éviter les chevauchements.
//- N'affiche rien si la simulation est arrêtée.
//
void	print_log(t_philo *philo, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->log_mutex);
	if (!philo->data->simulation_stop)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->log_mutex);
}

//precise_sleep:
//- Attend un temps donné en millisecondes avec usleep.
//- Utilise une boucle pour une précision accrue, car usleep seul peut être
//  imprécis sur certains systèmes.
//
void	precise_sleep(long ms)
{
	long	start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(100);
}

int	ft_atoi(char *str)
{
	long	res;

	res = 0;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res);
}
