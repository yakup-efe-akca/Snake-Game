#include "raylib.h"

#define CELL_SIZE 60
#define ROWS 15
#define COLS 20

typedef struct {
    int row;
    int col;
} Position;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    Position body[ROWS*COLS];
    int front;
    int rear;
    int length;
} Snake;

void enqueue(Snake *snake, Position pos) {
    if(snake->length == ROWS * COLS) {
        return;
    }
    snake->rear = (snake->rear + 1) % (ROWS * COLS);
    snake->body[snake->rear] = pos;
    snake->length += 1;
}

void dequeue(Snake *snake) {
    if(snake->length == 0) {
        return;
    }
    snake->front = (snake->front + 1) % (ROWS * COLS);
    snake->length -= 1;
}

int main(void) {

    Direction direction = RIGHT;
    bool occupied[ROWS][COLS] = {false};
    
    Snake snake;
    snake.front = 0;
    snake.rear = -1;
    snake.length = 0;

    enqueue(&snake, (Position){3, 1});
    enqueue(&snake, (Position){3, 2});
    enqueue(&snake, (Position){3, 3});
    enqueue(&snake, (Position){3, 4});

    for (int i = 0; i < snake.length; i++) {
        
        int index = (snake.front + i) % (ROWS * COLS);
        Position pos = snake.body[index];
    
        occupied[pos.row][pos.col] = true;
    }

    InitWindow(COLS * CELL_SIZE, ROWS * CELL_SIZE, "Snake Game");
    SetTargetFPS(60);
    float moveTimer = 0.0f;
    bool gameOver = false;

    while (!WindowShouldClose()) {
        
        if (IsKeyPressed(KEY_UP) && direction != DOWN)
            direction = UP;
        else if (IsKeyPressed(KEY_DOWN) && direction != UP)
            direction = DOWN;
        else if (IsKeyPressed(KEY_LEFT) && direction != RIGHT)
            direction = LEFT;
        else if (IsKeyPressed(KEY_RIGHT) && direction != LEFT)
            direction = RIGHT;
            
            
        moveTimer += GetFrameTime();
        if (moveTimer >= 0.2f && !gameOver) {
            
            Position nextHead = snake.body[snake.rear];
            if (direction == RIGHT) {
                nextHead.col++;
            }
            else if (direction == LEFT) {
                nextHead.col--;
            }
            else if (direction == UP) {
                nextHead.row--;
            }
            else if (direction == DOWN) {
                nextHead.row++;
            }
            moveTimer -= 0.2f;
            if(nextHead.row < 0 ||
                    nextHead.row >= ROWS || 
                    nextHead.col < 0 || 
                    nextHead.col >= COLS) {
                gameOver = true;
            }
            else {
                Position oldTail = snake.body[snake.front];
                if(occupied[nextHead.row][nextHead.col] &&
                    !(nextHead.row == oldTail.row && nextHead.col == oldTail.col)) {
                    gameOver = true;
                }
                else {
                    occupied[oldTail.row][oldTail.col] = false;
                    dequeue(&snake);
                    enqueue(&snake, nextHead);
                    occupied[nextHead.row][nextHead.col] = true;
                }
            }
        }
        

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                DrawRectangleLines(
                    col * CELL_SIZE,
                    row * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    DARKGRAY
                );
            }
        }
        
        for (int i = 0; i < snake.length; i++) {
            int index = (snake.front + i) % (ROWS * COLS);
            Position pos = snake.body[index];
            DrawRectangle(pos.col * CELL_SIZE, pos.row * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
        }
        
        if (gameOver) {
            const char *text = "GAME OVER";
            int fontSize = 50;

            int textWidth = MeasureText(text, fontSize);

            int x = (GetScreenWidth() - textWidth) / 2;
            int y = (GetScreenHeight() - fontSize) / 2;

            DrawText(text, x, y, fontSize, RED);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}