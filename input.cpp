/*
 
 USC/Viterbi/Computer Science
 "Jello Cube" Assignment 1 starter code
 
 */

#include "jello.h"
#include "input.h"

/* Write a screenshot, in the PPM format, to the specified filename, in PPM format */
void saveScreenshot(int windowWidth, int windowHeight, char *filename)
{
    if (filename == NULL)
        return;
    
    // Allocate a picture buffer
    Pic * in = pic_alloc(windowWidth, windowHeight, 3, NULL);
    
    printf("File to save to: %s\n", filename);
    
    for (int i=windowHeight-1; i>=0; i--)
    {
        glReadPixels(0, windowHeight-i-1, windowWidth, 1, GL_RGB, GL_UNSIGNED_BYTE,
                     &in->pix[i*in->nx*in->bpp]);
    }
    
    if (ppm_write(filename, in))
        printf("File saved Successfully\n");
    else
        printf("Error in Saving\n");
    
    pic_free(in);
}

vector<int> getMouseLocation(int x, int y)
{
    vector<int> location;
    location.push_back(x);
    location.push_back(y);
    return location;
}
/* converts mouse drags into information about rotation/translation/scaling */
void mouseMotionDrag(int x, int y)
{
    printf("x:%d, y:%d\n", x, y);
    int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
    
    if (g_iRightMouseButton) // handle camera rotations
    {
        Phi += vMouseDelta[0] * 0.01;
        Theta += vMouseDelta[1] * 0.01;
        
        if (Phi>2*pi)
            Phi -= 2*pi;
        
        if (Phi<0)
            Phi += 2*pi;
        
        if (Theta>pi / 2 - 0.01) // dont let the point enter the north pole
            Theta = pi / 2 - 0.01;
        
        if (Theta<- pi / 2 + 0.01)
            Theta = -pi / 2 + 0.01;
        
        g_vMousePos[0] = x;
        g_vMousePos[1] = y;
    }
}

void mouseMotion (int x, int y)
{
    g_vMousePos[0] = x;
    g_vMousePos[1] = y;
}

void mouseButton(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            g_iLeftMouseButton = (state==GLUT_DOWN);
            break;
        case GLUT_MIDDLE_BUTTON:
            g_iMiddleMouseButton = (state==GLUT_DOWN);
            break;
        case GLUT_RIGHT_BUTTON:
            g_iRightMouseButton = (state==GLUT_DOWN);
            break;
    }
    
    g_vMousePos[0] = x;
    g_vMousePos[1] = y;
}

// gets called whenever a key is pressed
void keyboardFunc (unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
            
        case 'e':
            Theta = pi / 6;
            Phi = pi / 6;
            viewingMode = 0;
            break;
            
        case 'v':
            viewingMode = 1 - viewingMode;
            break;
            
        case 'h':
            shear = 1 - shear;
            break;
            
        case 's':
            structural = 1 - structural;
            break;
            
        case 'b':
            bend = 1 - bend;
            break;
            
        case 'p':
            ppause = 1 - ppause;
            break;
            
        case 'z':
            R -= 0.2;
            if (R < 0.2)
                R = 0.2;
            break;
            
        case 'x':
            R += 0.2;
            break;
            
        case ' ':
            saveScreenToFile = 1 - saveScreenToFile;
            break;
    }
}

/* reads the world parameters from a world file */
/* fileName = string containing the name of the world file, ex: jello1.w */
/* function fills the structure 'jello' with parameters read from file */
/* structure 'jello' will typically be declared (probably statically, not on the heap)
 by the caller function */
