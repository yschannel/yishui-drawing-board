#include "FANCTION.h"
#include "PAGE.h"
#include "globals.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include<string.h>
#include<stdlib.h>

//=== Cross-Platform Directory Scanning Support ===
#ifdef _WIN32
  #include<io.h>
  #include<direct.h>
  #define access _access
#else
  #include <unistd.h>
  #include <direct.h>
#endif

//=== Global File List Storage
#define MAX_SAVED_FILES 100
char saved_filenames[MAX_SAVED_FILES][128] = {0};
int saved_file_count = 0;

#define INITIAL_CAPACITY 64

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

/*Determining turn Direction via Cross Product*/
float cross(const Vector2 *a,const Vector2 *b,const Vector2* c){
  return -((b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x));
}
//determining is point in triangle
bool point_in_triangle(const Vector2* p,const Vector2* a,const Vector2* b ,const Vector2 *c){
  float d1 = cross(p,a,b);
  float d2 = cross(p,b,c);
  float d3 = cross(p, c, a);

  bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
  bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

  return !(has_neg && has_pos);
}
//determining is ear
bool is_ear(int i ,int n, const Vector2* poly, const bool* removed){
  int prev = (i - 1 + n) % n;
  int next = (i + 1) % n;

  while (removed[prev]){
    prev = (prev - 1 + n) % n;}
  while (removed[next]){
    next = (next + 1) % n;}

  const Vector2 *A = &poly[prev];
  const Vector2 *B = &poly[i];
  const Vector2 *C = &poly[next];

  if (cross(A,B,C) <= 0.0f){
    return false;
  }
//check is other points in triangleABC
  for (int k = 0; k < n;k++){
    if(k == prev||k == i|| k==next||removed[k]){
      continue;
    }
    if(point_in_triangle(&poly[k], A, B, C)){
      return false;
    }
  }
  return true;
}
//Ear-clipping triangulation
int* triangulate_polygon(const Vector2* poly,int n,int * out_triangle_count){
  if(n<3){
    *out_triangle_count = 0;
    return NULL;
  }

  bool* removed = (bool*)MemAlloc(n*sizeof(bool));
  if(!removed){
    *out_triangle_count = 0;
    return NULL;
  }
  for (int i = 0; i < n;i++){
    removed[i] = false;
  }

  int max_tris = n - 2;
  int *triangles = (int *)MemAlloc(max_tris * 3 * sizeof(int));
  if(!triangles){
    MemFree(removed);
    *out_triangle_count = 0;
    return NULL;
  }

  int tri_index = 0;
  int remaining = n;

  while(remaining > 3){
    bool found_ear = false;
    for (int i = 0; i < n; i++){
      if(removed[i]){
        continue;
      }
      if(is_ear(i,n,poly,removed)){
        int prev = (i-1+n)%n;
        int next = (i + 1) % n;
        while (removed[prev]){
          prev = (prev - 1 + n) % n;
        }
        while(removed[next]){
          next = (next + 1) % n;
        }

        triangles[tri_index++] = prev;
        triangles[tri_index++] = i;
        triangles[tri_index++] = next;

        removed[i] = true;
        remaining--;
        found_ear = true;
        break;
      }
    }
    if(!found_ear){
      break;
    }
  }

  //add the last triangle
  int last_tri[3];
  int count = 0;
  for (int i = 0; i < n&&count<3;i++){
    if(!removed[i]){
      last_tri[count++] = i;
    }
  }
  if(count==3){
    triangles[tri_index++] = last_tri[0];
    triangles[tri_index++] = last_tri[1];
    triangles[tri_index++] = last_tri[2];
  }

  MemFree(removed);
  *out_triangle_count = tri_index / 3;
  return triangles;
}

/*Functions for all shapes*/
void add_shape(Drawable_shape shape){
  if(total_shapes >= shapes_capacity){
    int new_capacity = (shapes_capacity == 0) ? INITIAL_CAPACITY : shapes_capacity * 2;
    Drawable_shape *new_array = (Drawable_shape *)MemAlloc(sizeof(Drawable_shape) * new_capacity);

    for (int i = 0; i < total_shapes; i++){
      new_array[i] = all_shapes[i];
    }

    if(all_shapes != NULL){
      MemFree(all_shapes);
    }

    all_shapes = new_array;
    shapes_capacity = new_capacity;
  }

  all_shapes[total_shapes] = shape;
  total_shapes++;

  /*
  if(total_shapes >= shapes_capacity){
    shapes_capacity = (shapes_capacity == 0) ? INITIAL_CAPACITY : shapes_capacity * 2;
    Drawable_shape* all_shapes_new = (Drawable_shape *)MemAlloc(sizeof(Drawable_shape) * shapes_capacity) ;
    for (int i = 0; i < total_polygons;i++){
      all_shapes_new[i] = all_shapes[i];
    }
    all_shapes = all_shapes_new;
    MemFree(all_shapes_new);
  }
  all_shapes[total_shapes++] = shape;
*/
}

