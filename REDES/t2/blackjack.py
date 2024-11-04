# blACKjACK
import sys
from src.player import Player, setup_sockets, send_message, receive_message, player_process
from src.dealer import Dealer, dealer_process
from src.utils import print_hand

def main():
    player_id = int(sys.argv[1])
    receive_socket, transmit_socket, next_ip, next_port = setup_sockets(player_id)

    # Incializa objeto do jogador
    player = Player()

    # Mensagem em que o Dealer inicializa a rede
    if player_id == 0:
        input("Aperte alguma tecla para iniciar: \n")
        print("============== LOG DO ROUND ==============\n")

        dealer = Dealer()
        bets = [None] * 4

        message = {
            "type": "players-bet",
            "data": bets,
            "from": player_id,
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
                    player_process(player_id, player, transmit_socket, next_ip, next_port, message)
                    print(f"Seu dinheiro atual:{player.money}\n")

                case "distribute-cards":
                    player_process(player_id, player, transmit_socket, next_ip, next_port, message)
                    print(f"Suas cartas:")
                    print_hand(player.hand)

                case "get-actions":
                    player_process(player_id, player, transmit_socket, next_ip, next_port, message)

                case "result-payment":
                    player_process(player_id, player, transmit_socket, 
                    next_ip, next_port, message)

main()