/* function aborts the program if can't access the file */
void readWorld (char * fileName, struct world * jello)
{
    int i,j,k;
    FILE * file;
    
    file = fopen(fileName, "r");
    if (file == NULL) {
        printf ("can't open file\n");
        exit(1);
    }
    
    /*
     
     File should first contain a line specifying the integrator (EULER or RK4).
     Example: EULER
     
     Then, follows one line specifying the size of the timestep for the integrator, and
     an integer parameter n specifying  that every nth timestep will actually be drawn
     (the other steps will only be used for internal calculation)
     
     Example: 0.001 5
     Now, timestep equals 0.001. Every fifth time point will actually be drawn,
     i.e. frame1 <--> t = 0
     frame2 <--> t = 0.005
     frame3 <--> t = 0.010
     frame4 <--> t = 0.015
     ...
     
     Then, there should be two lines for physical parameters and external acceleration.
     Format is:
     kElastic dElastic kCollision dCollision
     mass
     Here
     kElastic = elastic coefficient of the spring (same for all springs except collision springs)
     dElastic = damping coefficient of the spring (same for all springs except collision springs)
     kCollision = elastic coefficient of collision springs (same for all collision springs)
     dCollision = damping coefficient of collision springs (same for all collision springs)
     mass = mass in kilograms for each of the 512 mass points
     (mass assumed to be the same for all the points; total mass of the jello cube = 512 * mass)
     
     Example:
     10000 25 10000 15
     0.002
     
     Then, there should be one or two lines for the inclined plane, with the obvious syntax.
     If there is no inclined plane, there should be only one line with a 0 value. There
     is no line for the coefficient. Otherwise, there are two lines, first one containing 1,
     and the second one containing the coefficients.
     Note: there is no inclined plane in this assignment (always 0).
     Example:
     1
     0.31 -0.78 0.5 5.39
     
     Next is the forceField block, first with the resolution and then the data, one point per row.
     Example:
     30
     <here 30 * 30 * 30 = 27 000 lines follow, each containing 3 real numbers>
     
     After this, there should be 1024 lines, each containing three floating-point numbers.
     The first 512 lines correspond to initial point locations.
     The last 512 lines correspond to initial point velocities.
     
     There should no blank lines anywhere in the file.
     
     */
    
    /* read integrator algorithm */
    fscanf(file,"%s\n",&jello->integrator);
    
    /* read timestep size and render */
    fscanf(file,"%lf %d\n",&jello->dt,&jello->n);
    
    /* read physical parameters */
    fscanf(file, "%lf %lf %lf %lf\n",
           &jello->kElastic, &jello->dElastic, &jello->kCollision, &jello->dCollision);
    
    /* read mass of each of the 512 points */
    fscanf(file, "%lf\n", &jello->mass);
    
    /* read info about the plane */
    fscanf(file, "%d\n", &jello->incPlanePresent);
    if (jello->incPlanePresent == 1)
        fscanf(file, "%lf %lf %lf %lf\n", &jello->a, &jello->b, &jello->c, &jello->d);
    
    /* read info about the force field */
    fscanf(file, "%d\n", &jello->resolution);
    jello->forceField =
    (struct point *)malloc(jello->resolution*jello->resolution*jello->resolution*sizeof(struct point));
    if (jello->resolution != 0)
        for (i=0; i<= jello->resolution-1; i++)
            for (j=0; j<= jello->resolution-1; j++)
                for (k=0; k<= jello->resolution-1; k++)
                    fscanf(file, "%lf %lf %lf\n",
                           &jello->forceField[i * jello->resolution * jello->resolution + j * jello->resolution + k].x,
                           &jello->forceField[i * jello->resolution * jello->resolution + j * jello->resolution + k].y,
                           &jello->forceField[i * jello->resolution * jello->resolution + j * jello->resolution + k].z);
    
    
    /* read initial point positions */
    for (i= 0; i <= 7 ; i++)
    {
        for (j = 0; j <= 7; j++)
        {
            for (k = 0; k <= 7; k++)
                fscanf(file, "%lf %lf %lf\n",
                       &jello->p[i][j][k].x, &jello->p[i][j][k].y, &jello->p[i][j][k].z);
        }
    }
    
    /* read initial point velocities */
    for (i = 0; i <= 7 ; i++)
    {
        for (j = 0; j <= 7; j++)
        {
            for (k = 0; k <= 7; k++)
                fscanf(file, "%lf %lf %lf\n",
                       &jello->v[i][j][k].x, &jello->v[i][j][k].y, &jello->v[i][j][k].z);
        }
    }
    fclose(file);
    
    return;
}

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
    fprintf(file, "%lf\n",
            jello->mass);
    
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
