/* Name: Tan Thai                                  *
 * NetID: tanhoangthai                             *
 * Description: this code contains my solution for *
 *              the sixth project of COP3514.002   */

#include <stdio.h>
#include <string.h>

// Core functions
void createArray(FILE *input, int rows, int cols, char map[rows][cols]);
void createCoast(FILE* coast, int rows, int cols, char map[rows][cols]);
void countTouchingWater(int *wet_perimeter, int i, int j, int rows, int cols, char map[rows][cols]);
void countWetPeri(int *wet_perimeter, int rows, int cols, char map[rows][cols]);
void countLand(int *land, int rows, int cols, char map[rows][cols]);

// Helper functions
int isPerimeter(int i, int j, int rows);
int isSurroundedByLand(int i, int j, int rows, int cols, char map[rows][cols]);

// Main
int main(int argc, char **argv) {

  // Get file names
  char *filename = argv[1];

  // Open and create files
  FILE *input = fopen(filename, "r");
  FILE *coast = fopen("coast.txt", "w");
  FILE *statistics = fopen("statistics.txt", "w");

  // Get rows and cols
  int rows, cols;
  char skip;
  fscanf(input, "%d %d", &rows, &cols);
  fscanf(input, "%c", &skip); // Skip '\n'

  // Create array
  char map[rows][cols];
  createArray(input, rows, cols, map);

  /*======================================================================*/

  // Solution 
  // Create coast map and file
  createCoast(coast, rows, cols, map);
  
  // Create statistics
  // Calculate land and wet perimeter 
  int land = 0;
  int wet_perimeter = 0;
  countLand(&land, rows,  cols, map);
  countWetPeri(&wet_perimeter, rows, cols, map);

  // Output statistics to statistics file
  fprintf(statistics ,"TOTAL AREA: %d\n", rows * cols);
  fprintf(statistics ,"TOTAL LAND AREA: %d\n", land);
  fprintf(statistics ,"WET PERIMETER: %d\n", wet_perimeter);
  
  // Close files
  fclose(input);
  fclose(coast);
  fclose(statistics);
  return 0;
}

// Create 2D array of map
void createArray(FILE *input, int rows, int cols, char map[rows][cols]){

  char letter;
  
  // Loop through given text file of map 
  for (int i = 0; i < rows; i++) {
    for (int j = 0 ; j < cols + 1; j++) {
      fscanf(input, "%c", &letter);
      if (letter != '\n') {
        map[i][j] = letter;
      }
    }
  }
}

// Check if on perimeter
int isPerimeter(int i, int j, int rows) {

  return (i-1 < 0 || i+1 >= rows || j-1 < 0 || j+1 >= rows);

}

// Count the number of land tiles that are touching water
void countTouchingWater(int *wet_perimeter, int i, int j, int rows, int cols, char map[rows][cols]){

  // if touching outside water
  if (j-1 < 0){
    *wet_perimeter += 1;
  } 

  if (i-1 < 0){
    *wet_perimeter += 1;
  } 
    
  if (j+1 >= rows){
    *wet_perimeter += 1;
  } 

  if (i+1 >= rows){
    *wet_perimeter += 1;
  } 

  /*============================================================*/

  // if touching water thats not the outside
  if (map[i][j-1] == 'W' && j-1 >= 0){
    *wet_perimeter += 1;
  } 

  if (map[i-1][j] == 'W' && i-1 >= 0){
    *wet_perimeter += 1;
  } 
    
  if (map[i][j+1] == 'W' && j+1 < rows){
    *wet_perimeter += 1;
  } 

  if (map[i+1][j] == 'W' && i+1 < rows){
    *wet_perimeter += 1;
  } 

}

// Count the wet perimeter
void countWetPeri(int *wet_perimeter, int rows, int cols, char map[rows][cols]){

  // Loop through 2D array of given map
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      
      // if land and on perimeter 
      if (map[i][j] == 'L' && isPerimeter(i,j,rows)) {
        countTouchingWater(wet_perimeter, i, j, rows, cols, map);
      }
        
      // if land and not on perimeter 
      else if (map[i][j] == 'L' && !(isPerimeter(i,j,rows))) {
        countTouchingWater(wet_perimeter, i, j, rows, cols, map);
      }
    }
  }
}

// Count the number of land
void countLand(int *land, int rows, int cols, char map[rows][cols]){

  // Loop through 2D array of given map, increment land whenever there is land
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      if (map[i][j] == 'L') *land += 1;
    }
  }
}

// Check if land is surrounded by land
int isSurroundedByLand(int i, int j, int rows, int cols, char map[rows][cols]){

  return (map[i-1][j] == 'L' && map[i+1][j] == 'L' && map[i][j-1] == 'L' && map[i][j+1] == 'L');

}

// Create coast map and file 
void createCoast(FILE* coast, int rows, int cols, char map[rows][cols]){

  char temp[rows][cols];

  // Loop through 2D array of given map
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){

      // If water, then it is not coast
      if (map[i][j] == 'W'){ 
        temp[i][j] = 'N';
        fprintf(coast, "%c", temp[i][j]);
      }

      // If land, then check for possibility of being coast
      else if (map[i][j] == 'L') {
        // if on perimeter and is surrounded by land, then it is a coast
        if (isPerimeter(i, j, rows) && isSurroundedByLand(i, j, rows, cols, map)){
          temp[i][j] = 'Y';
          fprintf(coast, "%c", temp[i][j]);
        }

        // else if it is only surrounded by land, then it is not a coast
        else if (isSurroundedByLand(i, j, rows, cols, map)){
          temp[i][j] = 'N';
          fprintf(coast, "%c", temp[i][j]);
        }
        
        // If all other conditions fails, then it is a coast
        else {
          temp[i][j] = 'Y';
          fprintf(coast, "%c", temp[i][j]);
        }
      }   
    }
    fprintf(coast, "\n");
  }
}

