#ifndef guard_quick_sort_h
#define guard_quick_sort_h

void swap(int *i, int *j);
void quick_sort(int *b, int *e);
int *qs_partition(int *b, int *e);

int *qs_partition(int *b, int *e) {
    int *pivot = b;
    int *i = b;
    int *j = e;
    
    while (i < j) {
        do {
            ++i;
        } while (i < e-1 && *i <= *pivot);
        
        do {
            --j;
        } while(j > b && *j >= *pivot);
        
        /* If j gets to b and every j is >= *pivot, then the pivot should stay where it is */        
        if (j == b) {            
            break;
        }

        /* At this point, we know that i != j because *i > *p and *j < *p, so *i != *j */
        /* However, they may have crossed. In the event they have crossed, we should not swap them. */
        if (i < j) {
            swap(i, j);
        }
    }
    /* At this point we know: */
    /*   i > j => because i > *p and j < *p, so i != j, and also i !< j because of while condition */    
    /*   everything left of j is <= *p because everything else was swapped on the way to i crossing j */
    /* so we can swap pivot with j */
    return j;
}

void quick_sort(int *b, int *e) {    
    if (e - b <= 1) {
        return;
    }
    
    int *j = qs_partition(b, e);    /* +n time */
    swap(b, j); /* pivot moves from b to its proper place, j */
    
    quick_sort(b, j); /* T(n-4) */
    quick_sort(j+1, e); /* T(3) */
}

void swap(int *i, int *j) {
    int tmp = *j;
    *j = *i;
    *i = tmp;
}

#endif/*guard_quick_sort_h*/
