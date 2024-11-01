import random
from src.player import send_message

NUM_PLAYERS = 4

class Dealer:
    def __init__(self):
        self.bets = [None] * 4
        self.hand = {}
        self.deck = []

# Gera um baralho embaralhado
def generate_deck():
    values = {
        "A": 11,  # Ou 1, dependendo da necessidade
        "2": 2, "3": 3, "4": 4, "5": 5, "6": 6, "7": 7, "8": 8, "9": 9,
        "10": 10, "J": 10, "Q": 10, "K": 10
    }
    suits = ["♠", "♣", "♥", "♦"]

    # Cria o baralho como uma lista de dicionários
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
    
    return players_cards

# Função que processa a mensagem recebida
def dealer_process(
        dealer, 
        transmit_socket, next_ip, next_port, 
        message):
    match message["type"]:
        case "players-bet": # Dealer processa as apostas do 1 round
            for i, bet in enumerate(message["data"]):
                if bet is not None:
                    dealer.bets[i] = bet
            
            print(f"Apostas recebidas: {dealer.bets}")

            # Dealer cria o baralho e distribui as cartas
            dealer.deck = generate_deck()
            player_cards = distribute_cards(dealer.deck)

            message["type"] = "distribute-cards"
            dealer.hand = player_cards[0]
            
            message["data"] = player_cards
            message["acks"] = [1, 0, 0, 0]

            # Mensagem com quais cartas cada jogador tem
            send_message(transmit_socket, next_ip, next_port, message)

    return
