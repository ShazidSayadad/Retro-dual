#include "raylib.h"
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define PI 3.14

Texture2D Background;
Texture2D Background2;
Texture2D Background3;
Texture2D BallIMG;
Texture2D Title;
Texture2D Instruction;
Texture2D Power;

bool isStart = false;
bool timerStart = false;
bool power_up_start = false, powerup_done = false;
bool draw_power_up = false;
int player1_score = 0;
int player2_score = 0;
int ball_direction = 1;
int powerup_direction = 0;
int Visible_paddle = 0, Short_paddle = 0;
int powerUP_no = 0;
int timer = 300;
int default_draw = 1;
int powX, powY;

typedef enum GameScreen { MENU = 0, INSTRUCTION, GAME, END, WINNING } GameScreen;
typedef enum PowerUP { INVISIBLE = 0, FASTBALL, SHORT_PADDLE } PowerUP;


typedef struct Ball
{
    float ballX;
    float ballY;
    float radius;
    float ballSpeedX;
    float ballSpeedY;

}Ball;

typedef struct Paddle
{
    float paddleX;
    float paddleY;
    float height;
    float width;
    float paddleSpeedY;

}Paddle;
void reset(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle)
{
    ball->ballX = GetScreenWidth() / 2;
    ball->ballY = GetScreenHeight() / 2;
    leftPaddle->paddleX = 10;
    leftPaddle->paddleY = GetScreenHeight() / 2;
    rightPaddle->paddleX = GetScreenWidth() - 10;
    rightPaddle->paddleY = GetScreenHeight() / 2;
    Visible_paddle = 0;
    ball->ballSpeedY = 0;
    isStart = false;
    if (player1_score == 3 && player1_score > player2_score || player1_score < player2_score && player2_score == 3)
        timer = 300;
    power_up_start = false;
    powerup_done = false;
    leftPaddle->height = 80;
    rightPaddle->height = 80;
    Short_paddle = 0;
    powerUP_no = 0;
    default_draw = 1;
    powerup_direction = 0;
}

void Draw_ball(Ball ball)
{
    DrawCircle(ball.ballX, ball.ballY - 8, ball.radius, BLACK);
    DrawTextureEx(BallIMG, Vector2 { ball.ballX - 12, ball.ballY - 20 }, 0, 0.5f, WHITE);

}
void ball_Update(Ball* ball, Paddle* paddle_L, Paddle* paddle_R)
{
    ball->ballX += ball->ballSpeedX;
    ball->ballY += ball->ballSpeedY;

    if (ball->ballY + ball->radius >= GetScreenHeight() || ball->radius - ball->ballY >= 0)
        ball->ballSpeedY *= -1;
    if (ball->ballX >= GetScreenWidth())
    {
        player1_score++;
        ball_direction = -1;
        reset(ball, paddle_L, paddle_R);

    }

    if (ball->ballX <= 0)
    {
        player2_score++;
        ball_direction = 1;
        reset(ball, paddle_L, paddle_R);
    }

}
void Draw_left_paddle(Paddle* paddle)
{

    DrawRectangleRounded(Rectangle { paddle->paddleX - 1, paddle->paddleY - (paddle->height / 2) - 1, paddle->width + 2, paddle->height + 2 }, 0.8, 0, BLACK);
    DrawRectangleRounded(Rectangle { paddle->paddleX, paddle->paddleY - (paddle->height / 2), paddle->width, paddle->height }, 0.8, 0, BLUE);

}
void Draw_right_paddle(Paddle* paddle)
{
    DrawRectangleRounded(Rectangle { paddle->paddleX - 20 - 1, paddle->paddleY - (paddle->height / 2) - 1, paddle->width + 2, paddle->height + 2 }, 0.8, 0, BLACK);
    DrawRectangleRounded(Rectangle { paddle->paddleX - 20, paddle->paddleY - (paddle->height / 2), paddle->width, paddle->height }, 0.8, 0, RED);
}

