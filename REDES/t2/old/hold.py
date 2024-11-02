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