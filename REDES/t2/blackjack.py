# blACKjACK
import sys
import time
from src.player import setup_sockets, send_message, receive_message, player_process
from src.dealer import Dealer, dealer_process

def main():
    player_id = int(sys.argv[1])
    receive_socket, transmit_socket, next_ip, next_port = setup_sockets(player_id)

    # Inicializa variaveis do jogador
    money = 1000
    hand = {}

    # Mensagem em que o Dealer inicializa a rede
    if player_id == 0:
        input("Aperte alguma tecla para iniciar: \n")

        dealer = Dealer()
        bets = [None] * 4

        message = {
            "type": "players-bet",
            "from": player_id,
            "data": bets,
            "acks": [1, 0, 0, 0]
        }

        # Manda mensagem pedindo as apostas de cada jogador
        send_message(transmit_socket, next_ip, next_port, message)

    # Loop principal do jogo
    while True:
        message = receive_message(receive_socket)

        if message["from"] == player_id: # DEALER
            dealer_process(dealer, transmit_socket, next_ip, next_port, message)

        else: # PLAYER
            match message["type"]:
                case "players-bet":
                    money = player_process(player_id, money, hand, transmit_socket, next_ip, next_port, message)
                    print(f"Seu dinheiro atual:{money}\n")
                case "distribute-cards":
                    hand = player_process(player_id, money, hand, transmit_socket, next_ip, next_port, message)
                    print(hand)



main()
