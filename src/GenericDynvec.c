#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Capacidade inicial do vetor dinâmico
#define DYNVEC_INIT_CAPACITY 8

// Estrutura que representa o vetor dinâmico genérico
typedef struct Intdynvec
{
    void *data;       // Ponteiro para os elementos
    size_t elem_size; // Tamanho de cada elemento
    size_t capacity;  // Capacidade atual do vetor
    size_t length;    // Número de elementos armazenados
} dynvec;

// Estrutura auxiliar para particionamento (utilizada no quicksort 3-way)
typedef struct Limits
{
    size_t left;
    size_t right;
} Limits;

/*-------------------------------------------------------
   Funções de manipulação do vetor dinâmico (dynvec)
-------------------------------------------------------*/

// Função auxiliar: troca os elementos nas posições 'a' e 'b' do vetor
static void swap(dynvec *vec, size_t a, size_t b)
{
    if (!vec || !vec->data || vec->elem_size == 0)
        return;

    void *temp = malloc(vec->elem_size);
    if (!temp)
        return;

    memcpy(temp, (char *)vec->data + a * vec->elem_size, vec->elem_size);
    memcpy((char *)vec->data + a * vec->elem_size, (char *)vec->data + b * vec->elem_size, vec->elem_size);
    memcpy((char *)vec->data + b * vec->elem_size, temp, vec->elem_size);
    free(temp);
}

// Função auxiliar: redimensiona o vetor para a nova capacidade
static bool dynvec_resize(dynvec *vec, size_t new_capacity)
{
    void *temp = realloc(vec->data, new_capacity * vec->elem_size);
    if (temp == NULL)
    {
        return false; // Falha ao redimensionar
    }
    vec->data = temp;
    vec->capacity = new_capacity;
    return true;
}

// Cria e inicializa um vetor dinâmico com o tamanho de elemento especificado
dynvec *dynvec_create(size_t elem_size)
{
    dynvec *vec = malloc(sizeof(dynvec));
    if (!vec)
        return NULL;

    vec->capacity = DYNVEC_INIT_CAPACITY;
    vec->length = 0;
    vec->elem_size = elem_size;
    vec->data = malloc(vec->elem_size * vec->capacity);
    if (!vec->data)
    {
        free(vec);
        return NULL;
    }
    return vec;
}

// Insere um novo elemento no final do vetor
bool dynvec_push(dynvec *vec, void *item)
{
    if (vec->length + 1 > vec->capacity)
    {
        if (!dynvec_resize(vec, vec->capacity * 2))
            return false;
    }
    memcpy((char *)vec->data + vec->elem_size * vec->length, item, vec->elem_size);
    vec->length++;
    return true;
}

// Remove e retorna uma cópia do último elemento do vetor
void *dynvec_pop(dynvec *vec)
{
    if (vec->length == 0)
        return NULL;

    void *value = malloc(vec->elem_size);
    if (!value)
        return NULL;

    memcpy(value, (char *)vec->data + (vec->length - 1) * vec->elem_size, vec->elem_size);
    vec->length--;

    // Reduz a capacidade se o número de elementos for pequeno
    if (vec->length <= vec->capacity / 4)
    {
        dynvec_resize(vec, vec->capacity / 2);
    }
    return value;
}

// Retorna o elemento na posição 'i'. Se 'i' for inválido, retorna NULL.
void *dynvec_get(dynvec *vec, size_t i)
{
    return (i < vec->length) ? (char *)vec->data + i * vec->elem_size : NULL;
}

// Define um novo valor para o elemento na posição 'i'
bool dynvec_set(dynvec *vec, size_t i, void *item)
{
    if (i >= vec->length)
        return false;
    memcpy((char *)vec->data + i * vec->elem_size, item, vec->elem_size);
    return true;
}

// Retorna o último elemento (topo) do vetor
void *dynvec_top(dynvec *vec)
{
    return (vec->length > 0) ? (char *)vec->data + (vec->length - 1) * vec->elem_size : NULL;
}

// Insere um novo elemento na posição 'i'
bool dynvec_insert(dynvec *vec, size_t i, void *n)
{
    if (i > vec->length)
        return false;
    if (vec->length + 1 > vec->capacity)
    {
        if (!dynvec_resize(vec, vec->capacity * 2))
            return false;
    }
    // Desloca os elementos para abrir espaço
    memmove((char *)vec->data + (i + 1) * vec->elem_size,
            (char *)vec->data + i * vec->elem_size,
            (vec->length - i) * vec->elem_size);
    memcpy((char *)vec->data + i * vec->elem_size, n, vec->elem_size);
    vec->length++;
    return true;
}

