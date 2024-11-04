import random
import copy
from src.player import NUM_PLAYERS, DEALER_ID, send_message
from src.utils import sum_points

#
# === Estruturas ===
#
# player_hands := dicionario de lista de dicionarios
#   { 
#       0: [{"value": "A", "suit": "♠", "points": "11"}, {"value": "J", "suit": "♠", "points": "10"}],
#       1: [{"value": "K", "suit": "♠", "points": "10"}, {"value": "5", "suit": "♠", "points": "5"}],
#   }
#

class Dealer:
    def __init__(self):
        self.bets = [None] * 4
        self.player_hands = {} # No '0'(dealer), contém as cartas como são vistas pelos jogadores
        self.dealer_hand = []
        self.deck = []

# Retorna 0 se a mensagem está com todos acks ligados e 1 caso tenha algum ack errado
def check_acks(acks):
    not_flag = 0

    for i, value in enumerate(acks):
        if value == 0:
            print(f"A máquina {i} não recebeu a mensagem corretamente.\n")
            not_flag = 1
    
    if not_flag:
        return 1
    else:
        return 0


# Gera um baralho embaralhado
def generate_deck():
    values = {
        "A": 11,  # Ou 1, dependendo da necessidade
        "2": 2, "3": 3, "4": 4, "5": 5, "6": 6, "7": 7, "8": 8, "9": 9,
        "10": 10, "J": 10, "Q": 10, "K": 10
    }
    suits = ["♠", "♣", "♥", "♦"]

    # Cria o baralho como uma lista de dicionários.
    # Cada dicionário tem três chaves: value, suit, points
    deck = [{"value": value, "suit": suit, "points": values[value]} for value in values for suit in suits]

    # Embaralha o baralho
    random.shuffle(deck)

    return deck

# Distribui cartas para cada jogador
def distribute_cards(deck):
    players_cards = {}

    for i in range(0, NUM_PLAYERS):
        # Tira duas cartas para cada jogador
        hand = [deck.pop(), deck.pop()]
        players_cards[i] = hand
    
    # dicionario de listas de dicionarios
    return players_cards

# Retorna uma lista com os jogadores que ganharam um blackjack 'natural'
def verify_blackjack(player_hands):
    blackjack_players = []

    for player_id, hand in player_hands.items():
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

# Função que processa a mensagem recebida. Pode retornar diferentes coisas dependendo do tipo da msg
def dealer_process(
        dealer, 
        transmit_socket, next_ip, next_port, 
        message):
    match message["type"]:

        case "players-bet": # Dealer processa as apostas do 1 round
            check_acks(message["acks"])

            for i, bet in enumerate(message["data"]):
                if bet is not None:
                    dealer.bets[i] = bet
            
            print(f"Apostas recebidas: {dealer.bets}\n")

            # Dealer cria o baralho e distribui as cartas
            dealer.deck = generate_deck()
            player_cards = distribute_cards(dealer.deck) 
            
            # TESTE: forçar blackjack natural para o Dealer
            player_cards[0] = [{"value": "A", "suit": "♠", "points": "11"}, {"value": "3", "suit": "♠", "points": "3"}]

            dealer.player_hands = player_cards
            
            # Insere as duas primeiras cartas para a mao do dealer
            dealer.dealer_hand = copy.deepcopy(dealer.player_hands[DEALER_ID])

            # Remove a segunda carta da visão dos jogadores
            dealer.player_hands[DEALER_ID].pop()

            print(f"Cartas do Dealer: {dealer.dealer_hand}\nA segunda está oculta aos jogadores!\n")

            message["type"] = "distribute-cards"

            # TESTE: BLACKJACK NATURAL
            player_cards[1] = [{"value": "A", "suit": "♠", "points": "11"}, {"value": "J", "suit": "♠", "points": "10"}]

            message["data"] = player_cards
            message["acks"] = [1, 0, 0, 0]

            # Mensagem com quais cartas cada jogador tem
            send_message(transmit_socket, next_ip, next_port, message)
            return

        case "distribute-cards":
            check_acks(message["acks"])
            print("Cartas distribuidas com sucesso.\n")

            message["type"] = "get-actions"
            message["data"] = [(None, None)] * NUM_PLAYERS

            message["acks"] = [1, 0, 0, 0]
            send_message(transmit_socket, next_ip, next_port, message)
            return

        case "get-actions":
            check_acks(message["acks"])

            # data: [ [None, None], ['NATURAL', None], ['STAND', None], ['HIT', None] ] 

            

            print("Processando ações:")
            
            has_hit = 0
            for action in message["data"]:
                if action[0] == "HIT":
                    has_hit = 1

            if has_hit:
                for action in message["data"]:
                    match action[0]:

                            case "HIT":
                                print("HITOU")
                                action[1] = dealer.deck.pop()
                            
                            case "STAND":
                                print("STANDOU")
                            
                            case "SURRENDER":
                                print("SURRENDOU")

                            case "NATURAL":
                                print("NATUROU")

                            case None:
                                print("Ação do Dealer")

            else: # Acabou as escolhas do round, Dealer precisa ver quem venceu
                message["type"] = "result-payment"

                # Tratar cada resultado
                for action in message["data"]:
                    match action[0]:

                        case "STAND":
                            print("Calculando Stand...")
                        
                        case "SURRENDER":
                            print("Calculando Surrender...")

                        case "NATURAL":
                            print("Calculando Natural...")

            
            message["acks"] = [1, 0, 0, 0]
            print(message)
            send_message(transmit_socket, next_ip, next_port, message)
            return

        case "result-payment":
            check_acks(message["acks"])

            print("Rodada concluida!")
            print("--------------\n")
            return

    return
