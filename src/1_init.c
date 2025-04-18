/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:48:44 by grohr             #+#    #+#             */
/*   Updated: 2025/04/18 19:13:35 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//init_philos:
//- Alloue et configure les structures des philosophes.
//- Associe les fourchettes gauche et droite à chaque philosophe.
//- Initialise le temps du dernier repas et le compteur de repas.
//- Retourne 1 en cas d'erreur, 0 sinon.
//
int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].last_meal = get_time();
		data->philos[i].meals_eaten = 0;
		data->philos[i].data = data;
		i++;
	}
	return (1);
}

//init_mutexes:
//- Alloue et initialise mutexes pour les fourchettes avec pthread_mutex_init.
//- Initialise les mutexes pour les logs et la vérification des morts.
//- pthread_mutex_init crée un mutex pour synchroniser l'accès aux ressources
//  partagées, empêchant les data races.
//- Retourne 1 en cas d'erreur, 0 sinon.
//
int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->log_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&data->death_mutex, NULL))
		return (0);
	return (1);
}

//parse_args:
//- Vérifie et convertit les arguments en paramètres de simulation.
//- Stocke nombre de philo, temps de mort, repas, sommeil, et repas max.
//- Retourne 0 si les arguments sont invalides, 1 sinon.
//
int	init_parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		return (0);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->max_meals = -1;
	if (argc == 6)
		data->max_meals = ft_atoi(argv[5]);
	if (data->num_philos < 1
		|| data->time_to_die < 0
		|| data->time_to_eat < 0
		|| data->time_to_sleep < 0
		|| (argc == 6 && data->max_meals < 0))
		return (0);
	return (1);
}