void clear_all_shapes(){
  for (int i = 0; i < total_shapes;i++){
    switch(all_shapes[i].type){
      case SHAPE_POLYGON:{
        if(all_shapes[i].data.polygon.points){
          MemFree(all_shapes[i].data.polygon.points);
          all_shapes[i].data.polygon.points = NULL;
        }
        break;
      }
      case SHAPE_FREEHAND:{
        if(all_shapes[i].data.freehand.points){
          MemFree(all_shapes[i].data.freehand.points);
          all_shapes[i].data.freehand.points = NULL;
        }
        break;
      }
      default:
        break;
    }
  }
  total_shapes = 0;
}

//color format conversion
Color Int_to_color(int rgb){
  return (Color){
      (unsigned char)((rgb >> 16) & 0xFF),
      (unsigned char)((rgb >> 8) & 0xFF),
      (unsigned char)((rgb >> 0) & 0xFF),
      255};
}
int Color_to_int(Color c){
  return (c.r << 16) | (c.g << 8) | c.b;
}

// === Helper: write a shape to file ===
static bool write_shape_to_file(FILE* file, const Drawable_shape* shape){
  if(fwrite(&shape->type, sizeof(Shape_type),1,file)!=1)
    return false;
  switch (shape->type){
    case SHAPE_POLYGON:{
      const Polygon *p = &shape->data.polygon;
      if(fwrite(&p->side,sizeof(int),1,file)!=1)
        return false;
      if(fwrite(&p->point_count, sizeof(int),1,file) != 1) 
        return false;
      if(fwrite(&p->color,sizeof(int),1,file) != 1)
        return false;
      int is_filled_int = p->is_filled ? 1 : 0;
      if(fwrite(&is_filled_int,sizeof(int),1,file) != 1) 
        return false;
      if(p->point_count > 0){
        if(fwrite(p->points,sizeof(Vector2),p->point_count,file) != (size_t)p->point_count)
          return false;
      }
      break;
    }
    case SHAPE_LINE:{
      if(fwrite(&shape->data.line,sizeof(Line_shape),1,file) != 1)
        return false;
      break;
    }
    case SHAPE_RECTANGLE:{
      if(fwrite(&shape->data.rect, sizeof(Rect_shape),1,file) != 1)
        return false;
      break;
    }
    case SHAPE_CIRCLE:{
      if(fwrite(&shape->data.circle,sizeof(Circle_shape),1,file) != 1)
        return false;
      break;
    }
    case SHAPE_FREEHAND:{
      const Freehand_shape *f = &shape->data.freehand;
      if(fwrite(&f->point_count,sizeof(int),1,file) != 1)
        return false;
      if(fwrite(&f->brush_size,sizeof(float),1,file) != 1)
        return false;
      if(fwrite(&f->color, sizeof(int),1,file) != 1)
        return false;
      if(f->point_count > 0){
        if(fwrite(f->points,sizeof(Vector2),f->point_count,file) != (size_t)f->point_count)
          return false;
      }
      break;
    }
    default:
      return false;
  }
  return true;
}

// === Save Drawing to Specified Path (Binary) ===
bool save_drawing_to_file(const char *filepath){
#ifdef _WIN32
    _mkdir("drawings");
#else
    mkdir("drawings", 0755);
#endif

  FILE* file = fopen(filepath,"wb");
  if(!file) return false;

  //write format signature to file
  if(fwrite(DRAWING_FILE_MAGIC,DRAWING_FILE_MAGIC_LEN,1,file) != 1){
    fclose(file);
    return false;
  }
  //write count of shape
  if (fwrite(&total_shapes,sizeof(int),1,file) != 1){
    fclose(file);
    return false;
  }
  //write all shapes
  for (int i = 0; i < total_shapes;i++){
    if(!write_shape_to_file(file,&all_shapes[i])){
      fclose(file);
      return false;
    }
  }

  fclose(file);
  return true;
}

// === Helper: read a shape from file
static bool read_shape_from_file(FILE* file,Drawable_shape* shape){
   if(fread(&shape->type,sizeof(Shape_type),1,file) != 1) return false;

   switch (shape->type){
    case SHAPE_POLYGON:{
      Polygon* p = &shape->data.polygon;
      if(fread(&p->side,sizeof(int),1,file) != 1) 
        return false;
      if(fread(&p->point_count,sizeof(int),1,file) != 1)
        return false;
      if(fread(&p->color,sizeof(int),1,file) != 1)
        return false;
      int is_filled_int = 0;
      if(fread(&is_filled_int,sizeof(int),1,file) != 1) 
        return false;
      p->is_filled = (is_filled_int != 0);

      p->points = NULL;
      if(p->point_count > 0){
        p->points = (Vector2 *)MemAlloc(sizeof(Vector2) * p->point_count);
        if(!p->points)
          return false;
        if(fread(p->points,sizeof(Vector2),p->point_count,file) != (size_t)p->point_count){
          MemFree(p->points);
          p->points = NULL;
          return false;
        }
      }
      break;
    }
    case SHAPE_LINE:{
      if(fread(&shape->data.line,sizeof(Line_shape),1,file) != 1)
        return false;
      break;
    }
    case SHAPE_RECTANGLE:{
      if(fread(&shape->data.rect,sizeof(Rect_shape),1,file) != 1)
        return false;
      break;
    }
    case SHAPE_CIRCLE:{
      if(fread(&shape->data.circle,sizeof(Circle_shape),1,file) != 1)
        return false;
      break;
    }
    case SHAPE_FREEHAND:{
      Freehand_shape* f = &shape->data.freehand;
      if(fread(&f->point_count,sizeof(int),1,file) != 1)
        return false;
      if(fread(&f->brush_size,sizeof(float),1,file) != 1)
        return false;
      if(fread(&f->color,sizeof(int),1,file) != 1)
        return false;
      
      f->points = NULL;
      if(f->point_count > 0){
        f->points = (Vector2 *)MemAlloc(sizeof(Vector2) * f->point_count);
        if(!f->points) return false;
        if(fread(f->points,sizeof(Vector2),f->point_count,file) != (size_t)f->point_count){
          MemFree(f->points);
          f->points = NULL;
          return false;
        }
      }
      break;
    }
    default:
        return false;
   }
   return true;
}

