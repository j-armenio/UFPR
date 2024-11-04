# Soma os pontos de uma mao e os retorna
def sum_points(hand):
    # calcula a soma dos pontos considerando ases como 11
    points = sum(int(card['points']) for card in hand)

    # ajusta o valor dos ases de 11 para 1, se necessário
    n_ases = sum(1 for card in hand if card['value'] == 'A')
    while points > 21 and n_ases > 0:
        points -= 10
        n_ases -= 1

    return points

