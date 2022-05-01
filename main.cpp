#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
#include <list>
#include <iostream>
const int WINDOW_W = 800;
const int WINDOW_H = 800;

struct Apple {
    Vector2 pos;
    float radius;
    Color col;
    Apple(Vector2 p0, float r0=10, Color c0=YELLOW): pos(p0), radius(r0), col(c0){}

    void draw() {
        DrawCircleV(pos, radius, col);
    }
};

struct Snake{
    
    std::list<Vector2> body;
    Color col;
    float radius = 10;
    Vector2 direction;
    Snake(Vector2 start_pos, int start_len=3, Vector2 dir = {0,1}, Color col0=GREEN) {
        direction = dir/std::max(dir.x, dir.y);
        for (int i = 0; i < start_len; i++)
            body.push_back(start_pos-Vector2{0,radius*2*(i)});
        col = col0;
    }

    void draw() {
        DrawCircleV(body.front(), radius, RED);

        for (auto it = ++body.begin(); it != body.end(); ++it) {
            DrawCircleV((*it), radius, col);
        }
    }

    void move() {
        body.pop_back();
        body.push_front(body.front()+direction*radius*2);
    }

    void insert() {
        body.push_back(body.back()-direction*radius*2);
    }

    bool colision_bonus(Apple apl) {
        if (
            Vector2Length(body.front() - apl.pos) < radius + apl.radius
            )
        {
            insert();
            return true;
        }
        return false;
    }

};




int main()
{
    InitWindow(WINDOW_W, WINDOW_H, "Creative Coding: Game of Life");
    SetTargetFPS(60);

    // Game state
    bool is_running = true;
    float timeout = 0;
    int score = 0;
    Snake snake({WINDOW_W / 2, WINDOW_H / 2}, 3);
    Apple apl(Vector2{float(rand()%WINDOW_W),float(rand() % WINDOW_H)});

    while (!WindowShouldClose()) {

        // Change game state
        if (is_running) {
            if (IsKeyPressed(KEY_W) && !(snake.direction == Vector2{ 0, 1 }))
                snake.direction = { 0,-1 };
            else if (IsKeyPressed(KEY_S) && !(snake.direction == Vector2{ 0, -1 }))
                snake.direction = { 0,1 };
            else if (IsKeyPressed(KEY_A) && !(snake.direction == Vector2{ 1, 0 }))
                snake.direction = { -1,0 };
            else if (IsKeyPressed(KEY_D) && !(snake.direction == Vector2{ -1, 0 }))
                snake.direction = { 1,0 };
        }
        
        if (IsKeyPressed(KEY_SPACE))
            is_running =! is_running;

        BeginDrawing();
        {
            ClearBackground(BLACK);
            snake.draw();
            apl.draw();
        }
        EndDrawing();
        
        // Update frame only after timeout
        if (timeout <= 0 && is_running) {
            timeout = 0.1;
        }
        else {
            timeout -= GetFrameTime();
            continue;
        }
        snake.move();
        if (snake.colision_bonus(apl)) {
            apl.pos = Vector2{ float(rand() % WINDOW_W),float(rand() % WINDOW_H) };
        }
    }
    CloseWindow();

    return 0;
}