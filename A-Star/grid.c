#include "grid.h"
#include <stdio.h>

#define false 0
#define true 1

typedef struct Node {
    uint16_t grid_idx;
    struct Node* parent;
    uint16_t gCost;
    uint16_t hCost; // NOTE: Once max cols and rows are determined, the bits used for this can be restricted
} Node;

//  _   _  _          _      ___ ___  _        __   
// | \ |_ /  |   /\  |_)  /\  |   |  / \ |\ | (_  o 
// |_/ |_ \_ |_ /--\ | \ /--\ |  _|_ \_/ | \| __) o

// ___     ___ _  _                  _           _ ___ ___  _        __ 
//  |  |\ | | |_ |_) |\ |  /\  |    |_ | | |\ | /   |   |  / \ |\ | (_  
// _|_ | \| | |_ | \ | \| /--\ |_   |  |_| | \| \_  |  _|_ \_/ | \| __) 

bool node_less_than(Node* a, Node* b);
bool node_greater_than(Node* a, Node* b);
int pq_parent(int i);
int pq_left_child(int i);
int pq_right_child(int i);
void pq_shift_down(Node* pq, const size_t size, const uint16_t i);
void node_copy(Node* orig, Node* cpy);
void pq_dequeue(Node* pq, size_t* size, Node* result);
void pq_shift_up(Node* pq, int16_t i);

void node_print(Node* node);
void pq_print(Node* pq, size_t size);
void printBits(size_t const size, void const * const ptr);

//  _   _  _ ___      ___ ___ ___  _        __   
// | \ |_ |_  |  |\ |  |   |   |  / \ |\ | (_  o 
// |_/ |_ |  _|_ | \| _|_  |  _|_ \_/ | \| __) o

// ___     ___ _  _                  _           _ ___ ___  _        __ 
//  |  |\ | | |_ |_) |\ |  /\  |    |_ | | |\ | /   |   |  / \ |\ | (_  
// _|_ | \| | |_ | \ | \| /--\ |_   |  |_| | \| \_  |  _|_ \_/ | \| __)


bool node_less_than(Node* a, Node* b) {
    return (a->gCost + a->hCost) < (b->gCost + b->hCost);                                                                         
}

bool node_greater_than(Node* a, Node* b) {
    return (a->gCost + a->hCost) > (b->gCost + b->hCost);
}

int pq_parent(int i)
{
    return (i - 1) / 2;
}

int pq_left_child(int i)
{
    return (2 * i) + 1;
}

int pq_right_child(int i)
{
    return (2 * i) + 2;
}

void pq_shift_down(Node* pq, const size_t size, uint16_t i)
{
    uint16_t minIndex = i;

    while (true) {
        uint16_t l = pq_left_child(i);

        if (l < size && node_less_than(pq + l, pq + minIndex)) {
            minIndex = l;
        }

        uint16_t r = pq_right_child(i);

        if (r < size && node_less_than(pq + r, pq + minIndex)) {
            minIndex = r;
        }

        if (i != minIndex) {
            Node temp = pq[i];
            pq[i] = pq[minIndex];
            pq[minIndex] = temp;
            i = minIndex;
        }
        else {
            break;
        }
    }
}

void node_copy(Node* orig, Node* cpy)
{
    cpy->grid_idx = orig->grid_idx;
    cpy->parent = orig->parent;
    cpy->gCost = orig->gCost;
    cpy->hCost = orig->hCost;
}

void pq_dequeue(Node* pq, size_t* size, Node* result)
{
    node_copy(pq, result);    
    pq[0] = pq[(*size)-1];
    (*size)--;
    pq_shift_down(pq, *size, 0);
}

void pq_shift_up(Node* pq, int16_t i)
{
    while (i > 0 && node_greater_than(pq + pq_parent(i), pq + i)) {
        int16_t parent_i = pq_parent(i);
        Node temp = pq[i];
        pq[i] = pq[parent_i];
        pq[parent_i] = temp;

        i = parent_i;
    }
}

void node_print(Node* node)
{
    printf("[gC: %u gI: %u]\n", node->gCost, node->grid_idx);
}

