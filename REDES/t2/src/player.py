import socket
import json

PORTS = [2000, 2001, 2002, 2003]
LOCAL_IP = "127.0.0.1"
NEXT_IP = "127.0.0.1"

NUM_PLAYERS = 4
DEALER_ID = 0
START_MONEY = 1000

class Player:
    def __init__(self):
        self.money = START_MONEY
        self.hand = {}
        self.players_hands = {}

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
    # print(f"Enviando para {next_port}: {message}\n")
    transmit_socket.sendto(json.dumps(message).encode(), (next_ip, next_port))

def receive_message(receive_socket):
    data, addr = receive_socket.recvfrom(1024)
    message = json.loads(data.decode())

    # print(f"Recebeu {message}\n")
    return message

def player_process(
        player_id, player, 
        transmit_socket, next_ip, next_port, 
        message):
    match message["type"]:

        case "players-bet":
            # bet = int(input("Quanto deseja apostar?\n")) Comentado só p acelerar testes
            bet = 10

            if bet > player.money:
                input("Você não tem tanto dinheiro. Insira novamente:\n")

            message["data"][player_id] = bet
            player.money = player.money - bet

            message["acks"][player_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)

            return

        case "distribute-cards":
            player.hand = message["data"][str(player_id)]
            player.players_hands = message["data"]

            print(f"Cartas do Dealer: {message["data"][str(DEALER_ID)][0]}\n")

            message["acks"][player_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)

            return

    return
