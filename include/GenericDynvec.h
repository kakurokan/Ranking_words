#ifndef DYNVEC_H
#define DYNVEC_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Macro para capacidade inicial do vetor dinâmico */
#define DYNVEC_INIT_CAPACITY 8

typedef struct Intdynvec dynvec;

/*
 * Estrutura auxiliar utilizada para armazenar os limites
 * das partições no quicksort 3-way.
 */
typedef struct Limits Limits;

/*-------------------------------------------------------
    Declarações das funções do vetor dinâmico
-------------------------------------------------------*/

/* Cria e inicializa um vetor dinâmico com o tamanho de elemento especificado */
dynvec *dynvec_create(size_t elem_size);

/* Insere um novo elemento no final do vetor */
bool dynvec_push(dynvec *vec, void *item);

/* Remove e retorna uma cópia do último elemento do vetor */
void *dynvec_pop(dynvec *vec);

/* Retorna o elemento na posição 'i'. Retorna NULL se 'i' for inválido */
void *dynvec_get(dynvec *vec, size_t i);

/* Define um novo valor para o elemento na posição 'i' */
bool dynvec_set(dynvec *vec, size_t i, void *item);

/* Retorna o último elemento (topo) do vetor */
void *dynvec_top(dynvec *vec);

/* Insere um novo elemento na posição 'i' */
bool dynvec_insert(dynvec *vec, size_t i, void *n);

/* Remove o elemento da posição 'i' */
bool dynvec_delete(dynvec *vec, size_t i);

/* Verifica se o vetor contém o elemento (usando função de comparação) */
bool dynvec_contains(dynvec *vec, void *elem, int (*cmp)(const void *, const void *));

/* Retorna o índice do elemento (usando função de comparação); se não encontrado, retorna -1 */
size_t dynvec_index(dynvec *vec, void *elem, int (*cmp)(const void *, const void *));

/* Aplica uma função de processamento em cada elemento do vetor */
void dynvec_map(dynvec *vec, void (*processo)(void *));

/* Aplica uma função de acumulação (fold left) sobre os elementos do vetor */
void dynvec_fold_left(dynvec *vec, void *acc, void (*func)(void *acc, void *elem));

/* Verifica se todos os elementos satisfazem um predicado */
bool dynvec_forall(dynvec *vec, bool (*predicate)(void *));

/* Verifica se existe pelo menos um elemento que satisfaz o predicado */
bool dynvec_exists(dynvec *vec, bool (*predicate)(void *));

/* Retorna o índice do primeiro elemento que satisfaz o predicado; se nenhum satisfaz, retorna -1 */
size_t dynvec_exists_index(dynvec *vec, bool (*predicate)(void *));

/* Cria um novo vetor contendo somente os elementos que satisfazem o predicado */
dynvec *dynvec_filter(dynvec *vec, bool (*predicate)(void *));

/* Busca linear: retorna o índice do elemento que corresponde à chave; se não encontrado, retorna -1 */
int dynvec_lsearch(dynvec *vec, void *key, int (*cmp)(const void *, const void *));

/* Busca binária (para vetor ordenado): retorna o índice do elemento que corresponde à chave; se não encontrado, retorna -1 */
int dynvec_bsearch(dynvec *vec, void *key, int (*cmp)(const void *, const void *));

/* Retorna a capacidade atual do vetor */
size_t dynvec_capacity(dynvec *vec);

/* Retorna o número de elementos atualmente armazenados no vetor */
size_t dynvec_length(dynvec *vec);

/* Retorna o tamanho dos elementos atualmente armazenados no vetor */
size_t dynvec_elem_size(dynvec *vec);

/* Reinicializa o vetor, liberando os elementos e alocando novamente com capacidade inicial */
bool dynvec_empty(dynvec *vec);

/* Libera a memória alocada para o vetor dinâmico */
void dynvec_free(dynvec *vec);

/*-------------------------------------------------------
    Declarações das funções de ordenação
-------------------------------------------------------*/

/*
 * Ordenação utilizando Quicksort com particionamento 3-way.
 * - 'cmp' é a função de comparação.
 * - 'left' e 'right' definem os limites do vetor a ser ordenado.
 */
void quicksort_dynvec_three_way(dynvec *vec, int (*cmp)(const void *, const void *), size_t left, size_t right);

/*
 * Ordenação utilizando Mergesort.
 * - 'temp_data' é um vetor auxiliar com o mesmo tamanho do vetor dinâmico.
 * - 'cmp' é a função de comparação.
 * - 'left' e 'right' definem os limites do vetor a ser ordenado.
 */
void mergesort_dynvec(dynvec *vec, void *temp_data, int (*cmp)(const void *, const void *), size_t left, size_t right);

#endif /* DYNVEC_H */