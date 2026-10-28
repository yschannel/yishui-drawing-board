#include "FANCTION.h"
#include "PAGE.h"
#include "globals.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

// Define a function that can calculate the distance between two points
float vector2_distance(Vector2 start_Pos, Vector2 end_Pos) {
  float dx = end_Pos.x - start_Pos.x;
  float dy = end_Pos.y - start_Pos.y;
  return sqrtf(pow(dx, 2) + pow(dy, 2));
}

/*Fanction for polygon*/
//(1)initialization
void initialize_canvas() {
  polygons = (Polygon *)MemAlloc(polygon_max_capacity * sizeof(Polygon));
  total_polygons = 0;
}
//(2)memory management
void resize_polygons_array(int new_capacity) {
  Polygon *new_array = (Polygon *)MemAlloc(new_capacity * sizeof(Polygon));

  // initialize new array
  for (int i = 0; i < new_capacity; i++) {
    new_array[i].points = NULL;
    new_array[i].point_count = 0;
  }

  // deeply copy current polygons
  for (int i = 0; i < total_polygons && i < new_capacity; i++) {
    new_array[i].side = polygons[i].side;
    new_array[i].color = polygons[i].color;
    new_array[i].is_filled = polygons[i].is_filled;
    new_array[i].point_count = polygons[i].point_count;

    // assign and copy points
    if (polygons[i].points && polygons[i].point_count > 0) {
      new_array[i].points =
          (Vector2 *)MemAlloc((polygons[i].point_count) * sizeof(Vector2));
      for (int j = 0; j < polygons[i].point_count; j++) {
        new_array[i].points[j] = polygons[i].points[j];
      }
    }
  }

  // free
  for (int i = 0; i < total_polygons; i++) {
    if (polygons[i].points) {
      MemFree(polygons[i].points);
    }
  }

  MemFree(polygons);
  polygons = new_array;
  polygon_max_capacity = new_capacity;
}

void resize_point_array(Polygon *poly, int new_count) {
  Vector2 *new_points = (Vector2 *)MemAlloc(new_count * sizeof(Vector2));

  for (int i = 0; i < poly->side; i++) {
    new_points[i] = poly->points[i];
  }

  MemFree(poly->points);
  poly->points = new_points;
  poly->point_count = new_count;
}
