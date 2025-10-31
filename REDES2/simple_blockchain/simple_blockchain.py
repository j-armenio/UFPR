import hashlib
import time

class Block:
    def __init__(self, data, previousHash="0", timeStamp):
        self.data = data
        self.previousHash = previousHash
        self.timeStamp = timeStamp
        self.nonce = 0
        self.hash = self.calculateBlockHash()
        
    def calculateBlockHash(self) -> str:
        '''
        Calcula o hash SHA-256 do conteudo do bloco.
        Concatena previousHash, timeStamp, nonce e data
        Retorna o hexdigest de 64 caracteres
        '''
        value = f"{self.previousHash}{self.timeStamp}{self.nonce}{self.data}"
        return hashlib.sha256(value.encode('utf-8')).hexdigest()

    def mineBlock(self, prefix) -> str:
        '''
        Proof-of-work: incrementa nonce até que o hash do
        bloco combine com o prefix
        '''
        target_prefix = '0' * prefix
        while not self.hash.startswith(target_prefix):
            self.nonce += 1
            self.hash = self.calculateBlockHash()
        
        return self.hash
    
def givenBlockchain_whenNewBlockAdded_thenSuccess():
    # cria novo bloco
    newBlock = Block(
        data="This is a New Block.",
        previousHash=blockchain[-1].hash,
        timeStamp=int(time.time() * 1000)
    )

    # minera o bloco
    newBlock.mineBlock(prefix)

    # verifica se o hash começa com o prefixo correto
    assert newBlock.hash[:prefix] == prefixString

    # adiciona o bloco a blockchain
    blockchain.append(newBlock)

def givenBlockchain_whenValidated_thenSuccess():
    flag = True
    for i in range(len(blockchain)):
        previousHash = "0" if i == 0 else blockchain[i - 1].hash
        flag = (
            blockchain[i].hash == blockchain[i].calculateBlockHash()
            and previousHash == blockchain[i].previousHash
            and blockchain[i].hash[:prefix] == prefixString
        )
        if not flag:
            break
    
    assert flag
