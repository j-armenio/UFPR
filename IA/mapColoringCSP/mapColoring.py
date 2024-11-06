# Verifica se a 'color' em 'region' atende as restrições
def is_consistent(region, color, assignment, constraints):
    for r1, r2 in constraints:
        if r1 == region: # Apenas regioes da restrição atual
            if assignment[r2] == color: # Verifica se a cor da restrição é igual
                return False
        
        if r2 == region:
            if assignment[r1] == color:
                return False
    return True

def backtrack(assignment, constraints, domain): # Retorna ou a solução ou falha
    # Base da recursão
    has_none = 0
    for i in assignment.values():
        if i == None:
            has_none = 1
    if has_none == 0:
        return assignment

    # Escolhe variavel unassigned
    region = None
    for r in assignment:
        if assignment[r] == None:
            region = r
            break
    
    # Passo da recursão
    for color in domain:
        if is_consistent(region, color, assignment, constraints):
            assignment[region] = color

            # Chamada recursiva
            result = backtrack(assignment, constraints, domain)
            if result:
                return result

            assignment[region] = None
        
    return None # Não há solução

# Australia

regions = ['WA','NT','Q','NSW','V','SA','T']

domain = ['red', 'green', 'blue']

constraints = [
    ('SA','WA'), ('SA','NT'), ('SA','Q'), ('SA','NSW'), ('SA','V'),
    ('WA','NT'), ('NT','Q'), ('Q','NSW'), ('NSW','V')
]

assignment = {
    'WA': None, 'NT': None, 'SA': None,
    'Q': None, 'NSW': None, 'V': None, 'T': None
}
    
print(backtrack(assignment, constraints, domain))
