# Cibely Cristiny e Luiz Gustavo

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

# Atualiza estoque com base nas movimentações do dia
for produto, tipo, quantidade in movimentacoes_dia:
    if produto not in estoque_atual:
        estoque_atual[produto] = 0  
    
    if tipo == "entrada":
        estoque_atual[produto] += quantidade
    elif tipo == "saída":
        estoque_atual[produto] -= quantidade

while True:
    print("\n=== MENU DE ESTOQUE ===")
    print("1 - Adicionar produto")
    print("2 - Remover produto")
    print("3 - Mostrar estoque")
    print("4 - Sair")

    opcao = input("Escolha uma opção: ")

    if opcao == "1":
        produto = input("Nome do produto: ").lower()
        quantidade = int(input("Quantidade a adicionar: "))
        estoque_atual[produto] = estoque_atual.get(produto, 0) + quantidade
        print(f"✅ {quantidade} unidades de '{produto}' adicionadas ao estoque.")

    elif opcao == "2":
        produto = input("Nome do produto: ").lower()
        if produto in estoque_atual:
            quantidade = int(input("Quantidade a remover: "))
            if quantidade <= estoque_atual[produto]:
                estoque_atual[produto] -= quantidade
                print(f"❌ {quantidade} unidades de '{produto}' removidas do estoque.")
            else:
                print("Quantidade maior que o disponível em estoque.")
        else:
            print("Produto não encontrado no estoque.")

    elif opcao == "3":
        print("\n===== RELATÓRIO DE ESTOQUE =====")
        for produto, qtd in estoque_atual.items():
            print(f"- {produto}: {qtd} unidades")

        reposicao = [p for p, qtd in estoque_atual.items() if qtd <= 50]
        print("\nProdutos que precisam de reposição:")
        if reposicao:
            for p in reposicao:
                print(f"{p} (estoque atual: {estoque_atual[p]})")
        else:
            print("Nenhum produto precisa de reposição.")

    elif opcao == "4":
        print("Saindo do sistema...")
        break

    else:
        print("Opção inválida. Tente novamente.")
