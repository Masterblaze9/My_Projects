from functions import *


def get_city_object(cities, city_name):
    return [city for city in cities if city.getName() == city_name][0]


def uniform_cost_algorithm_build_path(is_append_start_city, current_city, connection_distance, start_city=None, lowest_cost_path=None):
    aux_paths = []
    aux_costs = []
    aux_total_cost = 0
    if is_append_start_city:
        aux_paths.append(start_city.getName())
    else:
        aux_paths = lowest_cost_path["paths"].copy()
        aux_costs = lowest_cost_path["costs"].copy()
        aux_total_cost = lowest_cost_path["total_cost"]

    aux_paths.append(current_city.getName())
    aux_costs.append(int(connection_distance))
    aux_total_cost += int(connection_distance)

    return {"paths": aux_paths, "costs": aux_costs, "total_cost": aux_total_cost}


def uniform_cost_algorithm(cities, start_city, end_city, is_from_a_star=False):
    paths = []
    complete_path = {}
    lowest_cost_path = {}
    latest_lowest_cost_path = {}
    is_first_iteration = True

    while True:
        for connection in start_city.getConnections():
            city = get_city_object(cities, connection["name"])

            paths.append(uniform_cost_algorithm_build_path(paths == [] or is_first_iteration, city,
                         connection["distance"], start_city, lowest_cost_path))

            if city.getName() == end_city.getName():
                if complete_path != {}:
                    if paths[-1]["total_cost"] < complete_path["total_cost"]:
                        complete_path = paths[-1]
                else:
                    complete_path = paths[-1]

        if is_first_iteration:
            is_first_iteration = False

        lowest_cost_path = min(paths, key=lambda path: path["total_cost"] + (
            h(start_city, end_city) if is_from_a_star else 0))
        
        if complete_path != {}:
            if complete_path["total_cost"] < lowest_cost_path["total_cost"]:
                result = (complete_path["paths"], complete_path["costs"])
                complete_path = {}
                paths.clear()
                return result

        if lowest_cost_path != latest_lowest_cost_path or latest_lowest_cost_path == {}:
            latest_lowest_cost_path = lowest_cost_path

            next_city = get_city_object(cities, lowest_cost_path["paths"][-1])
            paths.remove(lowest_cost_path)

            start_city = next_city


def uniform_cost(cities, start_city, end_city):
    return uniform_cost_algorithm(cities, start_city, end_city)


def depth_limited_search(cities, start_city, end_city, depth_limit):
    if depth_limit == 0:
        return [], []

    for connection in start_city.getConnections():
        city = get_city_object(cities, connection["name"])

        if city.getName() == end_city.getName():
            return [start_city.getName(), city.getName()], [int(connection["distance"])]

        path = depth_limited_search(cities, city, end_city, depth_limit - 1)
        if path is not None and path[0] != []:
            return [start_city.getName()] + path[0], [int(connection["distance"])] + path[1]


def greedy_search(cities, start_city, end_city, greedypath=[], greedycost=[]):
    minHeuristic = 99999
    nextcity = None
    nextcitydistance = 0

    greedypath.append(start_city.getName())
    for connection in start_city.getConnections():
        city = get_city_object(cities, connection["name"])

        if city.getName() == end_city.getName():
            greedypath.append(city.getName())
            greedycost.append(connection["distance"])
            result = (greedypath.copy(), greedycost.copy())
            greedypath.clear()
            greedycost.clear()
            return result

        if h(city, end_city) < minHeuristic:
            minHeuristic = h(city, end_city)
            nextcity = city
            nextcitydistance = connection["distance"]

    greedycost.append(nextcitydistance)

    try:
        return greedy_search(cities, nextcity, end_city, greedypath, greedycost)
    except Exception as e:
        return [], []


def a_star(cities, start_city, end_city):
    return uniform_cost_algorithm(cities, start_city, end_city, is_from_a_star=True)
