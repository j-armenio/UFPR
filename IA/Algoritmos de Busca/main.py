# Quebra-cabeça de oito peças

from problem import Problem, Node
from bfs import bfs
from ucs import ucs
from a_star import a_star

initial_state = Node([
    [7, 2, 3],
    [5, 0, 6],
    [8, 4, 1]
])

problem = Problem(initial_state)

solution_path = ucs(problem)

for i in solution_path:
    print(i)
    print("---")
