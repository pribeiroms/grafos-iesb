Sim, os comandos são os mesmos. O Graphviz identifica o tipo pelo conteúdo do arquivo:

- `graph` com `--` = grafo não orientado;
- `digraph` com `->` = grafo orientado.

Estando na pasta `aula-01`, execute:

```powershell
dot -Tpng .\grafo-orientado.dot -o .\grafo-orientado.png
Start-Process .\grafo-orientado.png
```

No `como-executar.md`, você pode deixar os dois exemplos:

```powershell
# Grafo não orientado
dot -Tpng .\grafo-nao-orientado.dot -o .\grafo-nao-orientado.png

# Grafo orientado
dot -Tpng .\grafo-orientado.dot -o .\grafo-orientado.png
```

A única diferença é trocar o nome do arquivo de entrada e da imagem de saída.