void left_paddle_Update(Paddle* paddle)
{
    if (IsKeyDown(KEY_W))
        paddle->paddleY -= paddle->paddleSpeedY;
    if (IsKeyDown(KEY_S))
        paddle->paddleY += paddle->paddleSpeedY;
    if ((paddle->paddleY) <= 40)
        paddle->paddleY = 40;
    if (paddle->paddleY + paddle->height / 2 >= GetScreenHeight())
        paddle->paddleY = GetScreenHeight() - paddle->height / 2;

}
void right_paddle_Update(Paddle* paddle)
{
    if (IsKeyDown(KEY_UP))
        paddle->paddleY -= paddle->paddleSpeedY;
    if (IsKeyDown(KEY_DOWN))
        paddle->paddleY += paddle->paddleSpeedY;
    if (paddle->paddleY <= 40)
        paddle->paddleY = 40;
    if (paddle->paddleY + paddle->height / 2 >= GetScreenHeight())
        paddle->paddleY = GetScreenHeight() - paddle->height / 2;

}
void left_collision_check(Ball* ball, Paddle* paddle)
{
    if (CheckCollisionCircleRec(Vector2 { ball->ballX, ball->ballY }, ball->radius, Rectangle { paddle->paddleX, paddle->paddleY - 40, paddle->width, paddle->height }))
    {
        if (ball->ballSpeedX < 0)
        {
            powerup_direction = -1;
            ball->ballSpeedX *= -1;

            ball->ballSpeedY = ((ball->ballY) - (paddle->paddleY)) / (paddle->height / 5) * ball->ballSpeedX;
        }
    }
}
void right_collision_check(Ball* ball, Paddle* paddle)
{
    if (CheckCollisionCircleRec(Vector2 { ball->ballX, ball->ballY }, ball->radius, Rectangle { paddle->paddleX - 20, paddle->paddleY - 40, paddle->width, paddle->height }))
    {
        if (ball->ballSpeedX > 0)
        {
            powerup_direction = 1;
            ball->ballSpeedX *= -1;
            ball->ballSpeedY = ((ball->ballY) - (paddle->paddleY)) / (paddle->height / 5) * -ball->ballSpeedX;
        }
    }
}


