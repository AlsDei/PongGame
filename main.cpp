#include <raylib.h>
#include <iostream>
#include "button.hpp"
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int MAX_SCORE = 5; 

using namespace std;

Color back = Color{161, 45, 45, 255};
Color kiri = Color{196, 73, 73, 255};
Color sky_blue = Color{214, 214, 214, 200};
Color Ballnpaddle = Color{224, 225, 221, 255};

int p1score = 0;
int p2score = 0;
int cround = 1;

Sound ugh;



class balls {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void circle() {
        DrawCircle(x, y, radius, Ballnpaddle);
    }

    void update() {
        x += speed_x;
        y += speed_y;
        
        if (x + radius > GetScreenWidth()+100) {
            PlaySound(ugh);
            speed_x *= -1;
            p1score += 1;
            cround += 1;
            reset();
    
        }

        if (x - radius < -100) {
            PlaySound(ugh);
            speed_x *= -1;
            p2score += 1;
            cround += 1;
            reset();
         
        }

        if (y + radius > GetScreenHeight() || y - radius < 0) {
            speed_y *= -1;
        }
     
    }

    void reset() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x = 10 * speed_choices[GetRandomValue(0, 1)];
        speed_y = 10 * speed_choices[GetRandomValue(0, 1)];
    }
};

class paddle {
public:
    float x, y;
    float width, height;
    int speed;

    void pads() {
        DrawRectangle(x, y, width, height, Ballnpaddle);
    }

    void update1() {
        if (IsKeyDown(KEY_W) && y > 0) {
            y -= speed;
        }
        if (IsKeyDown(KEY_S) && y < SCREEN_HEIGHT - height) {
            y += speed;
        }
    }

    void update2() {
        if (IsKeyDown(KEY_I) && y > 0) {
            y -= speed;
        }
        if (IsKeyDown(KEY_K) && y < SCREEN_HEIGHT - height) {
            y += speed;
        }
    }
};

balls ball;
paddle p1;
paddle p2;

void resetGame() {
    p1score = 0;
    p2score = 0;
    cround = 1;
    ball.reset();
}



int main() 
{
     cout << "Starting...." << endl;

    ball.radius = 20;
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.speed_x = 10;
    ball.speed_y = 10;

    p1.x = 10;
    p1.y = SCREEN_HEIGHT / 2 - 60;
    p1.width = 25;
    p1.height = 200;
    p1.speed = 15;

    p2.x = SCREEN_WIDTH - 35;
    p2.y = SCREEN_HEIGHT / 2 - 60;
    p2.width = 25;
    p2.height = 200;
    p2.speed = 15;

    
    InitWindow(1920,1080,"PONG!");
    InitAudioDevice();

    Sound Hit1Sound = LoadSound("Sounds/Hit1.mp3");
    Sound Hit2Sound = LoadSound("Sounds/Hit2.mp3");;
    Sound BGM = LoadSound("Sounds/BGM.mp3");
    Sound Ingamebgm = LoadSound("Sounds/InGameBGM.mp3");

    ugh = LoadSound("Sounds/Ugh.mp3");


    SetTargetFPS(120);
    PlaySound(BGM);
    Texture2D background = LoadTexture("Graphics/Pong-Menu.png");
    Button startButton("Graphics/start.png", {725, 410}, 0.49);
    Button quitButton("Graphics/EXIT.png", {725, 590}, 0.49);
    bool exit = false;

    while(!WindowShouldClose() && !exit){
        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        
        if(startButton.isPressed(mousePosition, mousePressed))
        {
            while (!WindowShouldClose()) {
        
        BeginDrawing();
        StopSound(BGM);
        PlaySound(BGM);
        
        ball.update();
        p1.update1();
        p2.update2();

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{p1.x, p1.y, p1.width, p1.height})) {
            PlaySound(Hit1Sound);
            ball.speed_x *= -1.1;
            ball.x += ball.speed_x;
        } 

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{p2.x, p2.y, p2.width, p2.height})) {
            PlaySound(Hit2Sound);
            ball.speed_x *= -1.1;
            ball.x += ball.speed_x;
        } 

     
        
        

        ClearBackground(back);
        DrawRectangle(0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, kiri);
        DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 200, sky_blue);
        ball.circle();
        p1.pads();
        p2.pads();

        DrawText(TextFormat("%i", p1score), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", p2score), SCREEN_WIDTH / 4 * 3 + 20, 20, 80, WHITE);
        DrawText(TextFormat("ROUND %i", cround), SCREEN_WIDTH / 2 - 100, 20, 50, WHITE);


        EndDrawing();
    
        
    }
        }
        PlaySound(Ingamebgm);
        if(quitButton.isPressed(mousePosition, mousePressed)){
            UnloadSound(Hit1Sound);
            UnloadSound(Hit2Sound);    
            UnloadSound(BGM); 
            UnloadSound(Ingamebgm);
            UnloadSound(ugh);
            CloseAudioDevice();
            exit = true;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background, 0, 0, WHITE);
        startButton.Draw();
        quitButton.Draw();
        EndDrawing();
        
        
    }
    CloseWindow();
}