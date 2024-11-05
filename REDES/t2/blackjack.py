# blACKjACK
import sys
from src.player import Player, setup_sockets, send_message, receive_message, player_process
from src.dealer import Dealer, dealer_process
from src.utils import print_hand

def main():
    player_id = int(sys.argv[1])

    dealer_id = -1
    if len(sys.argv) > 2 and sys.argv[2] == "-d":
        dealer_id = player_id

    receive_socket, transmit_socket, next_ip, next_port = setup_sockets(player_id)

    # Incializa objeto do jogador e do dealer
    player = Player()
    dealer = Dealer()

    # Mensagem em que o Dealer inicializa a rede
    if player_id == dealer_id:
        input("Aperte alguma tecla para iniciar: \n")
        print("============== LOG DO ROUND ==============\n")

        dealer.dealer_id = dealer_id

        message = {
            "type": "inform-dealer",
            "data": dealer_id,
            "from": dealer_id,
            "acks": [0, 0, 0, 0]
        }
        message["acks"][dealer_id] = 1

        # Manda mensagem pedindo as apostas de cada jogador
        send_message(transmit_socket, next_ip, next_port, message)

    # Loop principal do jogo
    while True:
        if not player.f_dealer_round:
            print("Aguardando...")
            message = receive_message(receive_socket)

        if dealer.dealer_id == player_id: # DEALER
            dealer_process(dealer, transmit_socket, next_ip, next_port, message)

        else: # PLAYER
            
            match message["type"]:

                case "inform-dealer":
                    player_process(player_id, player, transmit_socket, next_ip, next_port, message)
                    print(f"ID do Dealer guardado: {player.dealer_id}\n")

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

while True:
    main()
