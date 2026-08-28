# Como executar um arquivo `.dot`

Este guia serve para qualquer arquivo `.dot`.

## Preencher

No PowerShell, substitua os valores abaixo:

```powershell
$pasta = "CAMINHO_DA_PASTA"
$arquivo = "NOME_DO_ARQUIVO"
```

O nome do arquivo deve ser informado sem a extensão `.dot`.

## Entrar na pasta

```powershell
cd $pasta
```

## Gerar a imagem

O Graphviz lerá o arquivo definido em `$arquivo` e criará uma imagem `.png` com o mesmo nome:

```powershell
dot -Tpng ".\$arquivo.dot" -o ".\$arquivo.png"
```
 
## Abrir a imagem

```powershell
Start-Process ".\$arquivo.png"
```

Para gerar outro grafo, altere apenas o valor de `$arquivo` e execute os comandos novamente.

## Materiais relacionados

- [Configuração do ambiente](configuracao-ambiente.md): instale e configure o Graphviz.
- [Introdução ao Graphviz](introducao-graphviz.md): entenda a estrutura do arquivo `.dot`.
- [Tipos de grafos](tipos-de-grafos.md): veja exemplos de grafos orientados e não orientados.
