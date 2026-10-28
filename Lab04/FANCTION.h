#ifndef FANCTION_H
#define FANCTION_H

#include"raylib.h"
#include"globals.h"
float vector2_distance(Vector2 start_Pos,Vector2 end_Pos);
void initialize_canvas();
void resize_polygons_array(int new_capacity);
void resize_point_array(Polygon* poly,int new_count);
float cross(const Vector2 *a, const Vector2 *b, const Vector2 *c);
bool point_in_triangle(const Vector2 *p, const Vector2 *a, const Vector2 *b, const Vector2 *c);
bool is_ear(int i, int n, const Vector2 *poly, const bool *removed);
int *triangulate_polygon(const Vector2 *poly, int n, int *out_triangle_count);
void add_shape(Drawable_shape shape);
void clear_all_shapes();
Color Int_to_color(int rgb);
int Color_to_int(Color c);
bool save_drawing_to_file(const char *filepath);
bool load_drawing_from_file(const char *filepath);
void scan_saved_files();

#endif