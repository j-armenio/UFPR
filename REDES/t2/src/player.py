import socket

# Configuração de IP e portas
ports = [5001, 5002, 5003, 5004]
local_ip = "127.0.0.1"

# Recebe um id e retorna as informações da rede do jogador equivalente
def setup_sockets(player_id):
    local_receive_port = ports[player_id]
    next_ip = "127.0.0.1"
    next_port = ports[(player_id + 1) % len(ports)] # Garante que 3 aponta para 0

    # Criação dos sockets de recebimento e transmissão
    receive_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    receive_socket.bind((local_ip, local_receive_port))

    transmit_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    return receive_socket, transmit_socket, next_ip, next_port

def send_message(transmit_socket, message, next_ip, next_port, player_id):
    print(f"{message}, {next_ip}, {next_port}, {player_id}")
    transmit_socket.sendto(message.encode(), (next_ip, next_port))

def receive_message(receive_socket, player_id):
    data, addr = receive_socket.recvfrom(1024)
    message = data.decode()
    return message