// Remove o elemento da posição 'i'
bool dynvec_delete(dynvec *vec, size_t i)
{
    if (i >= vec->length)
        return false;
    // Desloca os elementos para "sobrescrever" o elemento removido
    memmove((char *)vec->data + i * vec->elem_size,
            (char *)vec->data + (i + 1) * vec->elem_size,
            (vec->length - i - 1) * vec->elem_size);
    vec->length--;

    if (vec->length <= vec->capacity / 4)
    {
        dynvec_resize(vec, vec->capacity / 2);
    }
    return true;
}

// Verifica se o vetor contém o elemento (usando função de comparação)
bool dynvec_contains(dynvec *vec, void *elem, int (*cmp)(const void *, const void *))
{
    if (!vec || !elem || !cmp)
        return false;
    for (size_t i = 0; i < vec->length; i++)
    {
        if (cmp((char *)vec->data + i * vec->elem_size, elem))
            return true;
    }
    return false;
}

// Retorna o índice do elemento (usando função de comparação); se não encontrado, retorna -1
size_t dynvec_index(dynvec *vec, void *elem, int (*cmp)(const void *, const void *))
{
    if (!vec || !elem || !cmp)
        return -1;
    for (size_t i = 0; i < vec->length; i++)
    {
        if (cmp((char *)vec->data + i * vec->elem_size, elem))
            return i;
    }
    return -1;
}

// Aplica uma função de processamento em cada elemento do vetor
void dynvec_map(dynvec *vec, void (*processo)(void *))
{
    if (!vec || !processo)
        return;
    for (size_t i = 0; i < vec->length; i++)
    {
        processo((char *)vec->data + i * vec->elem_size);
    }
}

// Aplica uma função de acumulação (fold left) sobre os elementos do vetor
void dynvec_fold_left(dynvec *vec, void *acc, void (*func)(void *acc, void *elem))
{
    if (!vec || !acc || !func)
        return;
    for (size_t i = 0; i < vec->length; i++)
        func(acc, (char *)vec->data + i * vec->elem_size);
}

// Verifica se todos os elementos satisfazem um predicado
bool dynvec_forall(dynvec *vec, bool (*predicate)(void *))
{
    if (!vec || !predicate)
        return false;
    for (size_t i = 0; i < vec->length; i++)
    {
        if (!predicate((char *)vec->data + i * vec->elem_size))
            return false;
    }
    return true;
}

// Verifica se existe ao menos um elemento que satisfaz o predicado
bool dynvec_exists(dynvec *vec, bool (*predicate)(void *))
{
    if (!vec || !predicate)
        return false;
    for (size_t i = 0; i < vec->length; i++)
    {
        if (predicate((char *)vec->data + i * vec->elem_size))
            return true;
    }
    return false;
}

// Retorna o índice do primeiro elemento que satisfaz o predicado; caso contrário, retorna -1
size_t dynvec_exists_index(dynvec *vec, bool (*predicate)(void *))
{
    if (!vec || !predicate)
        return -1;
    for (size_t i = 0; i < vec->length; i++)
    {
        if (predicate((char *)vec->data + i * vec->elem_size))
            return i;
    }
    return -1;
}

// Cria um novo vetor contendo somente os elementos que satisfazem o predicado
dynvec *dynvec_filter(dynvec *vec, bool (*predicate)(void *))
{
    if (!vec || !predicate)
        return NULL;
    dynvec *new_vec = dynvec_create(vec->elem_size);
    if (!new_vec)
        return NULL;
    for (size_t i = 0; i < vec->length; i++)
    {
        void *elem = (char *)vec->data + i * vec->elem_size;
        if (predicate(elem))
            dynvec_push(new_vec, elem);
    }
    return new_vec;
}

// Busca linear: retorna o índice do elemento que corresponde à chave; se não encontrado, retorna -1
int dynvec_lsearch(dynvec *vec, void *key, int (*cmp)(const void *, const void *))
{
    size_t size = vec->length;
    for (size_t i = 0; i < size; i++)
    {
        void *elem = (char *)vec->data + i * vec->elem_size;
        if (cmp(key, elem) == 0)
            return i;
    }
    return -1;
}

// Busca binária (o vetor deve estar ordenado): retorna o índice do elemento que corresponde à chave; se não encontrado, retorna -1
int dynvec_bsearch(dynvec *vec, void *key, int (*cmp)(const void *, const void *))
{
    size_t low = 0, high = (vec->length) - 1, mid;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        void *elem = (char *)vec->data + mid * vec->elem_size;
        int comparision = cmp(key, elem);
        if (comparision == 0)
            return mid;
        if (comparision < 0)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return -1;
}

