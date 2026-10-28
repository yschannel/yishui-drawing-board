#include"raylib.h"
#include"globals.h"
float vector2_distance(Vector2 start_Pos,Vector2 end_Pos);
void initialize_canvas();
void resize_polygons_array(int new_capacity);
void resize_point_array(Polygon* poly,int new_count);