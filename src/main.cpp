#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "raylib.h"
#include "../incl/generator.hpp"

#define ARR_SIZE 50

Generator<void> BubbleSort(int arr[ARR_SIZE]);
Generator<void> SelectionSort(int arr[ARR_SIZE]);
Generator<void> InsertionSort(int arr[ARR_SIZE]);
void Draw(int arr[ARR_SIZE], bool runningStatus);
void DrawArray(int arr[ARR_SIZE]);
void DrawRunningStatus(bool runningStatus);
void FillArray(int arr[]);

int main() {
  InitWindow(1000, 1000, "Sort Visualizer");

  SetTargetFPS(60);
  
  srand(time(0));
  
  int arr[ARR_SIZE] = {0};
  FillArray(arr);

  Generator<void> sortGen;
  bool sortRunning = false;

  float timeSinceLastStep = 0.0f;
  const float stepDelay = 0.01f;
  
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    timeSinceLastStep += dt;
    
    if (!sortRunning && IsKeyPressed(KEY_B)) {
      sortGen = BubbleSort(arr);
      sortRunning = true;
    }

    if (!sortRunning && IsKeyPressed(KEY_S)) {
      sortGen = SelectionSort(arr);
      sortRunning = true;
    }

    if (!sortRunning && IsKeyPressed(KEY_I)) {
      sortGen = InsertionSort(arr);
      sortRunning = true;
    }

    if (!sortRunning && IsKeyPressed(KEY_LEFT_SHIFT)) {
      FillArray(arr);
    }

    if (sortRunning && IsKeyPressed(KEY_SPACE)) {
      sortRunning = false;
    }

    if (sortRunning && timeSinceLastStep >= stepDelay) {
      sortRunning = sortGen.next();
      timeSinceLastStep = 0.0f;
    }
    
    Draw(arr, sortRunning);
  }
  
  CloseWindow();
  return 0;
}

Generator<void> BubbleSort(int arr[ARR_SIZE]) {
  for (int i = 0; i < ARR_SIZE - 1; i++) {
    for (int j = 0; j < ARR_SIZE - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
      }
      co_yield std::monostate();
    }
  }
  co_return;
}

Generator<void> SelectionSort(int arr[ARR_SIZE]) {
  int i, j, imin;

  for (i = 0; i < ARR_SIZE - 1; i++) {
    imin = i;
    
    for (j = i + 1; j < ARR_SIZE; j++) {
      if (arr[j] < arr[imin]) {
        imin = j;
      }
    }

    std::swap(arr[i], arr[imin]);
    co_yield std::monostate();
  }
  
  co_return;
}

Generator<void> InsertionSort(int arr[ARR_SIZE]) {
  int i, j, key;
  int opCount = 0;
  for (i = 1; i < ARR_SIZE; i++) {
    key = arr[i];
    j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
      opCount++;
    }
    arr[j + 1] = key;
    
    // Yield every few operations to normalize speed
    if (opCount % 3 == 0) {
      co_yield std::monostate();
    }
  }
  co_return;
}

void Draw(int arr[ARR_SIZE], bool runningStatus) {
  BeginDrawing();

    ClearBackground(BLACK);

    DrawArray(arr);
    
    DrawFPS(10, 10);
    DrawRunningStatus(runningStatus);
  
  EndDrawing();
}

void DrawArray(int arr[ARR_SIZE]) {
  float barWidth = 1000.0f / ARR_SIZE;
  float heightScale = 1000.0f / ARR_SIZE;

  for (int i = 0; i < ARR_SIZE; i++) {
    float x = i * barWidth;
    float height = arr[i] * heightScale;
    DrawRectangle(x, 1000 - height, barWidth, height, WHITE);
  }
}

void DrawRunningStatus(bool runningStatus) {
  if (runningStatus) {
    DrawText("Status: Sorting", 10, 30, 15, GREEN);
  } else {
    DrawText("Status: Not Sorting", 10, 30, 15, RED);
  }
}

void FillArray(int arr[ARR_SIZE]) {
  std::srand(std::time(nullptr));

  for (int i = 0; i < ARR_SIZE; i++) {
    int rand_num;
    bool duplicate;

    do {
      rand_num = (std::rand() % ARR_SIZE) + 1; // 1 to ARR_SIZE
      duplicate = false;

      for (int j = 0; j < i; j++) {
        if (arr[j] == rand_num) {
          duplicate = true;
          break;
        }
      }
    } while (duplicate);

    arr[i] = rand_num;
  }
}
