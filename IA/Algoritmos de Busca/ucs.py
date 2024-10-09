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
    for n in border:
        if n.board == child.board:
            return False
        
    # verifica conjunto
    for n in explored:
        if n.board == child.board:
            return False

# Verifica se existe um no na borda com custo maior, se existir, substitui pelo novo
def check_border_cost(child, border):
    for i, n in border:
        if n.board == child.board and n.cost > child.cost:
            border[i] = child
            break

def ucs(problem):
    node = problem.initial_state
    if problem.objective_test(node):
        return [node]
    
    border = []
    heapq.heappush(border, (node.cost, node))
    explored = set()
    parent = {}

    while border:
        node = heapq.heappop(border)

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
