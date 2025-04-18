/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:58 by grohr             #+#    #+#             */
/*   Updated: 2025/04/19 18:59:40 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//get_time:
//- Récupère le temps actuel
//- gettimeofday fournit le temps passe en :
//	 	-en secondes;
//		- + les restes de micro secs ecoulees;
// On converti le temps en MILLIsecondes (ce que l'enonce nous demandait quoi)
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
void	print_msg(t_philo *philo, char *msg)
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

int ft_atoi(char *str)
{
    long res = 0;

    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    while (*str >= '0' && *str <= '9')
	{
        res = res * 10 + (*str - '0');
        str++;
    }
    if (*str != '\0')
        return (-1);
    if (res > 2147483647)
        return (-1);
    return ((int)res);
}
