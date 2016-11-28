/*

  USC/Viterbi/Computer Science
  "Jello Cube" Assignment 1 starter code

  createWorld utility to create your own world files

  Note: this utility uses its own copy of writeWorld routine, which is identical to the one
  found in input.cpp . If you need to change that routine, or even the definition of the
  world structure (you don't have to do this unless you decide to do some fancy
  extra credit), you have to update both copies.

*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct point 
{
   double x;
   double y;
   double z;
};

struct world
{
  char integrator[10]; // "RK4" or "Euler"
  double dt; // timestep, e.g.. 0.001
  int n; // display only every nth timestep
  double kElastic; // Hook's elasticity coefficient for all springs except collision springs
  double dElastic; // Damping coefficient for all springs except collision springs
  double kCollision; // Hook's elasticity coefficient for collision springs
  double dCollision; // Damping coefficient collision springs
  double mass; // mass of each of the 512 control points, mass assumed to be equal for every control point
  int incPlanePresent; // Is the inclined plane present? 1 = YES, 0 = NO
  double a,b,c,d; // inclined plane has equation a * x + b * y + c * z + d = 0; if no inclined plane, these four fields are not used
  int resolution; // resolution for the 3d grid specifying the external force field; value of 0 means that there is no force field
  struct point * forceField; // pointer to the array of values of the force field
  struct point p[8][8][8]; // position of the 512 control points
  struct point v[8][8][8]; // velocities of the 512 control points
};


/* writes the world parameters to a world file on disk*/
/* fileName = string containing the name of the output world file, ex: jello1.w */
/* function creates the output world file and then fills it corresponding to the contents
   of structure 'jello' */
/* function aborts the program if can't access the file */

/* writes the world parameters to a world file on disk*/
/* fileName = string containing the name of the output world file, ex: jello1.w */
/* function creates the output world file and then fills it corresponding to the contents
   of structure 'jello' */
/* function aborts the program if can't access the file */
void writeWorld (char * fileName, struct world * jello)
{
  int i,j,k;
  FILE * file;
  
  file = fopen(fileName, "w");
  if (file == NULL) {
    printf ("can't open file\n");
    exit(1);
  }

  /* write integrator algorithm */ 
  fprintf(file,"%s\n",jello->integrator);

  /* write timestep */
  fprintf(file,"%lf %d\n",jello->dt,jello->n);

  /* write physical parameters */
  fprintf(file, "%lf %lf %lf %lf\n", 
    jello->kElastic, jello->dElastic, jello->kCollision, jello->dCollision);

  /* write mass */
  fprintf(file, "%lf\n", jello->mass);

  /* write info about the plane */
  fprintf(file, "%d\n", jello->incPlanePresent);
  if (jello->incPlanePresent == 1)
    fprintf(file, "%lf %lf %lf %lf\n", jello->a, jello->b, jello->c, jello->d);

  /* write info about the force field */
  fprintf(file, "%d\n", jello->resolution);
  if (jello->resolution != 0)
    for (i=0; i<= jello->resolution-1; i++)
      for (j=0; j<= jello->resolution-1; j++)
        for (k=0; k<= jello->resolution-1; k++)
          fprintf(file, "%lf %lf %lf\n", 
             jello->forceField[i * jello->resolution * jello->resolution + j * jello->resolution + k].x, 
             jello->forceField[i * jello->resolution * jello->resolution + j * jello->resolution + k].y, 
             jello->forceField[i * jello->resolution * jello->resolution + j * jello->resolution + k].z);
  

  /* write initial point positions */
  for (i = 0; i <= 7 ; i++)
  {
    for (j = 0; j <= 7; j++)
    {
      for (k = 0; k <= 7; k++)
        fprintf(file, "%lf %lf %lf\n", 
          jello->p[i][j][k].x, jello->p[i][j][k].y, jello->p[i][j][k].z);
    }
  }
      
  /* write initial point velocities */
  for (i = 0; i <= 7 ; i++)
  {
    for (j = 0; j <= 7; j++)
    {
      for (k = 0; k <= 7; k++)
        fprintf(file, "%lf %lf %lf\n", 
          jello->v[i][j][k].x, jello->v[i][j][k].y, jello->v[i][j][k].z);
    }
  }

  fclose(file);
  
  return;
}

/* modify main to create your own world */
int main()
{
  struct world jello;
  int i,j,k;
  double x,y,z;

  // set the integrator and the physical parameters
  // the values below are EXAMPLES, to be modified by you as needed
  strcpy(jello.integrator,"RK4");
  jello.dt=0.0005000;
  jello.n=1;
  jello.kElastic=200;
  jello.dElastic=0.25;
  jello.kCollision=400.0;
  jello.dCollision=0.25;
  jello.mass= 1.0 / 512;

  // set the inclined plane (not used in this assignment; ignore)
  jello.incPlanePresent=1;
  jello.a=-1;
  jello.b=1;
  jello.c=1;
  jello.d=2;

  // set the external force field
  jello.resolution=30;
  jello.forceField = 
    (struct point *)malloc(jello.resolution*jello.resolution*jello.resolution*sizeof(struct point));
  for (i=0; i<= jello.resolution-1; i++)
    for (j=0; j<= jello.resolution-1; j++)
      for (k=0; k<= jello.resolution-1; k++)
      {
        // set the force at node i,j,k
        // actual space location = x,y,z
        x = -2 + 4*(1.0 * i / (jello.resolution-1));
        y = -2 + 4*(1.0 * j / (jello.resolution-1));
        z = -2 + 4*(1.0 * k / (jello.resolution-1));

        jello.forceField[i * jello.resolution * jello.resolution 
          + j * jello.resolution + k].x = 0; 
        jello.forceField[i * jello.resolution * jello.resolution 
          + j * jello.resolution + k].y = 0;
        jello.forceField[i * jello.resolution * jello.resolution 
          + j * jello.resolution + k].z = 0;
      }

  // set the positions of control points
  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
	    for (k=0; k<=7; k++)
      {
        jello.p[i][j][k].x=1.0 * i / 7;
	  	  jello.p[i][j][k].y=1.0 * j / 7;
		    jello.p[i][j][k].z=1.0 * k / 7;
        if ((i==7) && (j==7) && (k==7))
        {
          jello.p[i][j][k].x=1.0 + 1.0 / 7;
	    	  jello.p[i][j][k].y=1.0 + 1.0 / 7;
		      jello.p[i][j][k].z=1.0 + 1.0 / 7;
        }
 

      }

  // set the velocities of control points
  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
        jello.v[i][j][k].x=10.0;
	  	  jello.v[i][j][k].y=-10.0;
		    jello.v[i][j][k].z=20.0;
      }

  // write the jello variable out to file on disk
  // change jello.w to whatever you need
  writeWorld("jello.w",&jello);

  return 0;
}

