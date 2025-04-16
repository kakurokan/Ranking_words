#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <GenericDynvec.h>
#include <errno.h>

#define MAX_WORD_LENGTH 50                                                         // Capacidade máxima de uma palavra
#define BOOK_PATH "/home/kakurokan/LP_Projetos/Projeto2_curriculo/padre_amaro.txt" // Caminho do livro à ser lido

// Estrura auxiliar para contar as aparições de cada palavra
typedef struct Word
{
    char *word;
    size_t times;
} word;

// Função para comparar duas strings
int string_comp(const void *key, const void *elem)
{
    const char *key_str = (const char *)key;
    const char *elem_str = (const char *)elem;
    return strcmp(key_str, elem_str);
}

// Função para comparar as aparições de duas estruturas word
int word_times_comp(const void *key, const void *elem)
{
    return ((const word *)key)->times - ((const word *)elem)->times;
}

int main()
{
    int n;
    if (scanf("%d", &n) != 1) // Lê quantas palavras o usuário quer e guarda na variável 'n'
        return -1;
    getchar(); // Consome o '\n' deixado no buffer

    dynvec *vec_input = dynvec_create(sizeof(char) * MAX_WORD_LENGTH); // Vetor para guardar todas as palavras lidas no arquivo

    FILE *file = fopen(BOOK_PATH, "r"); // Variável para guardar o arquivo.
    if (!file)
    {
        perror("Error opening file"); // Emite um erro caso não seja possível ler o arquivo
        return -1;
    }

    char ch;                            // Guarda cada letra lida no livro
    int len = 0;                        // Guarda o tamanho de cada palavra lida
    char current_word[MAX_WORD_LENGTH]; // Guarda a palavra atual

    while ((ch = fgetc(file)) != EOF) // Lê as letras e guarda em 'ch' até o fim do arquivo
    {
        // Insere em "current_word" 'ch', caso seja uma letra normal ou uma letra acentuada
        if (isalpha(ch) || (unsigned)ch >= 128)
        {
            current_word[len++] = ch;
        }
        // Caso exista uma palavra, insere o terminador nulo, a coloca no vetor "vec_input" e reinicia o "len"
        else if (len > 0)
        {
            current_word[len] = '\0';
            dynvec_push(vec_input, current_word);
            len = 0;
        }
    }

    fclose(file); // Fecha o arquivo

    // Caso haja palavras no vetor, ordenas elas utilizando quicksort_three_way
    if (dynvec_length(vec_input) > 0)
    {
        quicksort_dynvec_three_way(vec_input, string_comp, 0, dynvec_length(vec_input) - 1);
    }
    else
    {
        perror("Error reading file");
        return -1;
    }

    dynvec *vec_sorted = dynvec_create(sizeof(word)); // Vetor auxiliar para guardar as palavras com sua quantidade de aparições
    word temp_word;
    temp_word.times = 1; // Iniciliza em 1, pois cada palavra aparece pelo menos 1 vez

    // Copia os elementos de "vec_input" para "vec_sorted", contando quantas aparições tem cada palavra
    for (size_t i = 0; i < dynvec_length(vec_input); i++)
    {
        char *atual = (char *)dynvec_get(vec_input, i);

        // Caso a palavra atual e a seguinte sejam iguais, incrementa a sua aparição
        if (i + 1 != dynvec_length(vec_input) && string_comp(atual, dynvec_get(vec_input, i + 1)) == 0)
        {
            temp_word.times++;
        }
        // Caso o contrário, guarda ela no vetor e reinicia as aparições para a próxima
        else
        {
            temp_word.word = atual;
            dynvec_push(vec_sorted, &temp_word);

            temp_word.times = 1;
        }
    }

    void *auxiliar = malloc((dynvec_length(vec_sorted)) * (dynvec_elem_size(vec_sorted))); // Vetor auxiliar para realizar o mergesort
    mergesort_dynvec(vec_sorted, auxiliar, word_times_comp, 0, dynvec_length(vec_sorted)); // Realizar o mergesort para ordenar o vetor por ordem de aparição

    // Escreve no console as 'n' palavras mais usadas do livro
    for (int i = dynvec_length(vec_sorted) - 1, j = 1; i >= 0 && n > 0; i--, n--, j++)
    {
        word *tmp_word = (word *)dynvec_get(vec_sorted, (size_t)i);
        printf("%dº: \"%s\", %zu aparições\n", j, tmp_word->word, tmp_word->times);
    }

    // Libera os vetores utilizados na memória
    free(auxiliar);
    dynvec_free(vec_input);
    dynvec_free(vec_sorted);

    return 0;
}