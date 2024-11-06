# Dijkstra/Uniform-cost Search - Busca de Custo Uniforme

import heapq

def backtrace(parent, start, end):
    path = [end]

    # vai adicionando o pai do ultimo elemento
    while path[-1] != start:
        path.append(parent[path[-1]])

    path.reverse()
    return path

# Retorna verdadeiro se o child esta na borda ou no explorados
def check_border_and_explored(child, border, explored):
    # verifica fila
    for i in border:
        # verifica se a borda esta vazia
        if isinstance(i, tuple):
            _, n = i

            if n.board == child.board:
                return False
        
    # verifica conjunto
    for n in explored:
        if n.board == child.board:
            return False
    
    return True

# Verifica se existe um no na borda com custo maior, se existir, substitui pelo novo
def check_border_cost(child, border):
    for i, (cost, node) in enumerate(border):
        if node.board == child.board:
            if child.cost < cost:
                # remove nó antigo e adiciona novo
                border[i] = (child.cost, child)
                # reordena a heap
                heapq.heapify(border)
            return        
    return 

def ucs(problem):
    node = problem.initial_state
    if problem.objective_test(node):
        return [node]

    border = []
    heapq.heappush(border, (node.cost, node))
    explored = set()
    parent = {}

    while border:
        _, node = heapq.heappop(border)

        if problem.objective_test(node):
            return backtrace(parent, problem.initial_state, node)
            
        explored.add(node)

        for child in node.get_actions():
            if check_border_and_explored(child, border, explored):
                parent[child] = node
                heapq.heappush(border, (node.cost, child))
            else:
                check_border_cost(child, border)

    return None