// Libera a memória alocada para o vetor dinâmico
void dynvec_free(dynvec *vec)
{
    if (vec)
    {
        free(vec->data);
        free(vec);
    }
}

// Retorna a capacidade atual do vetor
size_t dynvec_capacity(dynvec *vec)
{
    return vec->capacity;
}

// Retorna o número de elementos atualmente armazenados
size_t dynvec_length(dynvec *vec)
{
    return vec->length;
}

size_t dynvec_elem_size(dynvec *vec)
{
    return vec->elem_size;
}

// Reinicializa o vetor, liberando os elementos e alocando novamente com capacidade inicial.
// Retorna true se a operação foi bem-sucedida.
bool dynvec_empty(dynvec *vec)
{
    free(vec->data);
    vec->capacity = DYNVEC_INIT_CAPACITY;
    vec->length = 0;
    vec->data = malloc(vec->elem_size * vec->capacity);
    return (vec->data != NULL);
}

/*-------------------------------------------------------
   Funções de ordenação
-------------------------------------------------------*/

// Particiona o vetor em três partes (<, =, >) em relação ao pivô
// Utiliza a técnica quicksort 3-way para melhorar desempenho em casos com muitos elementos iguais
static Limits partition_three_way(dynvec *vec, int (*cmp)(const void *, const void *), size_t left, size_t right)
{
    size_t i = left + 1;
    Limits limite = {left, right};

    // Escolhe um pivô aleatório e o posiciona no início
    size_t r_pivot = left + rand() % (right - left + 1);
    swap(vec, r_pivot, left);

    // Copia o valor do pivô para uma variável auxiliar
    void *pivot = malloc(vec->elem_size);
    memcpy(pivot, (char *)vec->data + left * vec->elem_size, vec->elem_size);

    // Particiona o vetor comparando cada elemento com o pivô
    while (i <= limite.right)
    {
        int comp = cmp((char *)vec->data + i * vec->elem_size, pivot);
        if (comp < 0)
        {
            swap(vec, i, limite.left);
            limite.left++;
            i++;
        }
        else if (comp > 0)
        {
            swap(vec, i, limite.right);
            limite.right--;
        }
        else
        {
            i++;
        }
    }
    free(pivot);
    return limite;
}

// Quicksort utilizando particionamento 3-way para ordenar o vetor dinâmico
void quicksort_dynvec_three_way(dynvec *vec, int (*cmp)(const void *, const void *), size_t left, size_t right)
{
    if (!vec || !cmp || !vec->data || vec->elem_size <= 1)
        return;
    if (left < right)
    {
        Limits l = partition_three_way(vec, cmp, left, right);
        if (l.left > left)
            quicksort_dynvec_three_way(vec, cmp, left, l.left - 1);
        quicksort_dynvec_three_way(vec, cmp, l.right + 1, right);
    }
}

// Função auxiliar: realiza a mesclagem (merge) de duas sub-partes do vetor
static void merge(dynvec *vec, int (*cmp)(const void *, const void *), size_t left, size_t right, size_t mid, void *temp)
{
    size_t i = left, j = mid;
    for (size_t k = left; k < right; k++)
    {
        void *elem1 = (char *)temp + i * vec->elem_size;
        void *elem2 = (char *)temp + j * vec->elem_size;
        void *target = (char *)vec->data + k * vec->elem_size;
        if (i < mid && (j == right || cmp(elem1, elem2) <= 0))
        {
            memcpy(target, elem1, vec->elem_size);
            i++;
        }
        else
        {
            memcpy(target, elem2, vec->elem_size);
            j++;
        }
    }
}

// Mergesort para ordenar o vetor dinâmico. 'temp_data' é um vetor auxiliar com o mesmo tamanho.
void mergesort_dynvec(dynvec *vec, void *temp_data, int (*cmp)(const void *, const void *), size_t left, size_t right)
{
    if (!vec || !cmp || !vec->data || vec->elem_size <= 1 || right - left <= 1)
        return;

    size_t mid = left + (right - left) / 2;
    mergesort_dynvec(vec, temp_data, cmp, left, mid);
    mergesort_dynvec(vec, temp_data, cmp, mid, right);

    // Copia a parte a ser mesclada para o vetor auxiliar
    memcpy((char *)temp_data + left * vec->elem_size, (char *)vec->data + left * vec->elem_size, (right - left) * vec->elem_size);
    merge(vec, cmp, left, right, mid, temp_data);
}
