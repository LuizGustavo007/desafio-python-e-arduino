
estoque_atual = {
    "canetas": 150,
    "cadernos": 95,
    "borrachas": 70,
    "lápis": 200,
    "réguas": 60,
    "marcadores": 80,
    "pastas": 40,
    "clips": 500
}

movimentacoes_dia = [
    ("canetas", "saída", 25),
    ("cadernos", "entrada", 10),
    ("borrachas", "saída", 50),
    ("lápis", "saída", 30),
    ("réguas", "entrada", 15),
    ("marcadores", "saída", 40),
    ("pastas", "entrada", 20),
    ("clips", "saída", 200),
    ("apontadores", "entrada", 35),  
    ("marcadores", "entrada", 25),
    ("canetas", "saída", 60),
    ("pastas", "saída", 30),
    ("cadernos", "saída", 80),
    ("lápis", "entrada", 50),
    ("clips", "saída", 100)
]


for produto, tipo, quantidade in movimentacoes_dia:
    if produto not in estoque_atual:
        estoque_atual[produto] = 0  
    
    if tipo == "entrada":
        estoque_atual[produto] += quantidade
    elif tipo == "saída":
        estoque_atual[produto] -= quantidade


reposicao = [produto for produto, qtd in estoque_atual.items() if qtd <= 50]

print("===== RELATÓRIO DE ESTOQUE =====")
print("Estoque atualizado:")
for produto, qtd in estoque_atual.items():
    print(f"- {produto}: {qtd} unidades")

print("\nProdutos que precisam de reposição:")
if reposicao:
    for produto in reposicao:
        print(f"⚠️ {produto} (estoque atual: {estoque_atual[produto]})")
else:
    print("Nenhum produto precisa de reposição.")
