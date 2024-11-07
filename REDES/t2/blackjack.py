# blACKjACK
import sys
import time
from src.player import Player, setup_sockets, send_message, receive_message, player_process
from src.dealer import Dealer, dealer_process
from src.utils import debug_print

def start_round(dealer, transmit_socket, next_ip, next_port):
        message = {
        "type": "inform-dealer",
        "data": dealer.dealer_id,
        "from": dealer.dealer_id,
        "acks": [0] * 4
        }
        message["acks"][dealer.dealer_id] = 1
        
        print("[DEALER] Você é o Dealer!")
        print("[DEALER] Enviando aviso de quem é o Dealer aos outros jogadores.\n")
        send_message(transmit_socket, next_ip, next_port, message)

def main():
    PLAYER_ID = int(sys.argv[1])

    receive_socket, transmit_socket, next_ip, next_port = setup_sockets(PLAYER_ID)

    # Inicializa objeto Player e Dealer dentro de todos jogadores. 
    player = Player()
    dealer = Dealer()

    # Pode ter os estados: 'STARTING', 'ENDING' e 'RUNNING'
    game_state = 'RUNNING'

    # Ações do Dealer inicial, deve inicializar a rede
    if len(sys.argv) > 2 and sys.argv[2] == "-d":   
        player.dealer_id = PLAYER_ID
        dealer.dealer_id = PLAYER_ID

        input("Aperte alguma tecla para iniciar: \n")
        print("============================ LOG DO ROUND ============================\n")

        start_round(dealer, transmit_socket, next_ip, next_port)

    # Loop principal do jogo
    while True:
        debug_print(game_state)
        debug_print(f"player.dealer_id: {player.dealer_id}, dealer.dealer_id: {dealer.dealer_id}")

        if game_state == 'RUNNING':
            if dealer.dealer_id == PLAYER_ID:
                print("[DEALER] Aguardando...\n")
            else:
                print("Aguardando...\n")

            message = receive_message(receive_socket)

            # input("PRESSIONE PARA CONTINUAR...")
            print("Recebi mensagem.\n")
            time.sleep(2)

        elif game_state == 'STARTING': # Só é usado quando Dealer é passado
            dealer.dealer_id = player.dealer_id
            start_round(dealer, transmit_socket, next_ip, next_port)
            game_state = 'RUNNING'

        if dealer.dealer_id == PLAYER_ID: # DEALER
            debug_print("DEALER PROCESS\n")
            game_state = dealer_process(game_state, dealer, player, transmit_socket, next_ip, next_port, message)
        else:                             # PLAYER
            debug_print("PLAYER PROCESS\n")
            game_state = player_process(game_state, PLAYER_ID, player, dealer, transmit_socket, next_ip, next_port, message)

if __name__ == "__main__":
    main()
