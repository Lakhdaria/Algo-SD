#include <stdio.h>

int main() {
    // Déclaration et initialisation
    int array[5] = {10, 20, 30, 40, 50};

    // Accès aux éléments (O(1))
    printf("Premier élément : %d\n", array[0]);

    // Parcours du tableau
    for (int i = 0; i < 5; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }

    return 0;
}