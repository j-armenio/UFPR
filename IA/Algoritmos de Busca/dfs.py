# Depth-first Search - Busca em Profundidade
# Igual a bfs, só muda o tipo de fila

from collections import deque

def backtrace(parent, start, end):
    path = [end]

    # vai adicionando o pai do ultimo elemento
    while path[-1] != start:
        path.append(parent[path[-1]])

    path.reverse()
    return path

def check_border_and_explored(child, border, explored):
    # verifica fila
    for n in border:
        if n.board == child.board:
            return False
        
    # verifica conjunto
    for n in explored:
        if n.board == child.board:
            return False

    return True

# Recebe o estado inicial do problema
def dfs(problem):
    node = problem.initial_state
    if problem.objective_test(node):
        return [node]
    
    border = [node]      # fila LIFO
    explored = set()       # conjunto de nos explorados
    parent = {}

    while border:
        node = border.pop()
        explored.add(node)

        for child in node.get_actions():
            if check_border_and_explored(child, border, explored):
                parent[child] = node # guarda o pai

                if problem.objective_test(child):
                    return backtrace(parent, problem.initial_state, child)
                border.append(child)

    return None

    
