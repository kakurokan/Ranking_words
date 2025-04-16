# README

## Conteúdo

- [README](#readme)
  - [Conteúdo](#conteúdo)
  - [Descrição](#descrição)
  - [Exemplo de execução](#exemplo-de-execução)
  - [Estrutura do Projeto](#estrutura-do-projeto)
  - [Para rodar o projeto](#para-rodar-o-projeto)
  - [Tecnologias Utilizadas](#tecnologias-utilizadas)

## Descrição

Este projeto foi criado para uma atividade currícula da disciplina "Laboratório de programação". Seu objetivo é ler um livro e apresentar as 'n' palavras mais usadas no livro, incluindo palavras acentuadas. O usuário primeiro insere quantas palavras quer ver no ranking e após isso o caminho do arquivo ".txt" onde está guardado o contéudo do livro. Assim o ranking irá aparecer.

O projeto usa como base o seguinte comando linux:

`cat padre_amaro.txt | tr -cs '[:alpha:]À-ÿ' '[\n*]' | sort | uniq -c | sort -rn | head -n 10`

## Exemplo de execução

*Na pasta do projeto está o livro "Padre Amaro" de Eça de Queirós, que vai ser utilizado para este exemplo.*

Input:

```bash
./main 
10
padre_amaro.txt
```

Output:

```bash
1º: "a", 5690 aparições
2º: "o", 4938 aparições
3º: "de", 3900 aparições
4º: "que", 3709 aparições
5º: "e", 2866 aparições
6º: "se", 2346 aparições
7º: "da", 1981 aparições
8º: "com", 1937 aparições
9º: "um", 1859 aparições
10º: "do", 1792 aparições
```

## Estrutura do Projeto

- `/src`: Código-fonte do projeto.
- `/obj`: Arquivos objeto.
- `/include`: Bibliotecas do projeto.
- `padre_amaro.txt`: Livro para exemplo

## Para rodar o projeto

Antes de começar, é bom ter um editor de código como o Vscode e saber utilizar o terminal/cmd.

```bash
# Clone este repositório
$ git clone <https://github.com/kakurokan/Ranking_words>

#Acesse a pasta do projeto no terminal/cmd
$ cd Ranking_words

#Execute o projeto
$ ./main
```

## Tecnologias Utilizadas

- C
- Makefile

---

Made in Kakali´s house👋
