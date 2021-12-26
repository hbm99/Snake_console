
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "queue.h"
#include "stack.h"

//Values used in game execution
int rows, cols, game_over, score, fruits_counter, path_counter, snake_growth_counter;

//World declaration
int **world = NULL;

//Point of the map where the snake is
point snake_head;

//Pointers to the snake
node_t *snake_body, *snake_path;

//Direction arrays
int dir_row[4] = {0, -1, 0, 1};
int dir_col[4] = {1, 0, -1, 0};

void generate_fruits(void);

void create_world(void);

void draw(void);

void bfs(void);

void play(void);

//Prints the map in the console
void draw() {
    system("clear");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int pos = world[i][j];
            switch (pos) {
                case 0:
                    printf(".");
                    break;
                case 1:
                    printf("@");
                    break;
                case 2:
                    printf("0");
                    break;
                default:
                    printf("x");
                    break;
            }
        }
        printf("\n");
    }
    printf("score = %d", score);
    printf("\n");
}

//Generates fruits on random map's positions
void generate_fruits() {
    
    srand((int)time(0));
    int i = 5;
    while (i > 0) {
        point fruit;
        fruit.x = (rand() % rows);
        fruit.y = (rand() % cols);
        if (world[fruit.x][fruit.y] == 0) {
            world[fruit.x][fruit.y] = 3;
            fruits_counter++;
            i--;
        }
    }
}

//Initializes world variables
void create_world() {
    
    world = calloc(rows, sizeof(int *));
    
    for (int i = 0; i < rows; i++) {
        world[i] = calloc(cols, sizeof(int));
    }
    
    snake_head.x = rows/2;
    snake_head.y = cols/2;
    
    snake_body = NULL;
    
    point body;
    body.x = snake_head.x;
    body.y = snake_head.y - 2;
    world[body.x][body.y] = 2;
    enqueue(&snake_body, body);
    
    body.x = snake_head.x;
    body.y = snake_head.y - 1;
    world[body.x][body.y] = 2;
    enqueue(&snake_body, body);
    
    //snake_head
    world[snake_head.x][snake_head.y] = 1;
    enqueue(&snake_body, snake_head);
    
    generate_fruits();
}


//Fills a stack (LIFO structure), with the best path found in BFS cover-tree (from the snake head to the closest fruit)
void fill_path(node_t * v_node) {
    node_t *current = v_node;
    while (current->parent != NULL) {
        push(&snake_path, current->val);
        path_counter++;
        current = current->parent;
    }
}

//Makes exhaustive search of the closest fruit (BFS algorithm)
void bfs() {
    node_t *bfs_queue = NULL;
    int bfs_queue_counter = 0;
    
    int **world_mask = calloc(rows, sizeof(int *));
    
    for (int i = 0; i < rows; i++) {
        world_mask[i] = calloc(cols, sizeof(int));
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            world_mask[i][j] = world[i][j];
        }
    }
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node)
        return;
    new_node->val = snake_head;
    new_node->parent = NULL;
    node_enqueue(&bfs_queue, new_node);
    bfs_queue_counter++;
    
    while (bfs_queue_counter > 0) {
        node_t *u_node = node_dequeue(&bfs_queue);
        bfs_queue_counter--;
        for (int i = 0; i < 4; i++) {
            point v;
            v.x = u_node->val.x + dir_row[i];
            v.y = u_node->val.y + dir_col[i];
            if ((v.x >= 0 && v.x < rows && v.y >= 0 && v.y < cols) && (world_mask[v.x][v.y] == 0 || world_mask[v.x][v.y] == 3)) {
                node_t *v_node = malloc(sizeof(node_t));
                if (!v_node)
                    return;
                v_node->val = v;
                v_node->parent = u_node;
                node_enqueue(&bfs_queue, v_node);
                bfs_queue_counter++;
                
                if (world_mask[v.x][v.y] == 3) {
                    fill_path(v_node);
                    free(bfs_queue);
                    free(world_mask);
                    return;
                }
                world_mask[v.x][v.y] = 1;
            }
        }
    }
    
}

//Checks if a point is a valid move for the snake
int is_valid(point p) {
    if (p.x < 0 || p.x >= rows || p.y < 0 || p.y >= cols || world[p.x][p.y] == 1 || world[p.x][p.y] == 2)
        return 0;
    return 1;
}

//Makes the snake step in every execution of the program
void play() {
    point next_position;
    next_position.x = -1;
    next_position.y = -1;
    if (path_counter == 0)
        bfs();
    if (path_counter == 0) {
        for (int i = 0; i < 4; i++) {
            point possible_move;
            possible_move.x = snake_head.x + dir_row[i];
            possible_move.y = snake_head.y + dir_col[i];
            if (is_valid(possible_move)) {
                next_position = possible_move;
            }
        }
    }
    else {
        next_position = pop(&snake_path);
        path_counter--;
    }
            
    if (next_position.x == -1 && next_position.y == -1) {
        game_over = 1;
        return;
    }
    
    if (snake_growth_counter == 0) {
        point last = dequeue(&snake_body);
        world[last.x][last.y] = 0;
    }
    else
        snake_growth_counter--;
    
    if (world[next_position.x][next_position.y] == 3) {
        snake_growth_counter += 3;
        fruits_counter--;
        score++;
    }
    
    enqueue(&snake_body, next_position);
    world[snake_head.x][snake_head.y] = 2;
    snake_head = next_position;
    world[snake_head.x][snake_head.y] = 1;
    
    if (fruits_counter == 0)
        generate_fruits();
}

int main(int argc, const char * argv[]) {
    
    while (1) {
        printf("World size:\nRows: ");
        scanf("%d", &rows);
        printf("Columns: ");
        scanf("%d", &cols);
        if (rows < 5 || cols < 5) {
            //Too small world for the initial requirements indicated
            printf("World size too small.\n");
        }
        else break;
    }
    
    create_world();
    
    while (!game_over) {
        sleep(1);
        draw();
        play();
    }
    
    return 0;
}
