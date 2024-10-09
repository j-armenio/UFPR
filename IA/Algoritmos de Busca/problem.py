import copy

class Problem:
    FINAL_STATE = [
        [0, 1, 2],
        [3, 4, 5],
        [6, 7, 8]
    ]

    def __init__(self, initial_state):
        self.initial_state = initial_state

    def objective_test(self, state):
        return state.board == Problem.FINAL_STATE

class Node:
    def __init__(self, board, cost=0):
        self.board = board
        self.cost = cost

    def __lt__(self, other):
        return self.cost < other.cost

    def __str__(self):
        board_str = ''
        for i in self.board:
            board_str += ' '.join(str(n) for n in i) + '\n'
        return board_str.strip()
    
    def get_actions(self):
        board = self.board
        zero_pos = None

        for i in range(len(board)):
            for j in range(len(board[i])):
                if board[i][j] == 0:
                    zero_pos = (i, j)
                    break
            if zero_pos:
                break

        actions = []
        directions = {
            "up": (-1, 0),
            "down": (1, 0),
            "left": (0, -1),
            "right": (0, 1)
        }

        for direction in directions.values():
            new_i = zero_pos[0] + direction[0]
            new_j = zero_pos[1] + direction[1]

            # Verifica se o movimento é válido
            if 0 <= new_i < len(board) and 0 <= new_j < len(board[0]):
                new_board = [row[:] for row in board]
                new_board[zero_pos[0]][zero_pos[1]] = new_board[new_i][new_j]
                new_board[new_i][new_j] = 0

                new_cost = self.cost.copy()
                new_cost += 1

                actions.append(Node(new_board, new_cost))

        return actions
