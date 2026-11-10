#include <raylib.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720

#define GRID_SIZE 20
#define GRID_COLOR (Color){86, 86, 86, 255}
#define GRID_COUNT_X WINDOW_WIDTH / GRID_SIZE
#define GRID_COUNT_Y WINDOW_HEIGHT / GRID_SIZE

static bool cells[GRID_COUNT_Y][GRID_COUNT_X] = {0};
static bool next[GRID_COUNT_Y][GRID_COUNT_X] = {0};
static bool start = false;

int count_neighbours(int i, int j)
{
  int count = 0;
  
  for (int di=-1; di<=1; ++di)
  {
    for (int dy=-1; dy<=1; ++dy)
    {
      if (di == 0 && dy == 0) continue;

      int r = i + di;
      int c = j + dy;

      if (r >= 0 && r < GRID_COUNT_Y && c >= 0 && c < GRID_COUNT_X)
        count += cells[r][c];
    }
  }

  return count;
}

int main()
{
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Of Life");

  SetTargetFPS(60);

  for (int i=0; i<GRID_COUNT_Y; ++i)
  {
    for (int j=0; j<GRID_COUNT_X; ++j)
    {
      int count = i + j;
      if (count % 2 != 0)
      {
        cells[i][j] = true;
      }
    }
  }

  int frame = 0;
  int simulation_speed = 6;

  while(!WindowShouldClose())
  {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
      Vector2 mouse_pos = GetMousePosition();
      int r = (int)(mouse_pos.y / GRID_SIZE);
      int c = (int)(mouse_pos.x / GRID_SIZE);
      cells[r][c] = true;
    }
    else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
      Vector2 mouse_pos = GetMousePosition();
      int r = (int)(mouse_pos.y / GRID_SIZE);
      int c = (int)(mouse_pos.x / GRID_SIZE);
      cells[r][c] = false;     
    }

    if (IsKeyPressed(KEY_SPACE))
    {
      start = !start;
    }

    if (IsKeyPressed(KEY_Z))
    {
      simulation_speed++;
      if (simulation_speed > 60) simulation_speed = 60;
    }
    else if (IsKeyPressed(KEY_X))
    {
      simulation_speed--;
      if (simulation_speed < 0) simulation_speed = 1;
    }
    else if (IsKeyPressed(KEY_R))
    {
      for (int i=0; i<GRID_COUNT_Y; ++i)
      {
        for (int j=0; j<GRID_COUNT_X; ++j)
        {
          cells[i][j] = false;
          next[i][j] = false;
        }
      }
    }

    if (start && ++frame % simulation_speed == 0) 
    {
      for (int i=0; i<GRID_COUNT_Y; ++i)
      {
        for (int j=0; j<GRID_COUNT_X; ++j)
        {
          int live_neighbours = count_neighbours(i, j);

          if (cells[i][j])
          {
            next[i][j] = live_neighbours == 2 || live_neighbours == 3;
          }
          else
          {
            next[i][j] = live_neighbours == 3;
          }
        }
      }

      for (int i=0; i<GRID_COUNT_Y; ++i)
      {
        for (int j=0; j<GRID_COUNT_X; ++j)
        {
          cells[i][j] = next[i][j];
        }
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    for (int i=0; i<GRID_COUNT_Y; ++i)
    {
      for (int j=0; j<GRID_COUNT_X; ++j)
      {
        if (cells[i][j])  
        {
          DrawRectangle(j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE, WHITE);
        }
      }
    }

    // Draw Grid Lines
    {
      for (int i=0; i<GRID_COUNT_Y; ++i)
      {
        DrawLine(0, i * GRID_SIZE, WINDOW_WIDTH, i * GRID_SIZE, GRID_COLOR);
      }

      for (int i=0; i<GRID_COUNT_X; ++i)
      {
        DrawLine(i * GRID_SIZE, 0, i * GRID_SIZE, WINDOW_HEIGHT, GRID_COLOR);
      }
    }

    DrawText(TextFormat("Simulation Speed: %d", 100 / simulation_speed), 0, 0, 20, RED);

    EndDrawing();
  }

  CloseWindow();
  
  return 0;
}
