import random
import copy
from src.player import NUM_PLAYERS, DEALER_ID, send_message
from src.utils import sum_points, print_hand

#
# === Estruturas ===
#
# players_hand := dicionario de lista de dicionarios
#   { 
#       0: [{"value": "A", "suit": "♠", "points": "11"}, {"value": "J", "suit": "♠", "points": "10"}],
#       1: [{"value": "K", "suit": "♠", "points": "10"}, {"value": "5", "suit": "♠", "points": "5"}],
#   }
#

class Dealer:
    def __init__(self):
        self.bets = [None] * 4
        self.players_hand = {} # No '0'(dealer), contém as cartas como são vistas pelos jogadores
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

# Função que processa a mensagem recebida. Pode retornar diferentes coisas dependendo do tipo da msg
def dealer_process(
        dealer, 
        transmit_socket, next_ip, next_port, 
        message):
    print(f"Recebi: {message}\n")
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
            
            dealer.players_hand = player_cards
            
            # Insere as duas primeiras cartas para a mao do dealer
            dealer.dealer_hand = copy.deepcopy(dealer.players_hand[DEALER_ID])

            # Remove a segunda carta da visão dos jogadores
            dealer.players_hand[DEALER_ID].pop()

            print(f"Cartas do dealer: A segunda está oculta aos jogadores!\n")        
            print_hand(dealer.dealer_hand)

            message["type"] = "distribute-cards"

            message["data"] = player_cards
            message["acks"] = [1, 0, 0, 0]

            # Mensagem com quais cartas cada jogador tem
            print(f"Enviei: {message}")
            send_message(transmit_socket, next_ip, next_port, message)
            return

        case "distribute-cards":
            check_acks(message["acks"])
            print("Cartas distribuidas com sucesso.\n")

            message["type"] = "get-actions"
            message["data"] = [[None, None] for _ in range(NUM_PLAYERS)]  # [action, card]

            message["acks"] = [1, 0, 0, 0]
            print(f"Enviei: {message}")
            send_message(transmit_socket, next_ip, next_port, message)
            return

        case "get-actions": # data: [[None, None], ['NATURAL', None], ['STAND', None], ['HIT', None]]
            check_acks(message["acks"])

            print("Processando ações:")
            
            has_hit = 0
            for i, action in enumerate(message["data"]):
                if message["data"][i][0] == "HIT":
                    has_hit = 1

            if has_hit:
                for i, action in enumerate(message["data"]):
                    match action[0]:

                            case "HIT":
                                new_card = dealer.deck.pop()
                                message["data"][i][1] = new_card
                                dealer.players_hand[i].append(new_card)
                            
                            case "STAND":
                                pass                    
                            case "SURRENDER":
                                pass
                            case "NATURAL":
                                pass
                            case None: # Ação do Dealer
                                pass

                print(f"{message}\n")
                print(f"Mãos atuais após hits: {dealer.players_hand}\n")

            else: # Acabou as escolhas do round, Dealer precisa ver quem venceu
                message["type"] = "result-payment"

                # Dealer precisa jogar
                dealer_points = sum_points(dealer.dealer_hand)

                while dealer_points < 17:
                    new_card = dealer.deck.pop()
                    dealer.dealer_hand.append(new_card)
                    dealer_points = sum_points(dealer.dealer_hand)

                print("Mão do dealer após jogar:")
                print_hand(dealer.dealer_hand)
                print("\n")

                dealer_bust = 0
                if dealer_points > 21:
                    dealer_bust = 1
                    print("Dealer estorou!\n")

                final_actions = copy.deepcopy(message["data"]) # guarda as ações p ultima iteração sobre

                message["data"] = [[None, None] for _ in range(NUM_PLAYERS)]  # [result, payment]

                # Tratar cada resultado
                for i, action in enumerate(final_actions):
                    if dealer_bust: # todos que não estouraram ou deram surrender ganham
                        if action[0] == None: # Ação do Dealer
                            pass
                        elif action[0] != "BUST" or action[0] != "SURRENDER":                            
                            message["data"][i] = ["WIN", dealer.bets[i] * 2]
                        elif action == "BUST":
                            message["data"][i] = ["LOSE", 0]
                        elif action[0] == "SURRENDER":                            
                            message["data"][i] = ["SURRENDER", dealer.bets[i] / 2]

                    elif dealer_points == 21:
                        if action[0] == "STAND" or action[0] == "BUST":
                            message["data"][i] = ["LOSE", 0]
                        elif action[0] == "SURRENDER":
                            message["data"][i] = ["WIN", dealer.bets[i] * 2]
                        elif action[0] == "NATURAL":
                            message["data"][i] = ["TIE", dealer.bets[i]]
                        else: # Ação do dealer
                            pass

                    else: # Caso padrão -- sem 21 e sem bust                        
                        player_points = sum_points(dealer.players_hand[i])
                        print(f"player_points: {player_points}")

                        match action[0]:

                            case "SURRENDER":
                                print("Calculando Surrender...")
                                message["data"][i] = ["SURRENDER", dealer.bets[i] / 2]                                

                            case "STAND":
                                print("Calculando Stand...")

                                if player_points == dealer_points:
                                    message["data"][i] = ["TIE", dealer.bets[i]]                                    

                                elif player_points > dealer_points:
                                    message["data"][i] = ["WIN", dealer.bets[i] * 2]
                                
                                else:
                                    message["data"][i] = ["LOSE", 0]

                            case "NATURAL":
                                print("Calculando Natural...")
                                message["data"][i] = ["WIN", dealer.bets[i] * 2]

                            case _: # ação do Dealer
                                print("Ação dealer...")
                                pass
            
            message["data"][DEALER_ID] = dealer.dealer_hand
            message["acks"] = [1, 0, 0, 0]
            print(f"Enviei: {message}")
            send_message(transmit_socket, next_ip, next_port, message)
            return

        case "result-payment":
            check_acks(message["acks"])

            print("Rodada concluida!")
            print("--------------\n")
            return

    return