void pq_print(Node* pq, size_t size)
{
    printf("pq: ");
    for (size_t i = 0; i < size; ++i) {
        node_print(pq+i);
    }
    printf("\n");
}

void printBits(size_t const size, void const * const ptr)
{
    uint8_t *b = (uint8_t*) ptr;
    uint8_t byte;
    int16_t i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    printf("\n");
}

//  _       _     ___  _         _ ___ 
// |_) | | |_) |   |  /     /\  |_) |  
// |   |_| |_) |_ _|_ \_   /--\ |  _|_

bool grid_obstacle_at(const Grid* grid, size_t idx)
{
    const uint8_t bit_idx = (idx % 8);
    const size_t byte_idx = idx / 8;
    return (grid->obstacles[byte_idx] & (1 << bit_idx)) >> bit_idx;
}

void grid_print(const Grid* grid)
{    
    for (size_t i = 0; i < (grid->cols * grid->rows); ++i) {     
        printf("%s", grid_obstacle_at(grid, i) ? "# " : ". ");
        if ((i+1) % grid->cols == 0) {
            printf("\n");
        }
    }
}

void grid_print_mark(const Grid* grid, const uint16_t marked)
{    
    for (size_t i = 0; i < (grid->cols * grid->rows); ++i) {
        if (i == marked) {            
            printf("@ ");
        }
        else {
            printf("%s", grid_obstacle_at(grid, i) ? "# " : ". ");
        }
        if ((i+1) % grid->cols == 0) {
            printf("\n");
        }
    }
}

void grid_print_path(const Grid* grid, const uint16_t* path, const uint16_t path_size)
{
    for (size_t i = 0; i < (grid->cols * grid->rows); ++i) {
        bool in_path = false;
        for (size_t j = 0; j < path_size; ++j) {
            if (path[j] == i) {
                in_path = true;
                break;
            }
        }
        
        if (in_path) {
            printf("@ ");
        }
        else {
            printf("%s", grid_obstacle_at(grid, i) ? "# " : ". ");
        }
        if ((i+1) % grid->cols == 0) {
            printf("\n");
        }
    }
}

Err grid_init_str(char* s, Grid* grid)
{
    grid->cols = 0;
    grid->rows = 0;
    uint8_t end_row_found = 0;
    for (size_t i = 0; i < strlen(s); ++i) {
        if (s[i] == '\n') {
            end_row_found = 1;
            grid->rows++;
        }
        if (!end_row_found) {
            if (s[i] != ' ') {
                grid->cols++;
            }
        }
    }
    grid->rows++;
    size_t grid_size = (grid->cols * grid->rows * sizeof(uint8_t)) / 8 + 1;
    grid->obstacles = (uint8_t*) malloc(grid_size);
    for (size_t i = 0; i < grid_size; ++i) {
        grid->obstacles[i] = 0;
    }

    char delim[1] = {'\n'};
    char* row = strtok(s, delim);
    
    int16_t j = 0;  // byte index
    uint8_t k = 0; // bit index
    while (row != NULL) {
        for (size_t i = 0; i < strlen(row); ++i) {
            switch (row[i]) {
            case ' ':
                continue;
                break;
            case '.':
                if (++k == 8) {
                    k = 0;
                    ++j;
                }
                break;
            case '#':
                grid->obstacles[j] = grid->obstacles[j] | (1<<k);
                if (++k == 8) {
                    k = 0;
                    ++j;
                }
                break;
            default:
                printf("Unexpected char: %c\n", row[i]);

                return 1;
            }
        }
        
        row = strtok(NULL, delim);
    }

    return 0;
}

void grid_idx_to_cartesian(const Grid* grid, const uint16_t i, int16_t* x, int16_t* y)
{
    *x = i % grid->cols;
    *y = i / grid->cols;    
}

uint16_t grid_distance(const Grid* grid, const uint16_t i, const uint16_t j)
{
    int16_t ix;
    int16_t iy;
    grid_idx_to_cartesian(grid, i, &ix, &iy);
    int16_t jx;
    int16_t jy;
    grid_idx_to_cartesian(grid, j, &jx, &jy);

    return round(10 * sqrt(pow((float)abs(jy - iy), 2) + pow((float)abs(jx - ix), 2)));    
}

