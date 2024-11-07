import socket
import json
from src.utils import sum_points, print_hand, debug_print

PORTS = [2000, 2001, 2002, 2003]
LOCAL_IP = "127.0.0.1"
NEXT_IP = "127.0.0.1"

NUM_PLAYERS = 4
START_MONEY = 1000

class Player:
    def __init__(self):
        self.money = START_MONEY
        self.hand = {}
        self.players_hand = {}
        self.dealer_id = -1

        # flags
        self.f_first_round = 1
        self.f_natural_bj = 0
        self.f_surrender = 0
        self.f_stand = 0

# Retorna as referências de rede de um jogador
def setup_sockets(player_id):
    local_receive_port = PORTS[player_id]
    next_port = PORTS[(player_id + 1) % len(PORTS)]

    # Criação dos sockets de recebimento e transmissão
    receive_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    receive_socket.bind((LOCAL_IP, local_receive_port))

    transmit_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    print(f"Player {player_id} localizado em {local_receive_port}\n")
    return receive_socket, transmit_socket, NEXT_IP, next_port

def send_message(transmit_socket, next_ip, next_port, message):
    debug_print(f"Enviando para {next_port}: {message}\n")
    transmit_socket.sendto(json.dumps(message).encode(), (next_ip, next_port))

def receive_message(receive_socket):
    data, addr = receive_socket.recvfrom(1024)
    message = json.loads(data.decode())

    debug_print(f"Recebi: {message}\n")
    return message

def player_process(
        game_state,
        player_id, player, dealer,
        transmit_socket, next_ip, next_port, 
        message):
    match message["type"]:
        
        case "baston":
            print("Você é o novo Dealer.\n")
            player.dealer_id = player_id
            dealer.dealer_id = player_id

            return 'STARTING'
        
        case "inform-dealer":
            player.dealer_id = message["data"]
            dealer.dealer_id = player.dealer_id

            print(f"O Dealer é o player {dealer.dealer_id}!\n")

            # Resetando as flags para inicio do round
            player.f_first_round = 1
            player.f_natural_bj = 0
            player.f_surrender = 0
            player.f_stand = 0

            message["acks"][player_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)
            return 'RUNNING'

        case "players-bet":
            while True:
                try:
                    bet = float(input("Quanto deseja apostar?\n"))

                    if bet > player.money:
                        print("Você não tem tanto dinheiro assim. Insira novamente.")
                    elif bet < 0:
                        print("Ue, quer ficar devendo mano? Coloca um valor certo.")
                    else:
                        break

                except ValueError:
                    print("Entrada inválida! Por favor, insira um número.")

            message["data"][player_id] = bet
            player.money = player.money - bet

            message["acks"][player_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)
            return 'RUNNING'

        case "distribute-cards":
            player.hand = message["data"][str(player_id)]
            player.players_hand = message["data"]

            print(f"Cartas do Dealer: {message["data"][str(player.dealer_id)][0]["points"]} {message["data"][str(player.dealer_id)][0]["suit"]} XX")

            print("Suas cartas: ")
            print_hand(player.hand)

            # Ativa a flag de blackjack natural
            player_points = sum_points(player.hand)
            if player_points == 21:
                player.f_natural_bj = 1

            message["acks"][player_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)
            return 'RUNNING'
        
        case "get-actions":
            for i, action in enumerate(message["data"]):
                if action[1] is not None: # É uma segundo envio, Dealer respondeu com algo
                    if player_id == i: # É o indice do jogador
                        card = action[1]                        
                        print(f"Você recebeu: {card["points"]} {card["suit"]}\n")
                        
                        if card not in player.hand:
                            player.hand.append(card.copy())
                        
                        # Tava dando bug de ponteiro no append, `not in` resolve
                        if card not in player.players_hand[str(player_id)]:
                            player.players_hand[str(player_id)].append(card.copy())

                        action[1] = None
                        print(f"Mão atual:")
                        print_hand(player.hand)

            if player.f_surrender:
                print("Você se rendeu e não joga até o fim do round.\n")
            elif player.f_stand:
                print("Você deu 'stand' e não joga até o fim do round.\n")

            elif sum_points(player.hand) > 21: # bust
                print("Você estorou! Perdeu.\n")
                message["data"][player_id] = ["BUST", None]

            else:
                if player.f_natural_bj == 1:
                    message["data"][player_id] = ["NATURAL", None]
                    print("Você possui um Blackjack natural, você não joga esse round.\n")
                else:
                    print("Insira o número de sua jogada?")
                    print("1.HIT")
                    print("2.STAND")

                    if player.f_first_round:
                        print("3.SURRENDER\n")
                    
                    while True: 
                        play = input()
                        try:
                            play = int(play)
                            if play == 1:
                                message["data"][player_id] = ["HIT", None]
                                break
                            elif play == 2:
                                message["data"][player_id] = ["STAND", None]
                                player.f_stand = 1
                                break
                            elif play == 3 and player.f_first_round:
                                message["data"][player_id] = ["SURRENDER", None]
                                player.f_surrender = 1
                                break
                            else:
                                print("Opção inválida! Por favor, insira uma das opções.")

                        except ValueError:
                            print("Entrada inválida! Por favor, insira uma das opções.")

            if player.f_first_round:
                player.f_first_round = 0

            message["acks"][player_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)
            return 'RUNNING'
        
        case "result-payment":
            print("Mão final do dealer: ")
            print_hand(message["data"][player.dealer_id])
            print("Sua mão final:")
            print_hand(player.hand)

            print(f"Meu pagamento é {message["data"][player_id]}")
            print("========================================================\n")
        
            message["acks"][player_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)
            return 'RUNNING'        
    return
