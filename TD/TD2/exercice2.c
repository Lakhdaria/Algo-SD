/* TD2 - Exercice 2 : The Tournament Rankings
 * Compilation : gcc -O2 -Wall exercice2.c -o exercice2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
/* Q1 - Before implementing the sorting algorithms, implement the following utility functions:
• swap_int: exchanges the values of two integers.
• is_sorted_ascending: checks whether an integer array is sorted in ascending order.
• copy_array: copies the contents of one integer array into another. */
 
void swap_int(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
 
int is_sorted_ascending(const int *A, int n)
{
    for (int i = 1; i < n; i++)
        if (A[i - 1] > A[i])   /* > et pas >= : les doublons sont autorisés */
            return 0;
    return 1;
}
 
void copy_array(int *dst, const int *src, int n)
{
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}
 
/* Q2 - The organizers first decide to repeatedly select the participant with the lowest remaining score
and place that score at its correct position.
Implement selection sort using an iterative approach.
Determine its worst-case time complexity.
 * Pour chaque i, on cherche le minimum de A[i..n-1] : (n-1)+(n-2)+...+1
 * = n(n-1)/2 comparaisons quel que soit le tableau -> O(n²) (pire et meilleur cas) */
void selection_sort(int *A, int n)
{
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (A[j] < A[min])
                min = j;
        if (min != i)
            swap_int(&A[i], &A[min]);
    }
}
 
/* Q3 - Another organizer proposes processing the scores one by one, inserting each new score into its
correct position among the previously sorted scores.
Implement insertion sort using an iterative approach.
Determine its best-case and worst-case time complexities.
 * Meilleur cas : tableau déjà trié, une comparaison par élément -> O(n)
 * Pire cas : tableau trié à l'envers, chaque élément remonte jusqu'au début
 * -> 1+2+...+(n-1) = n(n-1)/2 décalages -> O(n²) */
void insertion_sort(int *A, int n)
{
    for (int i = 1; i < n; i++) {
        int key = A[i];
        int j = i - 1;
        while (j >= 0 && A[j] > key) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = key;
    }
}
 
/* Q4 - A third organizer suggests repeatedly comparing neighboring scores and swapping them when-
ever they are in the wrong order.
Implement bubble sort using an iterative approach.
Determine its worst-case time complexity.
 * Pire cas (tableau inversé) : n-1 passes, n(n-1)/2 comparaisons et échanges -> O(n²)
 * Avec l'arrêt anticipé (aucun échange dans une passe), meilleur cas O(n). */
void bubble_sort(int *A, int n)
{
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (A[j] > A[j + 1]) {
                swap_int(&A[j], &A[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }
}
 
/* Q5 - As the tournament grows, the organizers need a more efficient sorting algorithm.
One strategy is to divide the scores into two halves, sort each half independently, and then
merge the two sorted halves.
Implement merge sort using a recursive approach.
Determine its worst-case time complexity.
 * T(n) = 2T(n/2) + O(n) : log2(n) niveaux de récursion, O(n) de fusion par niveau
 * -> O(n log n) dans tous les cas. Mémoire auxiliaire O(n). */
static void merge(int *A, int *tmp, int left, int mid, int right)
{
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right)
        tmp[k++] = (A[i] <= A[j]) ? A[i++] : A[j++];  /* <= : tri stable */
    while (i <= mid)
        tmp[k++] = A[i++];
    while (j <= right)
        tmp[k++] = A[j++];
    for (k = left; k <= right; k++)
        A[k] = tmp[k];
}
 
static void merge_sort_rec(int *A, int *tmp, int left, int right)
{
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    merge_sort_rec(A, tmp, left, mid);
    merge_sort_rec(A, tmp, mid + 1, right);
    merge(A, tmp, left, mid, right);
}
 
void merge_sort(int *A, int n)
{
    int *tmp = malloc(n * sizeof *tmp);
    if (!tmp)
        return;
    merge_sort_rec(A, tmp, 0, n - 1);
    free(tmp);
}
 
/* Q6 - Another strategy consists of selecting a pivot, partitioning the scores around it, and recursively
sorting the resulting partitions.
Implement quick sort using a recursive approach.
Determine its best-case and worst-case time complexities.
 * Meilleur cas : le pivot coupe toujours en deux moitiés égales
 * -> T(n) = 2T(n/2) + O(n) -> O(n log n)
 * Pire cas : le pivot est toujours le min ou le max, une partition est vide
 * -> T(n) = T(n-1) + O(n) -> O(n²)
 * Prendre le pivot au milieu évite le pire cas sur un tableau déjà trié. */
static int partition(int *A, int low, int high)
{
    int mid = low + (high - low) / 2;
    swap_int(&A[mid], &A[high]);       /* pivot placé à la fin */
    int pivot = A[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (A[j] <= pivot) {
            i++;
            swap_int(&A[i], &A[j]);
        }
    }
    swap_int(&A[i + 1], &A[high]);
    return i + 1;
}
 
static void quick_sort_rec(int *A, int low, int high)
{
    if (low < high) {
        int p = partition(A, low, high);
        quick_sort_rec(A, low, p - 1);
        quick_sort_rec(A, p + 1, high);
    }
}
 
void quick_sort(int *A, int n)
{
    quick_sort_rec(A, 0, n - 1);
}
 
/* Q7 - Generate arrays of increasing sizes containing randomly generated scores.
For each array, compare the execution times of the five sorting algorithms.
Measure execution times in seconds using clock()2.
To ensure a fair comparison, each algorithm must receive an identical copy of the original
unsorted array.
Compare your experimental observations with the theoretical time complexities. */
 
typedef void (*sort_fn)(int *, int);
 
static double time_sort(sort_fn f, int *work, const int *original, int n)
{
    copy_array(work, original, n);    
    clock_t start = clock();
    f(work, n);
    clock_t end = clock();
    if (!is_sorted_ascending(work, n))
        printf("  ERREUR : tableau mal trié\n");
    return (double)(end - start) / CLOCKS_PER_SEC;
}
 
int main(void)
{
    srand((unsigned)time(NULL));
 
    const char *names[] = {"Selection", "Insertion", "Bulles", "Fusion", "Rapide"};
    sort_fn sorts[] = {selection_sort, insertion_sort, bubble_sort, merge_sort, quick_sort};
    int nsorts = 5;
 
    int sizes[] = {1000, 5000, 10000, 50000, 100000};
    int nb = sizeof sizes / sizeof sizes[0];
 
    printf("Temps d'exécution en secondes (scores aléatoires entre 0 et 10^6)\n\n");
    printf("%8s", "n");
    for (int s = 0; s < nsorts; s++)
        printf(" | %10s", names[s]);
    printf("\n---------");
    for (int s = 0; s < nsorts; s++)
        printf("+------------");
    printf("\n");
 
    for (int k = 0; k < nb; k++) {
        int n = sizes[k];
        int *original = malloc(n * sizeof *original);
        int *work = malloc(n * sizeof *work);
        if (!original || !work) return 1;
 
        for (int i = 0; i < n; i++)
            original[i] = rand() % 1000001;
 
        printf("%8d", n);
        for (int s = 0; s < nsorts; s++) {
            printf(" | %10.4f", time_sort(sorts[s], work, original, n));
            fflush(stdout);
        }
        printf("\n");
 
        free(original);
        free(work);
    }
    return 0;
}