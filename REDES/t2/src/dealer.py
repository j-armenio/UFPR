import random
import copy
from src.player import NUM_PLAYERS, send_message
from src.utils import sum_points, print_hand, debug_print

class Dealer:
    def __init__(self):
        self.bets = [None] * 4
        self.players_hand = {} # No '0'(dealer), contém as cartas como são vistas pelos jogadores
        self.dealer_hand = []
        self.deck = []
        self.dealer_id = -1

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
    player_cards = {}

    for i in range(0, NUM_PLAYERS):
        # Tira duas cartas para cada jogador
        hand = [deck.pop(), deck.pop()]
        player_cards[i] = hand

    return player_cards

# Função que processa a mensagem recebida. Pode retornar diferentes coisas dependendo do tipo da msg
def dealer_process(
        game_state,
        dealer, player,
        transmit_socket, next_ip, next_port,
        message):
    match message["type"]:
        
        case "inform-dealer":
            check_acks(message["acks"])

            bets = [None] * 4
            message["type"] = "players-bet"
            message["data"] = bets
            message["acks"] = [0] * 4

            message["acks"][dealer.dealer_id] = 1

            print("[DEALER] Solicitando apostas dos jogadores...\n")
            debug_print(f"Pedido de apostas: {message}\n")

            send_message(transmit_socket, next_ip, next_port, message)

        case "result-payment":
            check_acks(message["acks"])

            print("============================ RODADA CONCLUIDA ============================")

            print(f"Passando Dealer para jogador {(dealer.dealer_id + 1) % NUM_PLAYERS}")

            message = {
                "type": "baston",
                "data": None,
                "from": dealer.dealer_id,
                "acks": [0] * 4
            }
            message["acks"][dealer.dealer_id] = 1

            dealer.dealer_id = (dealer.dealer_id + 1) % NUM_PLAYERS
            player.dealer_id = dealer.dealer_id

            debug_print(message)
            send_message(transmit_socket, next_ip, next_port, message)

        case "players-bet": # Dealer processa as apostas do 1 round
            check_acks(message["acks"])

            print("[DEALER] Apostas dos jogadores:")
            for i, bet in enumerate(message["data"]):
                if bet is not None:
                    dealer.bets[i] = bet
                    print(f"[DEALER] Player {i}: {bet}")
            print()

            # Dealer cria o baralho e distribui as cartas
            dealer.deck = generate_deck()
            player_cards = distribute_cards(dealer.deck) 
            
            # players_hand := dicionario de lista de dicionarios
            #   { 
            #       0: [{"value": "A", "suit": "♠", "points": "11"}, {"value": "J", "suit": "♠", "points": "10"}],
            #       1: [{"value": "K", "suit": "♠", "points": "10"}, {"value": "5", "suit": "♠", "points": "5"}],
            #   }

            dealer.players_hand = player_cards
            
            # Insere as duas primeiras cartas para a mao do dealer
            dealer.dealer_hand = copy.deepcopy(dealer.players_hand[dealer.dealer_id])

            # Remove a segunda carta da visão dos jogadores
            dealer.players_hand[dealer.dealer_id].pop()

            print(f"[DEALER] Mão do Dealer (a segunda está oculta aos jogadores!):")
            print("[DEALER]", end=" ") # Prefixo da linha
            print_hand(dealer.dealer_hand)

            message["type"] = "distribute-cards"

            message["data"] = player_cards

            message["acks"] = [0, 0, 0, 0]
            message["acks"][dealer.dealer_id] = 1

            # Mensagem com quais cartas cada jogador tem
            send_message(transmit_socket, next_ip, next_port, message)

        case "distribute-cards":
            check_acks(message["acks"])
            print("[DEALER] Cartas distribuidas com sucesso.\n")

            message["type"] = "get-actions"
            message["data"] = [[None, None] for _ in range(NUM_PLAYERS)]  # [action, card]

            message["acks"] = [0, 0, 0, 0]
            message["acks"][dealer.dealer_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)

        case "get-actions": # data: [[None, None], ['NATURAL', None], ['STAND', None], ['HIT', None]]
            check_acks(message["acks"])

            print("[DEALER] Processando ações:")
            
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
                    # dealer.dealer_hand.append(new_card)
                    dealer.dealer_hand.append({'value': 'Q', 'suit': '♥', 'points': 10})
                    dealer_points = sum_points(dealer.dealer_hand)

                print("[DEALER] Mão do dealer após jogar:")
                print("[DEALER]", end=" ")
                print_hand(dealer.dealer_hand)
                print()

                dealer_bust = 0
                if dealer_points > 21:
                    dealer_bust = 1
                    print("[DEALER] Dealer estorou!\n")

                final_actions = copy.deepcopy(message["data"]) # guarda as ações p ultima iteração sobre

                message["data"] = [[None, None] for _ in range(NUM_PLAYERS)]  # [result, payment]

                print("[DEALER] Calculando resultados.")

                # Tratar cada resultado
                for i, action in enumerate(final_actions):
                    if dealer_bust: # todos que não estouraram ou deram surrender ganham
                        if action[0] == "STAND":
                            message["data"][i] = ["WIN", dealer.bets[i] * 2]
                        elif action[0] == "NATURAL":
                            message["data"][i] = ["WIN", dealer.bets[i] * 2]
                        elif action[0] == "SURRENDER":
                            message["data"][i] = ["SURRENDER", dealer.bets[i] / 2]
                        elif action[0] == "BUST":
                            message["data"][i] = ["LOSE", 0]
                        else: # Ação do dealer
                            pass

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
                        debug_print(f"player_points: {player_points}")

                        match action[0]:

                            case "SURRENDER":
                                debug_print("Calculando Surrender...")
                                message["data"][i] = ["SURRENDER", dealer.bets[i] / 2]                                

                            case "STAND":
                                debug_print("Calculando Stand...")

                                if player_points == dealer_points:
                                    message["data"][i] = ["TIE", dealer.bets[i]]                                    

                                elif player_points > dealer_points:
                                    message["data"][i] = ["WIN", dealer.bets[i] * 2]
                                
                                else:
                                    message["data"][i] = ["LOSE", 0]

                            case "NATURAL":
                                debug_print("Calculando Natural...")
                                message["data"][i] = ["WIN", dealer.bets[i] * 2]

                            case "BUST":
                                debug_print("Calculando Bust...")
                                message["data"][i] = ["LOSE", 0]

                            case _: # ação do Dealer
                                debug_print("Ação dealer...")
                                pass
                            
                print("[DEALER] Enviando resultados para jogadores.\n")
            
            message["data"][dealer.dealer_id] = dealer.dealer_hand

            message["acks"] = [0, 0, 0, 0]
            message["acks"][dealer.dealer_id] = 1
            send_message(transmit_socket, next_ip, next_port, message)
            
    return 'RUNNING'
