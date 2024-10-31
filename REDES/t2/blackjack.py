import sys
import time
import json
from src.player import setup_sockets, send_message, receive_message

def main():
    player_id = int(sys.argv[1])
    receive_socket, transmit_socket, next_ip, next_port = setup_sockets(player_id)


    # Player 0 inicia com o bastão e mensagem vazia
    if player_id == 0:
        message = {
            "Origem": player_id,
            "Tipo": "BASTAO",
            "Dados": "!!!BASTAO!!!"
        }

        # Primeira mensagem da execução, solta o bastão na rede
        send_message(transmit_socket, json.dumps(message), next_ip, next_port, player_id)
        print(f"Primeira mensagem enviada: {message['Dados']}")

    # Sempre rodando, pronto para receber mensagens
    while True:
        print("entrou no loop.")
        # Recebe a mensagem e converte de volta para dicionario
        received_data = receive_message(receive_socket, player_id)
        message = json.loads(received_data)
        print(f"Player {player_id}: {message['Dados']}")

        # AQUI: Insere/altera/remove algo da mensagem
        
        # COMENTADO SÓ PARA TESTE INICIAL
        # if message["Origem"] == player_id:
        #     # Se a mensagem chegou de volta a origem, retira da rede
        #     print(f"Player {player_id} recebeu de volta sua mensagem: {message}")
        # else:
        #     # Não é dele, passa a mensagem adiante
        #     send_message(transmit_socket, str(message), next_ip, next_port, player_id)
        
        # Sempre reenvia a mensagem que recebe

        send_message(transmit_socket, json.dumps(message), next_ip, next_port, player_id)
        time.sleep(1)
        
main() 