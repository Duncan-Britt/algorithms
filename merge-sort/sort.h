#ifndef guard_sort_h
#define guard_sort_h

void merge_sort(int xs[], int size);
void merge(int xs[], const int mid, int end);
void copy_from(int src[], int dst[], int size);

void merge_sort(int xs[], int size) {
    if (size == 1) return;
    
    merge_sort(xs, size/2);    
    int size2 = (size % 2 == 0 ? size/2 : size/2 + 1);
    merge_sort(xs + (size/2), size2);

    return merge(xs, size/2, size);
}

void merge(int xs[], const int mid, const int end) {
    int b = 0;        
    int m = mid;
    int tmp[end];
    int i;

    for (i = 0; b < mid && m < end; ++i) {
        if (xs[b] <= xs[m]) {
            tmp[i] = xs[b];
            ++b;
        } else {            
            tmp[i] = xs[m];
            ++m;
        }        
    }
    
    if (m == end) {
        while (b < mid) {
            tmp[i] = xs[b];
            ++b;
            ++i;
        }
    } else if (b == mid) {
        while (m < end) {
            tmp[i] = xs[m];
            ++m;
            ++i;
        }
    }

    copy_from(tmp, xs, end);
}

void copy_from(int src[], int dst[], int size) {
    int i;
    for (i = 0; i < size; ++i) {
        dst[i] = src[i];
    }
}

#endif/*guard_sort_h*/

