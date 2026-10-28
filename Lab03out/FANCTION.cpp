#include<stdio.h>
#include"raylib.h"
#include<math.h>
#include"globals.h"
#include"PAGE.h"
#include"FANCTION.h"


//Define a function that can calculate the distance between two points
float vector2_distance(Vector2 start_Pos,Vector2 end_Pos){
    float dx = end_Pos.x - start_Pos.x;
    float dy = end_Pos.y - start_Pos.y;
    return sqrtf(pow(dx,2)+pow(dy,2));
}

/*Fanction for polygon*/
//(1)initialization
void initialize_canvas(){
    polygons = (Polygon*)MemAlloc(polyhon_max_capacity * sizeof(Polygon) );
    total_polygons = 0;
}
//(2)memory management
void resize_polygons_array(int new_capacity){
    Polygon* new_array = (Polygon*)MemAlloc(new_capacity * sizeof(Polygon));

    for(int i = 0; i < total_polygons;i++){
        new_array[i] = polygons[i];
    }

    MemFree(polygons);
    polygons = new_array;
    polyhon_max_capacity = new_capacity;
}

void resize_point_array(Polygon* poly,int new_count){
    Vector2* new_points = (Vector2*) MemAlloc(new_count * sizeof(Vector2));

    for(int i = 0;i < poly->side;i++){
        new_points[i] = poly->points[i];
    }

    MemFree(poly->points);
    poly->points = new_points;
    poly->point_count = new_count;
}
