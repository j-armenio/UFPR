# A*
# Implementação igual a da ucs, mas a ordenação é feita por uma f(n)=g(n)+h(n)

# Dijkstra/Uniform-cost Search - Busca de Custo Uniforme

import heapq

def backtrace(parent, start, end):
    path = [end]

    # vai adicionando o pai do ultimo elemento
    while path[-1] != start:
        path.append(parent[path[-1]])

    path.reverse()
    return path

# numero de pecas fora do lugar
def heuristic1(node, problem):
    f = 0

    for i in range(len(node.board)):
        for j in range(len(node.board[i])):
            if node.board[i][j] != 0 and node.board[i][j] != problem.FINAL_STATE[i][j]:
                f+=1

    return f

# soma das distancias das pecas de suas posicoes objetivo (distancia de manhattan)
def heuristic2(node, problem):
    f = 0
    
    for i in range(len(node.board)):
        for j in range(len(node.board[i])):
            value = node.board[i][j]
            if value != 0:  # Ignora o espaço vazio
                # Encontra a posição final da peça atual
                for x in range(len(problem.FINAL_STATE)):
                    for y in range(len(problem.FINAL_STATE[x])):
                        if problem.FINAL_STATE[x][y] == value:
                            # Calcula a distância de Manhattan
                            f += abs(i - x) + abs(j - y)
    
    return f

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
def check_border_cost(child, border, problem):
    for i, (cur_cost, node) in enumerate(border):
        if node.board == child.board:
            child_cost = node.cost + heuristic2(node, problem)
            if child_cost < cur_cost:
                # remove nó antigo e adiciona novo
                border[i] = (child_cost, child)
                # reordena a heap
                heapq.heapify(border)
            return
    return

def a_star(problem):
    node = problem.initial_state
    if problem.objective_test(node):
        return [node]

    border = []
    heapq.heappush(border, (node.cost + heuristic2(node, problem), node))
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
                heapq.heappush(border, (node.cost + heuristic2(child, problem), child))
            else:
                check_border_cost(child, border, problem)

    return None
