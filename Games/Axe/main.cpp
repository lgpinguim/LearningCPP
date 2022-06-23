#include "raylib.h"

int main()
{
  // Window Dimensions
  int windowWidth = 800;
  int windowHeigth = 450;
  InitWindow(windowWidth, windowHeigth, "Axe Game");

  // Circle coordinates
  int circle_x{200};
  int circle_y{200};
  int circle_radius{25};
  // circle edges
  int l_circle_x{circle_x - circle_radius};
  int r_circle_x{circle_x + circle_radius};
  int u_circle_y{circle_y - circle_radius};
  int b_circle_y{circle_y + circle_radius};

  // Axe coordinates
  int axe_x{400};
  int axe_y{0};
  int axe_lenght{50};
  // axe edges
  int l_axe_x{axe_x};
  int r_axe_x{axe_x + axe_lenght};
  int u_axe_y{axe_y};
  int b_axe_y{axe_y + axe_lenght};

  bool collision_with_axe = 
                            (b_axe_y >= u_circle_y) &&
                            (u_axe_y <= b_circle_y) &&
                            (l_axe_x <= r_circle_x) &&
                            (r_axe_x >= l_circle_x);

  int direction{10};

  SetTargetFPS(60);
  while (WindowShouldClose() == false)
  {
    BeginDrawing();
    ClearBackground(WHITE);

    if (collision_with_axe)
    {
      DrawText("Game Over!", 400, 200, 20, RED);
    }

    else
    { // Game Logic Begins

      //Update edges
      l_circle_x = circle_x - circle_radius;
      r_circle_x = circle_x + circle_radius;
      u_circle_y = circle_y - circle_radius;
      b_circle_y = circle_y + circle_radius;

      l_axe_x = axe_x;
      r_axe_x = axe_x + axe_lenght;
      u_axe_y = axe_y;
      b_axe_y = axe_y + axe_lenght;

      // update collision
      collision_with_axe = 
                            (b_axe_y >= u_circle_y) &&
                            (u_axe_y <= b_circle_y) &&
                            (l_axe_x <= r_circle_x) &&
                            (r_axe_x >= l_circle_x);

      DrawCircle(circle_x, circle_y, circle_radius, BLUE);
      DrawRectangle(axe_x, axe_y, axe_lenght, axe_lenght, RED);

      axe_y += direction;
      if (axe_y > windowHeigth || axe_y < 0)
      {
        direction = -direction;
      }

      if (IsKeyDown(KEY_D) && circle_x < windowWidth)
      {
        circle_x += 10;
      }
      if (IsKeyDown(KEY_A) && circle_x > 0)
      {
        circle_x -= 10;
      }
    }
    EndDrawing();
  }
}