int main()
{
    const int width = 1280;
    const int hight = 720;


    InitWindow(width, hight, "Retro Dual");
    InitAudioDevice();
    SetTargetFPS(60);

    Background = LoadTexture("D:/Project/Retro Dual/dirt2.png");
    Background2 = LoadTexture("D:/Project/Retro Dual/sand2-modified.png");
    BallIMG = LoadTexture("D:/Project/Retro Dual/ball.png");
    Background3 = LoadTexture("D:/Project/Retro Dual/bac.png");
    Title = LoadTexture("D:/Project/Retro Dual/Title.png");
    Instruction = LoadTexture("D:/Project/Retro Dual/Instruction.png");
    Power = LoadTexture("D:/Project/Retro Dual/power.png");

    Sound sound = LoadSound("D:/Project/Retro Dual/sound.mp3");

    PlaySound(sound);

    Ball ball;
    ball.ballX = GetScreenWidth() / 2;
    ball.ballY = GetScreenHeight() / 2;
    ball.radius = 10;
    ball.ballSpeedX = 6 * ball_direction;
    ball.ballSpeedY = 0;

    Paddle leftPaddle;
    leftPaddle.paddleX = 10;
    leftPaddle.paddleY = GetScreenHeight() / 2;
    leftPaddle.height = 80;
    leftPaddle.width = 20;
    leftPaddle.paddleSpeedY = 10;

    Paddle rightPaddle;
    rightPaddle.paddleX = GetScreenWidth() - 10;
    rightPaddle.paddleY = GetScreenHeight() / 2;
    rightPaddle.height = 80;
    rightPaddle.width = 20;
    rightPaddle.paddleSpeedY = 10;

    GameScreen current_screen = MENU;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F))
            ToggleFullscreen();

        if (IsKeyPressed(KEY_SPACE) && current_screen == GAME)
            isStart = true;

        if (isStart)
        {
            ball_Update(&ball, &leftPaddle, &rightPaddle);
            left_paddle_Update(&leftPaddle);
            right_paddle_Update(&rightPaddle);
            right_collision_check(&ball, &rightPaddle);
            left_collision_check(&ball, &leftPaddle);
        }

        BeginDrawing();

        ClearBackground(BLACK);
        switch (current_screen)
        {
        case MENU:

            DrawTextureEx(Title, Vector2 { 0, 0 }, 0, 1, WHITE);
            DrawText(TextFormat("Press 'ENTER' to start game"), 180, 500, 60, BLACK);

            if (IsKeyPressed(KEY_ENTER))
            {
                current_screen = INSTRUCTION;
            }
            break;

        case INSTRUCTION:

            DrawTextureEx(Instruction, Vector2 { 0, 0 }, 0, 1, WHITE);
            DrawText("Instructions", 500, 0, 40, BLACK);
            DrawText("W / S - Left paddle movement", 20, 60, 30, BLACK);
            DrawText("Up / Down arrow - Right paddle movement", 20, 130, 30, BLACK);
            DrawText("F - Fullscreen mode", 20, 200, 30, BLACK);
            DrawText("Q - Quit to main menu", 20, 270, 30, BLACK);
            DrawText("Simple pong game. There will be powerups randomly generated on screen.", 10, 350, 30, BLACK);
            DrawText("First to get 5 points wins the game.", 10, 385, 30, BLACK);

            if (IsKeyPressed(KEY_ENTER))
            {
                current_screen = GAME;
            }
            break;

        case GAME:
            DrawFPS(0, 0);
            DrawTextureEx(Background3, Vector2 { 0, 0 }, 0, 1, WHITE);

            Draw_ball(ball);
            if (default_draw == 1)
            {
                Draw_left_paddle(&leftPaddle);
                Draw_right_paddle(&rightPaddle);
            }
            else if (default_draw == 2)
            {
                if (Visible_paddle == -1)
                    Draw_left_paddle(&leftPaddle);
                else
                    Draw_right_paddle(&rightPaddle);
            }

            else if (default_draw == -2)
            {
                if (Short_paddle == 1)
                {
                    leftPaddle.height = 80 / 2;
                    Draw_left_paddle(&leftPaddle);
                    Draw_right_paddle(&rightPaddle);
                }
                else
                {
                    rightPaddle.height = 80 / 2;
                    Draw_left_paddle(&leftPaddle);
                    Draw_right_paddle(&rightPaddle);
                }
            }

            DrawText(TextFormat("Player 1: %i", player1_score), GetScreenWidth() / 4 - 100, 10, 50, BLUE);
            DrawText(TextFormat("Player 2: %i", player2_score), 3 * GetScreenWidth() / 4 - 150, 10, 50, RED);

            if (IsKeyPressed(KEY_Q))
            {
                current_screen = END;
            }
            if (player1_score == 1 || player2_score == 1)
            {
                timerStart = true;
            }
            if (player1_score == 3 || player2_score == 3)
            {
                timerStart = true;
            }

            if (timerStart)
            {
                timer = timer - 1;
                if (timer == 0)
                {
                    power_up_start = true;
                    draw_power_up = true;
                    timerStart = false;
                    powX = (rand() % 1000) + 200;
                    powY = (rand() % 720) + 15;

                }
            }
            if (power_up_start) {

                if (draw_power_up) {

                    DrawTextureEx(Power, Vector2 { 640, 360 }, 0, 0.4f, WHITE);
                }
                if (CheckCollisionCircles(Vector2 { 640, 360 }, 7, Vector2 { ball.ballX, ball.ballY }, ball.radius) && !powerup_done)
                {
                    srand(time(NULL));

                    powerUP_no = (rand() % 2) + 1;

                    if (powerUP_no == 1) {
                        default_draw = -2;
                        draw_power_up = false;
                        powerup_done = true;
                        if (powerup_direction == 1)
                            Short_paddle = 1;
                        else Short_paddle = -1;

                    }

                    else if (powerUP_no == 2) {
                        default_draw = 2;
                        draw_power_up = false;
                        powerup_done = true;
                        if (powerup_direction == 1)
                            Visible_paddle = 1;
                        else Visible_paddle = -1;

                    }
                }
            }

            if (player1_score == 5 || player2_score == 5)
                current_screen = WINNING;
            break;

        case END:
            DrawTextureEx(Instruction, Vector2 { 0, 0 }, 0, 1, WHITE);
            DrawText(TextFormat("WANT to quit?"), 0, 0, 40, WHITE);
            if (IsKeyPressed(KEY_ENTER))
                current_screen = MENU;
            break;

        case WINNING:
            DrawTextureEx(Instruction, Vector2 { 0, 0 }, 0, 1, WHITE);
            if (player1_score > player2_score)
            {
                DrawText(TextFormat("Player 1 is the winner!!!"), GetScreenWidth() / 2 - 460, GetScreenHeight() / 2 - 50, 80, BLUE);
                DrawText(TextFormat("Press ENTER to play again!!!"), GetScreenWidth() / 2 - 240, GetScreenHeight() / 2 - 140, 30, WHITE);
            }
            else
            {
                DrawText(TextFormat("Player 2 is the winner!!!"), GetScreenWidth() / 2 - 460, GetScreenHeight() / 2 - 50, 80, RED);
                DrawText(TextFormat("Press ENTER to play again!!!"), GetScreenWidth() / 2 - 240, GetScreenHeight() / 2 - 140, 30, WHITE);
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                current_screen = MENU;
                player1_score = 0;
                player2_score = 0;
            }
        }

        EndDrawing();

    }
    CloseWindow();
    return 0;
}