// === Load Drawing from Specified Path (Binary)
bool load_drawing_from_file(const char *filepath){
  printf("Attempting to load: %s\n", filepath);
  FILE *file = fopen(filepath, "rb");
  if(!file) return false;

  //Validate file signature
  char magic[DRAWING_FILE_MAGIC_LEN];
  if(fread(magic,DRAWING_FILE_MAGIC_LEN,1,file) != 1 ||
     memcmp(magic,DRAWING_FILE_MAGIC,DRAWING_FILE_MAGIC_LEN) != 0){
    fclose(file);
    return false;
  }

  clear_all_shapes();

  int loaded_total_shapes = 0;
  if(fread(&loaded_total_shapes,sizeof(int),1,file)!=1){
    fclose(file);
    return false;
  }

  if(loaded_total_shapes > shapes_capacity){
    MemFree(all_shapes);
    shapes_capacity = loaded_total_shapes;
    all_shapes = (Drawable_shape*)MemAlloc(sizeof(Drawable_shape)*shapes_capacity);
    if(!all_shapes){
      fclose(file);
      return false;
    }
  }

  for (int i = 0; i < loaded_total_shapes;i++){
    Drawable_shape shape ;
    if(!read_shape_from_file(file,&shape)){
      //Clean up loaded memory(flase item before)
      for (int j = 0; j < i;j++){
        switch(all_shapes[j].type){
          case SHAPE_POLYGON:{
            if(all_shapes[j].data.polygon.points){
              MemFree(all_shapes[j].data.polygon.points);
            }
            break;
          }
          case SHAPE_FREEHAND:{
            if(all_shapes[j].data.freehand.points){
              MemFree(all_shapes[j].data.freehand.points);
            }
            break;
          }
          default:{
            break;
          }
        }
      }
      total_shapes = 0;
      fclose(file);
      return false;
    }
    all_shapes[i] = shape;
  }
  total_shapes = loaded_total_shapes;
  fclose(file);

  BeginTextureMode(canvas);
  ClearBackground(WHITE);
  EndTextureMode();
  redraw_canvas_from_shapes();

  printf("Loaded %d shapes from %s\n", total_shapes, filepath);
  fflush(stdout);
  return true;
}

// === Scan for .ydf files in the darwings/ directory
void scan_saved_files(){
  saved_file_count = 0;

  //Ensure the drawings floder exists
#ifdef _WIN32
  _mkdir("drawings");
#else
  mkdir("drawings", 0755);
#endif

#ifdef _WIN32
  struct _finddata_t file_info;
  intptr_t handle = _findfirst("drawings/*.ydf", &file_info);
  if(handle != -1){
    do{
      if(saved_file_count < MAX_SAVED_FILES){
        char *dot = strrchr(file_info.name, '.');
        if(dot)
          *dot = '\0';
        snprintf(saved_filenames[saved_file_count], sizeof(saved_filenames[0]), "%s", file_info.name);
        saved_filenames[saved_file_count][127] = '\0';
        saved_file_count++;
      }
    } while (_findnext(handle, &file_info) == 0);
    _findclose(handle);
  }
#else
  DIR* dir = opendir("drawings");
  if (dir) {
      struct dirent* entry;
      while ((entry = readdir(dir)) != NULL) {
          if (saved_file_count >= MAX_SAVED_FILES) break;
          char* ext = strrchr(entry->d_name, '.');
          if (ext && strcmp(ext, ".ydf") == 0) {
              char basename[128];
              strncpy(basename, entry->d_name, ext - entry->d_name);
              basename[ext - entry->d_name] = '\0';
              strncpy(saved_filenames[saved_file_count], basename, 127);
              saved_filenames[saved_file_count][127] = '\0';
              saved_file_count++;
          }
      }
      closedir(dir);
  }
#endif 
}

// xxx Provide External Access Interface xxx
/*Not yet
int get_saved_file_count(){}
const char* get_saved_filename(int index){
  if(index >= 0 && index < saved_file_count){
    return saved_filenames[index];
  }
  return NULL;
}
*/
