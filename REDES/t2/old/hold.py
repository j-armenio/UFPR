# Retorna uma lista com os jogadores que ganharam um blackjack 'natural'
def verify_blackjack(players_hand):
    blackjack_players = []

    for player_id, hand in players_hand.items():
        # Calcula a pontuação inicial assumindo que todos os Ases valem 11
        points = sum(int(card['points']) for card in hand)

        # Conta quantos Ases existem na mão
        n_ases = sum(1 for card in hand if card['value'] == 'A')

        # Ajusta a pontuação, mudando o valor dos Ases de 11 para 1, se necessário
        while points > 21 and n_ases > 0:
            points -= 10
            n_ases -= 1

        # Verifica se a soma é 21 com exatamente duas cartas
        if points == 21 and len(hand) == 2:
            blackjack_players.append(player_id)

    return blackjack_players

# Agora o dealer precisa checar se houve algum blackjack
blackjack_players = verify_blackjack(dealer.player_hands)

# 1. CASO BLACKJACK NATURAL
if (blackjack_players):
    print(f"Player que fizeram Blackjack: {blackjack_players}\n")

    # Dealer checa SECRETAMENTE suas cartas

    dealer_points = sum_points(dealer.dealer_hand)

    while dealer_points < 17:
        new_card = dealer.deck.pop()
        dealer.dealer_hand.append(new_card)
        dealer_points = sum_points(dealer.dealer_hand)

    print(f"Mão do dealer após saques: {dealer.dealer_hand}")
    
    if (dealer_points == 21): # EMPATE: revela a mão e encerra a rodada com um empate para qualquer jogador com bj, e derrota para os demais
        message["type"] = "result-payment"
        message["data"] = [("LOSE", 0)] * NUM_PLAYERS # (result, payment)

        for player in blackjack_players:
            message["data"][player] = ("TIE", dealer.bets[player])

    else: # Dealer não tem 21, bj natural ganha, resto continua jogando
        if dealer_points > 21: # ESTOROU
            print("Dealer estorou!\n")
        elif dealer_points < 21: # PERDEU
            print("Dealer perdeu!\n")

        message["type"] = "result-payment"
        

# 2. CASO NORMAL
# else:

# Dealer tem que desvirar a carta dele e jogar
