#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int id = 0;

// Função para criar um novo nó da árvore
link novoNo(int item, link l, link r) {
  link t = malloc(sizeof(struct node));
  t->item = item;
  t->l = l;
  t->r = r;
  t->N = 1;
  t->id = id++;
  t->height = 0; // Inicialmente, a altura da subárvore é 0
  return t;
}

// Função para criar uma nova árvore
Tree createTree() {
  Tree t = malloc(sizeof(struct tree));
  t->z = malloc(sizeof(struct node));
  t->head = t->z;
  t->z->l = t->z;
  t->z->N = 0;
  t->z->r = t->z;
  t->z->height = 0; // A altura do nó externo é 0
  return t;
}

//Essas funções são usadas para calcular a altura de um nó, atualizar a altura de um nó e calcular o fator de balanceamento de um nó, respectivamente. O fator de balanceamento é a diferença entre as alturas das subárvores esquerda e direita.

// Função para obter a altura de um nó
int altura(link h) {
  if (h == NULL)
    return 0;
  return h->height;
}

// Função para atualizar a altura de um nó
int atualizaAltura(link h) {
  int altura_esq = altura(h->l);
  int altura_dir = altura(h->r);
  return (altura_esq > altura_dir ? altura_esq : altura_dir) + 1;
}

// Função para calcular o fator de balanceamento de um nó
int fatorBalanceamento(link h) {
  if (h == NULL)
    return 0;
  return altura(h->l) - altura(h->r);
}

// Rotação direita em um nó
link rotR(Tree t, link h) {
  link x = h->l;
  h->l = x->r;
  x->r = h;
  h->height = atualizaAltura(h);
  x->height = atualizaAltura(x);
  return x;
}

// Rotação esquerda em um nó
link rotL(Tree t, link h) {
  link x = h->r;
  h->r = x->l;
  x->l = h;
  h->height = atualizaAltura(h);
  x->height = atualizaAltura(x);
  return x;
}

// Função de busca recursiva em uma árvore
link searchR(Tree t, link h, int query) {
  if (h == t->z) {
    return NULL;
  }
  if (h->item == query)
    return h;
  if (h->item >= query)
    return searchR(t, h->l, query);
  return searchR(t, h->r, query);
}

// Função de busca em uma árvore
link search(Tree t, int query) { return searchR(t, t->head, query); }

// Função de inserção recursiva em uma árvore
link insertR(Tree t, link h, link n) {
  if (h == t->z) {
    return n;
  }

  if (h->item >= n->item)
    h->l = insertR(t, h->l, n);
  else
    h->r = insertR(t, h->r, n);

  h->N++;
  h->height = atualizaAltura(h); // mantém o balanceamento da árvore chamando as rotações apropriadas quando necessário 

  int fb = fatorBalanceamento(h);

  // Balanceamento
  if (fb > 1) {
    if (n->item < h->l->item)
      return rotR(t, h);
    else {
      h->l = rotL(t, h->l);
      return rotR(t, h);
    }
  }
  if (fb < -1) {
    if (n->item > h->r->item)
      return rotL(t, h);
    else {
      h->r = rotR(t, h->r);
      return rotL(t, h);
    }
  }
  return h;
}

// Função de inserção em uma árvore
link insert(Tree t, int item) {
  if (t->head == t->z) {
    return (t->head = novoNo(item, t->z, t->z));
  }
  t->head = insertR(t, t->head, novoNo(item, t->z, t->z));
  return t->head;
}

// Função para imprimir em ordem
void imprimeEmOrdem(Tree t, link h) {
  if (h == t->z)
    return;
  imprimeEmOrdem(t, h->l);
  printf("<chave: %d N: %d Altura: %d>", h->item, h->N, h->height);
  imprimeEmOrdem(t, h->r);
}

// Função para imprimir em pós-ordem
void imprimePosOrdem(Tree t, link h) {
  if (h == t->z)
    return;
  imprimePosOrdem(t, h->l);
  imprimePosOrdem(t, h->r);
  printf("<chave: %d N: %d Altura: %d>", h->item, h->N, h->height);
}

// Função para imprimir em pré-ordem
void imprimePreOrdem(Tree t, link h, int k) {
  if (h == t->z)
    return;
  for (int i = 0; i <= k; i++)
    printf(" . ");
  printf("<chave: %d N: %d Altura: %d>\n", h->item, h->N, h->height);
  imprimePreOrdem(t, h->l, k + 1);
  imprimePreOrdem(t, h->r, k + 1);
}

// Função para imprimir um nó
void printnode(char *x, int h) {
  int i;
  for (i = 0; i < h; i++)
    printf("\t");
  printf("%s\n", x);
}

int c = 0;

// Função para imprimir a árvore em formato de grafo
void imprimeR(Tree a, link t) {
  char s[255];
  if (t == a->z) {
    return;
  }
  imprimeR(a, t->r);
  printf("%d [ label = \"<f0>| <f1> %d (%d,%d) | <f2>\" ]\n", t->id, t->item,
         t->N, t->height);
  if (t->l != a->z) {
    printf("%d:f0->%d:f1;\n", t->id, t->l->id);
  } else {
    printf("z%d [ shape=point ];\n", c);
    printf("%d:f0->z%d;\n", t->id, c);
    c++;
  }
  if (t->r != a->z) {
    printf("%d:f2->%d:f1;\n", t->id, t->r->id);
  } else {
    printf("z%d [ shape=point ];\n", c);
    printf("%d:f2->z%d;\n", t->id, c);
    c++;
  }
  imprimeR(a, t->l);
}

// Função para imprimir a árvore em formato de grafo
void imprime(Tree a, char *s) {
  c = 0;
  printf("digraph %s {\n", s);
  printf("node [shape=record,style=rounded, height=.1]\n");
  imprimeR(a, a->head);
  printf("}\n");
}

// Função para imprimir a árvore a partir de um nó específico em formato de grafo
void imprimeFromNode(Tree a, link h, char *s) {
  c = 0;
  printf("digraph  {\n");
  printf("name [label = \"%s\",shape=plaintext]\n", s);
  printf("node [shape=record,style=rounded, height=.1]\n");
  imprimeR(a, h);
  printf("}\n");
}

  //São semelhantes às funções de inserção regulares, mas também aplicam rotações para manter   o balanceamento da árvore AVL.
  // Função de inserção AVL recursiva
link AVLinsertR(Tree t, link h, int item) {
    // Se chegamos a uma folha nula, criamos um novo nó
  if (h == t->z) {
    return novoNo(item, t->z, t->z);
  }
  // Inserção na subárvore esquerda ou direita com base no valor de 'item'
  if (item < h->item)
    h->l = AVLinsertR(t, h->l, item);
  else if (item > h->item)
    h->r = AVLinsertR(t, h->r, item);
 // Atualização do número de nós e da altura
  h->N++;
  h->height = atualizaAltura(h);
  // Cálculo do fator de balanceamento
  int fb = fatorBalanceamento(h);

  // Balanceamento
  if (fb > 1) {
    if (item < h->l->item)
      return rotR(t, h);
    else {
      h->l = rotL(t, h->l);
      return rotR(t, h);
    }
  }
  if (fb < -1) {
    if (item > h->r->item)
      return rotL(t, h);
    else {
      h->r = rotR(t, h->r);
      return rotL(t, h);
    }
  }
  return h;
}

  // Função de inserção AVL
link AVLinsert(Tree t, link h, int item) {
  if (h == t->z) {
    return novoNo(item, t->z, t->z);
  }
  // Chamando a função de inserção recursiva
  h = AVLinsertR(t, h, item);
  return h;
}
