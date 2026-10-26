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

void resetGame(Snake *snake, bool occupied[ROWS][COLS], Position *food) {
    snake->front = 0;
    snake->rear = -1;
    snake->length = 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            occupied[row][col] = false;
        }
    }
    enqueue(snake, (Position){3, 1});
    enqueue(snake, (Position){3, 2});
    enqueue(snake, (Position){3, 3});
    enqueue(snake, (Position){3, 4});
    
    for (int i = 0; i < snake->length; i++) {
        
        int index = (snake->front + i) % (ROWS * COLS);
        Position pos = snake->body[index];
    
        occupied[pos.row][pos.col] = true;
    }
    do {
        food->row = GetRandomValue(0, ROWS - 1);
        food->col = GetRandomValue(0, COLS - 1);
    } while (occupied[food->row][food->col]);
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
    
    int score = snake.length - 1;

    for (int i = 0; i < snake.length; i++) {
        
        int index = (snake.front + i) % (ROWS * COLS);
        Position pos = snake.body[index];
    
        occupied[pos.row][pos.col] = true;
    }
    
    Position food;
    do {
        food.row = GetRandomValue(0, ROWS - 1);
        food.col = GetRandomValue(0, COLS - 1);
    } while (occupied[food.row][food.col]);
    
    
    InitWindow(COLS * CELL_SIZE, ROWS * CELL_SIZE, "Snake Game");
    SetTargetFPS(60);
    float moveTimer = 0.0f;
    bool gameOver = false;
    bool directionChanged = false;

    while (!WindowShouldClose()) {
        
        if (gameOver && IsKeyPressed(KEY_R)) {
            resetGame(&snake, occupied, &food);
            direction = RIGHT;
            directionChanged = false;
            score = snake.length - 1;
            moveTimer = 0.0f;
            gameOver = false;
        }
        
        if (IsKeyPressed(KEY_UP) && direction != DOWN && !directionChanged) {
            direction = UP;
            directionChanged = true;
        }
        else if (IsKeyPressed(KEY_DOWN) && direction != UP && !directionChanged) {
            direction = DOWN;
            directionChanged = true;
        }
        else if (IsKeyPressed(KEY_LEFT) && direction != RIGHT && !directionChanged) {
            direction = LEFT;
            directionChanged = true;
        }
        else if (IsKeyPressed(KEY_RIGHT) && direction != LEFT && !directionChanged) {
            direction = RIGHT;
            directionChanged = true;
        }
            
            
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
            bool ateFood = (nextHead.row == food.row &&
                nextHead.col == food.col);
                
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
                    (ateFood ||  
                        !(nextHead.row == oldTail.row && nextHead.col == oldTail.col))) {
                    gameOver = true;
                }
                else {
                    if(!ateFood) {
                        occupied[oldTail.row][oldTail.col] = false;
                        dequeue(&snake);
                    }
                    enqueue(&snake, nextHead);
                    occupied[nextHead.row][nextHead.col] = true;
                    if (ateFood) {
                        score++;
                        if(snake.length < ROWS * COLS) {
                            do {
                                food.row = GetRandomValue(0, ROWS - 1);
                                food.col = GetRandomValue(0, COLS - 1);
                            } while (occupied[food.row][food.col]);
                        }else {
                            gameOver = true;
                        }
                    }
                }
            }
            directionChanged = false;
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
        
        DrawCircle(
            food.col * CELL_SIZE + CELL_SIZE / 2,
            food.row * CELL_SIZE + CELL_SIZE / 2,
            CELL_SIZE / 4,
            RED
        );
        DrawText(TextFormat("SCORE: %d", score), 10, 10, 24, BLACK);
        
        if (gameOver) {
            const char *text = "GAME OVER";
            const char *restartText = "PRESS R TO RESTART";

            int fontSize = 50;
            int restartFontSize = 24;

            int textWidth = MeasureText(text, fontSize);
            int x = (GetScreenWidth() - textWidth) / 2;
            int y = GetScreenHeight() / 2 - 50;

            DrawText(text, x, y, fontSize, RED);

            int restartWidth = MeasureText(restartText, restartFontSize);
            int restartX = (GetScreenWidth() - restartWidth) / 2;
            int restartY = y + 70;

            DrawText(restartText, restartX, restartY, restartFontSize, DARKGRAY);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}