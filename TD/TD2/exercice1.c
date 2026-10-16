/* TD2 - Exercice 1 : The Lost Artifact
 * Compilation : gcc -O2 -Wall exercice1.c -o exercice1 -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* Q1 - Preparing the search system. Implement is_sorted_increasing, which returns whether
the array is strictly increasing. */
int is_sorted_increasing(const int *A, int n)
{
    for (int i = 1; i < n; i++)
        if (A[i - 1] >= A[i])
            return 0;
    return 1;
}

/* Q2 - Implement a search that examines the elements sequentially. Determine its
worst-case time complexity.
 * Pire cas : x absent ou en dernière position -> n comparaisons -> O(n) */
int linear_search(const int *A, int n, int x)
{
    for (int i = 0; i < n; i++)
        if (A[i] == x)
            return i;
    return -1;
}

/* Q3 - For a sorted array, jump forward in blocks of size ⌊√n⌋, then search sequen-
tially inside the relevant block. Implement this algorithm and determine its worst-case time
complexity. First verify that the array is strictly increasing; otherwise return −1. */
int jump_search_nocheck(const int *A, int n, int x)
{
    int step = (int)sqrt((double)n);
    if (step < 1)
        step = 1;

    int prev = 0, next = step;
    /* On saute tant que le dernier élément du bloc courant est < x */
    while (next < n && A[next - 1] < x) {
        prev = next;
        next += step;
    }
    if (next > n)
        next = n;

    /* Recherche séquentielle dans le bloc [prev, next[ */
    for (int i = prev; i < next && A[i] <= x; i++)
        if (A[i] == x)
            return i;
    return -1;
}

int jump_search(const int *A, int n, int x)
{
    if (!is_sorted_increasing(A, n))
        return -1;
    return jump_search_nocheck(A, n, x);
}

/* Q4 - Implement iterative binary search by repeatedly eliminating half of the
remaining interval. Determine its worst-case time complexity. First verify that the array is
strictly increasing; otherwise return −1 */
int binary_search_nocheck(const int *A, int n, int x)
{
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;   /* évite le dépassement de lo+hi */
        if (A[mid] == x)
            return mid;
        if (A[mid] < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}

int binary_search(const int *A, int n, int x)
{
    if (!is_sorted_increasing(A, n))
        return -1;
    return binary_search_nocheck(A, n, x);
}

/* Q5 - Generate sorted arrays of increasing sizes and compare the
execution times of the three algorithms in seconds, using clock()1. Discuss the effect of
sortedness verification on the measurements. */

/* Tableau trié strictement croissant : A[i] = 2i (valeurs distinctes) */
static void fill_sorted(int *A, int n)
{
    for (int i = 0; i < n; i++)
        A[i] = 2 * i;
}

typedef int (*search_fn)(const int *, int, int);

/* Temps moyen d'une recherche, en secondes.
 * On répète la recherche car une seule exécution est trop rapide
 * pour la résolution de clock(). */
static double time_search(search_fn f, const int *A, int n, int x, int reps)
{
    volatile int sink = 0;
    clock_t start = clock();
    for (int r = 0; r < reps; r++)
        sink += f(A, n, x);
    clock_t end = clock();
    (void)sink;
    return (double)(end - start) / CLOCKS_PER_SEC / reps;
}

int main(void)
{
    /* Petits tests de validité */
    int T[] = {1, 3, 5, 7, 9, 11};
    int U[] = {4, 1, 9, 2};
    printf("Tests :\n");
    printf("  linear(7)  = %d (attendu 3)\n", linear_search(T, 6, 7));
    printf("  jump(11)   = %d (attendu 5)\n", jump_search(T, 6, 11));
    printf("  binary(1)  = %d (attendu 0)\n", binary_search(T, 6, 1));
    printf("  binary(4)  = %d (attendu -1)\n", binary_search(T, 6, 4));
    printf("  linear non trié(9) = %d (attendu 2)\n", linear_search(U, 4, 9));
    printf("  jump non trié(9)   = %d (attendu -1)\n\n", jump_search(U, 4, 9));

    int sizes[] = {1000, 10000, 100000, 1000000};
    int nb = sizeof sizes / sizeof sizes[0];
    int reps = 2000;

    printf("Pire cas (x absent, plus grand que tous les éléments), temps moyen par recherche en secondes\n\n");
    printf("%9s | %11s | %11s %11s | %11s %11s\n",
           "n", "lineaire", "jump+verif", "jump seul", "binary+verif", "binary seul");
    printf("----------+-------------+-------------------------+-------------------------\n");

    for (int k = 0; k < nb; k++) {
        int n = sizes[k];
        int *A = malloc(n * sizeof *A);
        if (!A) return 1;
        fill_sorted(A, n);
        int x = 2 * n + 1;  /* absent -> pire cas pour les trois */

        double tl  = time_search(linear_search, A, n, x, reps);
        double tjv = time_search(jump_search, A, n, x, reps);
        double tj  = time_search(jump_search_nocheck, A, n, x, reps * 100);
        double tbv = time_search(binary_search, A, n, x, reps);
        double tb  = time_search(binary_search_nocheck, A, n, x, reps * 100);

        printf("%9d | %11.3e | %11.3e %11.3e | %11.3e %11.3e\n",
               n, tl, tjv, tj, tbv, tb);
        free(A);
    }
    return 0;
}
