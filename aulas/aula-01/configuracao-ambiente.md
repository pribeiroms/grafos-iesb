# Configuração do ambiente 

## Clonar o template

Clone o repositório disponibilizado pelo professor:

```powershell
git clone https://github.com/profjosereginaldo/grafos-template.git
```

## Instalar o Graphviz

No PowerShell, execute:

```powershell
winget install Graphviz.Graphviz
```

Confirme a instalação quando o Windows perguntar.

## Reiniciar o terminal

Feche e abra o terminal novamente. Se necessário, feche e abra o VS Code.

Isso é importante para o Windows atualizar o `PATH`.

## Confirmar a instalação

Execute:

```powershell
dot -V
```

Você deverá ver algo parecido com:

```text
dot - graphviz version ...
```

## Se o comando `dot` não for reconhecido

O erro significa que o Windows não encontrou o executável `dot` no `PATH`.

### Verificar o caminho padrão

```powershell
Test-Path "C:\Program Files\Graphviz\bin\dot.exe"
```

Se retornar `True`, execute o Graphviz diretamente:

```powershell
& "C:\Program Files\Graphviz\bin\dot.exe" -V
```

Depois, feche e reabra o VS Code e teste novamente:

```powershell
dot -V
```

### Procurar o executável

Se o primeiro comando retornar `False`, procure onde o Graphviz foi instalado:

```powershell
Get-ChildItem "C:\Program Files","C:\Program Files (x86)" -Filter dot.exe -Recurse -ErrorAction SilentlyContinue
```

### Adicionar permanentemente ao Windows (recomendado)

1. Pressione `Win` e pesquise por **variáveis de ambiente**.
2. Abra **Editar as variáveis de ambiente do sistema**.
3. Clique em **Variáveis de Ambiente**.
4. Em **Variáveis do usuário**, selecione `Path`.
5. Clique em **Editar** e depois em **Novo**.
6. Adicione:

```text
C:\Program Files\Graphviz\bin
```

7. Confirme todas as janelas.
8. Feche e abra o terminal novamente.
9. Teste:

```powershell
dot -V
```

Essa configuração permanece disponível nos próximos terminais do Windows.

### Usar somente no terminal atual

Se o Graphviz estiver instalado em `C:\Program Files\Graphviz\bin`, adicione o caminho temporariamente:

```powershell
$env:Path += ";C:\Program Files\Graphviz\bin"
dot -V
```

Isso deve mostrar algo parecido com:

```text
dot - graphviz version 16.0.0 ...
```

Essa alteração vale somente para o terminal atual. Ao fechá-lo, será necessário executar o comando novamente.

Caso somente fechar o terminal não resolva, use novamente no terminal atual:

```powershell
$env:Path += ";C:\Program Files\Graphviz\bin"
dot -V
```

Se funcionar, a configuração permanente está correta, mas ainda não foi atualizada no terminal. Feche o VS Code, abra-o novamente e execute `dot -V`.

## Gerar a imagem do grafo

Entre na pasta onde está o arquivo `.dot`:

```powershell
cd D:\DevHub\grafos-iesb\aulas\aula-01
```

Gere a imagem:

```powershell
dot -Tpng .\grafo-nao-orientado.dot -o .\grafo.png
```

Abra a imagem gerada:

```powershell
Start-Process .\grafo.png
```

## Próximos passos

- [Introdução ao Graphviz](introducao-graphviz.md): conheça a estrutura dos arquivos `.dot`.
- [Tipos de grafos](tipos-de-grafos.md): compare grafos orientados e não orientados.
- [Executar um arquivo `.dot`](executar-dot.md): gere uma imagem a partir de qualquer grafo.



