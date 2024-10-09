# BrFS (Breadth-first Search) - Busca em Largura

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
def bfs(problem):
    node = problem.initial_state
    if problem.objective_test(node):
        return [node]
    
    border = deque([node]) # fila FIFO
    explored = set()       # conjunto de nos explorados
    parent = {}
    n=1

    while border:
        node = border.popleft()
        explored.add(node)

        for child in node.get_actions():
            if check_border_and_explored(child, border, explored):
                parent[child] = node # guarda o pai

                if problem.objective_test(child):
                    return backtrace(parent, problem.initial_state, child)
                border.append(child)  

        if n >= 1000000:
            print("ta paia")
            break

    return None