Err grid_find_path_a_star(const Grid* grid, const uint16_t start, const uint16_t dest, uint16_t* path, uint16_t* path_size, const uint16_t max_path_size)
{
    if (grid_obstacle_at(grid, start)) {
        return -2;
    }
    if (grid_obstacle_at(grid, dest)) {
        return -3;
    }
    // 5 6 7
    // 4 X 0
    // 3 2 1
    int16_t neighbors[8];
    neighbors[0] = 1;
    neighbors[1] = grid->cols + 1;
    neighbors[2] = grid->cols;
    neighbors[3] = grid->cols - 1;
    neighbors[4] = -1;
    neighbors[5] = 0 - grid->cols - 1;
    neighbors[6] = 0 - grid->cols;
    neighbors[7] = 0 - grid->cols + 1;

    const uint16_t pq_max_size = 8000;
    const uint16_t explored_max_size = pq_max_size; 

    Node pq[pq_max_size];
    pq[0].grid_idx = dest;
    pq[0].parent = NULL;
    pq[0].gCost = 0;
    pq[0].hCost = grid_distance(grid, dest, start);
    size_t pq_size = 1;
    
    Node explored[explored_max_size];
    uint16_t explored_size = 0;

    while (pq_size != 0) {
        if (explored_size >= explored_max_size) {
            return -5;
        }
        pq_dequeue(pq, &pq_size, explored + explored_size);
        explored_size++;
        
        for (uint8_t i = 0; i < 8; ++i) {
            if (explored[explored_size-1].grid_idx < grid->cols && i >= 5) continue; // first row
            if (explored[explored_size-1].grid_idx % grid->cols == 0 && i >= 3 && i <= 5) continue; // first column
            if (explored[explored_size-1].grid_idx % grid->cols == grid->cols - 1 && (i <= 1 || i == 7)) continue; // last column
            if (explored[explored_size-1].grid_idx / grid->cols == grid->rows - 1 && (i <= 3 && i >= 1)) continue; // last row
        
            size_t neighbor_idx = explored[explored_size - 1].grid_idx + neighbors[i];            
            if (grid_obstacle_at(grid, neighbor_idx)) continue;            

            if (neighbor_idx == start) { 
                path[0] = neighbor_idx;                
                (*path_size)++;
                Node* node_ptr = explored + (explored_size - 1);
                for (int i = 1; node_ptr != NULL && *path_size < max_path_size; ++i) {
                    path[i] = node_ptr->grid_idx;                    
                    node_ptr = node_ptr->parent;
                    (*path_size)++;
                    if (*path_size == max_path_size && node_ptr != NULL) {
                        return -1;
                    }
                }                

                return 0;
            }

            // ask whether the neighbor_idx exists in the pq already
            bool in_pq = false;
            size_t pq_idx = 0;
            for (size_t i = 0; i < pq_size; ++i) {
                if (pq[i].grid_idx == neighbor_idx) {
                    in_pq = true;
                    pq_idx = i;
                    break;
                }
            }                        
            // If so, the gCost and parent may need to be updated to reflect a lower cost path
            if (in_pq) { // when switching to using fCost, this needs to be updated
                uint16_t new_gCost = explored[explored_size-1].gCost + grid_distance(grid, explored[explored_size-1].grid_idx, neighbor_idx);
                if (new_gCost < pq[pq_idx].gCost) {
                    pq[pq_idx].gCost = new_gCost;
                    pq[pq_idx].parent = explored+(explored_size-1);
                    pq_shift_up(pq, pq_idx);                    
                }
            }
            else { // otherwise, add a new node to the queue as below
                pq[pq_size].grid_idx = neighbor_idx;
                pq[pq_size].parent = explored+(explored_size-1);
                pq[pq_size].gCost = explored[explored_size-1].gCost + grid_distance(grid, explored[explored_size-1].grid_idx, neighbor_idx);
                pq[pq_size].hCost = grid_distance(grid, neighbor_idx, start);
                pq_shift_up(pq, pq_size);
                ++pq_size;
                if (pq_size == pq_max_size) {
                    return -4;
                }
            }
        }
    }
    
    return 1;
}
