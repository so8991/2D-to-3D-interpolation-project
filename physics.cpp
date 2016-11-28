
/*
 
 USC/Viterbi/Computer Science
 "Jello Cube" Assignment 1 starter code
 
 */

#include "jello.h"
#include "physics.h"
#include <vector>
#include <iostream>
using namespace std;
static int isCollide=0;

void getStructForce(struct world *jello, int i, int j, int k, struct point * strForce)
{
    vector<struct point> neighbors;
    struct point currentIdx;
    if(i!=0)
    {
        currentIdx.x = i-1;
        currentIdx.y = j;
        currentIdx.z = k;
        neighbors.push_back(currentIdx);
    }
    if(i!=7)
    {
        currentIdx.x = i+1;
        currentIdx.y = j;
        currentIdx.z = k;
        neighbors.push_back(currentIdx);
    }
    if(j!=0){
        currentIdx.x = i;
        currentIdx.y = j-1;
        currentIdx.z = k;
        neighbors.push_back(currentIdx);
    }
    if(j!=7)
    {
        currentIdx.x = i;
        currentIdx.y = j+1;
        currentIdx.z = k;
        neighbors.push_back(currentIdx);
    }
    if(k!=0){
        currentIdx.x = i;
        currentIdx.y = j;
        currentIdx.z = k-1;
        neighbors.push_back(currentIdx);
    }
    if(k!=7)
    {
        currentIdx.x = i;
        currentIdx.y = j;
        currentIdx.z = k+1;
        neighbors.push_back(currentIdx);
    }
    // find forces through neighbors
    vector<struct point>::iterator iter;
    struct point oneStrForce;
    strForce->x = 0;
    strForce->y = 0;
    strForce->z = 0;
    oneStrForce.x = 0;
    oneStrForce.y = 0;
    oneStrForce.z = 0;
    for(iter=neighbors.begin(); iter!=neighbors.end(); iter++)
    {
        int ii, jj, kk; //neighbor (ii, jj, kk)
        ii = iter->x;
        jj = iter->y;
        kk = iter->z;
        
        point deltaL;
        pDIFFERENCE(jello->p[i][j][k], jello->p[ii][jj][kk], deltaL);
        point deltaV;
        pDIFFERENCE(jello->v[i][j][k], jello->v[ii][jj][kk], deltaV);
        double absL; // L * L
        absL = sqrt(deltaL.x * deltaL.x + deltaL.y * deltaL.y + deltaL.z * deltaL.z);
        
        //(va - vb) * L
        double difVL;
        difVL = deltaV.x * deltaL.x + deltaV.y * deltaL.y + deltaV.z * deltaL.z;
        //pDOTPRODUCT(deltaV, deltaL, difVL);
        double length;
        pNORMALIZE(deltaL);
        
        double restLen = (double)1/8.0;
        
        //Hook's LAW
        double dx = absL - restLen;
        double magForce = jello->kElastic * dx * (-1);
        
        point forceVect;
        pMULTIPLY(deltaL, magForce, forceVect);
        oneStrForce.x = strForce->x + oneStrForce.x;
        oneStrForce.y = strForce->y + oneStrForce.y;
        oneStrForce.z = strForce->z + oneStrForce.z;
        double dampForce = (jello->dElastic*difVL*(-1))/absL;
        point dampVForce;
        pMULTIPLY(deltaL, dampForce, dampVForce);
        pSUM(oneStrForce, dampVForce, oneStrForce);
        
    }
    strForce->x = oneStrForce.x;
    strForce->y = oneStrForce.y;
    strForce->z = oneStrForce.z;
}
void getSheerForce(struct world *jello, int i, int j, int k, struct point * shrForce)
{
    vector<struct point> neighbors;
    struct point currentIdx;
    if(j>=1)
    {
        if(k>=1)
        {
            currentIdx.x = i;
            currentIdx.y = j-1;
            currentIdx.z = k-1;
            neighbors.push_back(currentIdx);
        }
        if(k<=6)
        {
            currentIdx.x = i;
            currentIdx.y = j-1;
            currentIdx.z = k+1;
            neighbors.push_back(currentIdx);
        }
    }
    if(j<=6)
    {
        if(k>=1)
        {
            currentIdx.x = i;
            currentIdx.y = j+1;
            currentIdx.z = k-1;
            neighbors.push_back(currentIdx);
        }
        if(k<=6)
        {
            currentIdx.x = i;
            currentIdx.y = j+1;
            currentIdx.z = k+1;
            neighbors.push_back(currentIdx);
        }
    }
    if(i>=1)
    {
        if(k>=1)
        {
            currentIdx.x = i-1;
            currentIdx.y = j;
            currentIdx.z = k-1;
            neighbors.push_back(currentIdx);
        }
        if(k<=6)
        {
            currentIdx.x = i-1;
            currentIdx.y = j;
            currentIdx.z = k+1;
            neighbors.push_back(currentIdx);
        }
        if(j>=1)
        {
            currentIdx.x = i-1;
            currentIdx.y = j-1;
            currentIdx.z = k;
            neighbors.push_back(currentIdx);
            if(k>=1)
            {
                currentIdx.x = i-1;
                currentIdx.y = j-1;
                currentIdx.z = k-1;
                neighbors.push_back(currentIdx);
            }
            if(k<=6)
            {
                currentIdx.x = i-1;
                currentIdx.y = j-1;
                currentIdx.z = k+1;
                neighbors.push_back(currentIdx);
            }
        }
        if(j<=6)
        {
            currentIdx.x = i-1;
            currentIdx.y = j+1;
            currentIdx.z = k;
            neighbors.push_back(currentIdx);
            if(k>=1)
            {
                currentIdx.x = i-1;
                currentIdx.y = j+1;
                currentIdx.z = k-1;
                neighbors.push_back(currentIdx);
            }
            if(k<=6)
            {
                currentIdx.x = i-1;
                currentIdx.y = j+1;
                currentIdx.z = k+1;
                neighbors.push_back(currentIdx);
            }
        }
    }
    if(i<=6)
    {
        if(k>=1)
        {
            currentIdx.x = i+1;
            currentIdx.y = j;
            currentIdx.z = k-1;
            neighbors.push_back(currentIdx);
        }
        if(k<=6)
        {
            currentIdx.x = i+1;
            currentIdx.y = j;
            currentIdx.z = k+1;
            neighbors.push_back(currentIdx);
        }
        if(j>=1)
        {
            currentIdx.x = i+1;
            currentIdx.y = j-1;
            currentIdx.z = k;
            neighbors.push_back(currentIdx);
            if(k>=1)
            {
                currentIdx.x = i+1;
                currentIdx.y = j-1;
                currentIdx.z = k-1;
                neighbors.push_back(currentIdx);
            }
            if(k<=6)
            {
                currentIdx.x = i+1;
                currentIdx.y = j-1;
                currentIdx.z = k+1;
                neighbors.push_back(currentIdx);
            }
        }
        if(j<=6)
        {
            currentIdx.x = i+1;
            currentIdx.y = j+1;
            currentIdx.z = k;
            neighbors.push_back(currentIdx);
            if(k>=1)
            {
                currentIdx.x = i+1;
                currentIdx.y = j+1;
                currentIdx.z = k-1;
                neighbors.push_back(currentIdx);
            }
            
            if(k<=6)
            {
                currentIdx.x = i+1;
                currentIdx.y = j+1;
                currentIdx.z = k+1;
                neighbors.push_back(currentIdx);
            }
        }
    }
    
    // find forces through neighbors
    vector<struct point>::iterator iter;
    struct point oneShrForce;
    shrForce->x = 0;
    shrForce->y = 0;
    shrForce->z = 0;
    oneShrForce.x = 0;
    oneShrForce.y = 0;
    oneShrForce.z = 0;
    for(iter=neighbors.begin(); iter!=neighbors.end(); iter++)
    {
        int ii, jj, kk; //neighbor (ii, jj, kk)
        ii = iter->x;
        jj = iter->y;
        kk = iter->z;
        
        point deltaL;
        pDIFFERENCE(jello->p[i][j][k], jello->p[ii][jj][kk], deltaL);
        point deltaV;
        pDIFFERENCE(jello->v[i][j][k], jello->v[ii][jj][kk], deltaV);
        double absL; // L * L
        absL = sqrt(deltaL.x * deltaL.x + deltaL.y * deltaL.y + deltaL.z * deltaL.z);
        
        //double absL = sqrt((deltaL.x * deltaL.x + deltaL.y * deltaL.y + deltaL.z * deltaL.z));
        //(va - vb) * L
        double difVL;
        difVL = deltaV.x * deltaL.x + deltaV.y * deltaL.y + deltaV.z * deltaL.z;
        //pDOTPRODUCT(deltaV, deltaL, difVL);
        double length;
        pNORMALIZE(deltaL);
        
        //double restLen = (double)1/8.0;
        double restLen1, restLen2;
        restLen1 = sqrt(2)*0.142857;
        restLen2 = sqrt(3)*0.142857;
        
        //Hook's LAW
        double dx;
        if((abs(i-ii)==1 && (abs(j-jj)==1)&& (abs(k-kk)==1))) {
            dx = absL - restLen2;
        }else {
            dx = absL - restLen1;
        }
        double magShrForce = jello->kElastic*dx * (-1);
        
        point shearVForce;
        pMULTIPLY(deltaL, magShrForce, shearVForce);
        pSUM(shearVForce, oneShrForce, oneShrForce);
        double dampForce = (jello->dElastic*difVL*(-1))/absL;
        point dampVForce;
        pMULTIPLY(deltaL, dampForce, dampVForce);
        pSUM(oneShrForce, dampVForce, oneShrForce);
        
    }
    shrForce->x = oneShrForce.x;
    shrForce->y = oneShrForce.y;
    shrForce->z = oneShrForce.z;
}
void getBendForce(struct world *jello, int i, int j, int k, struct point * bndForce)
{
    vector<struct point> neighbors;
    struct point currentIdx;
    
    if(i>=2)
    {
        currentIdx.x = i-2;
        currentIdx.y = j;
        currentIdx.z = k;
        neighbors.push_back(currentIdx);
    }
    if(i<=5)
    {
        currentIdx.x = i+2;
        currentIdx.y = j;
        currentIdx.z = k;
        neighbors.push_back(currentIdx);
    }
    if(j>=2)
    {
        currentIdx.x = i;
        currentIdx.y = j-2;
        currentIdx.z = k;
        neighbors.push_back(currentIdx);
    }
    if(j<=5)
    {
        currentIdx.x = i;
        currentIdx.y = j+2;
        currentIdx.z = k;
    }
    if(k>=2)
    {
        currentIdx.x = i;
        currentIdx.y = j;
        currentIdx.z = k-2;
        neighbors.push_back(currentIdx);
    }
    if(k<=5)
    {
        currentIdx.x = i;
        currentIdx.y = j;
        currentIdx.z = k+2;
        neighbors.push_back(currentIdx);
    }
    //cout<<&neighbors[0]<<endl;
    vector<struct point>::iterator iter;
    bndForce->x = 0;
    bndForce->y = 0;
    bndForce->z = 0;
    //bend force for one node
    point oneBndForce;
    oneBndForce.x = 0;
    oneBndForce.y = 0;
    oneBndForce.z = 0;
    //iterate until the size of neighbors
    for (iter=neighbors.begin(); iter!=neighbors.end(); iter++)
    {
        int ii,jj,kk;
        ii = iter->x;
        jj = iter->y;
        kk = iter->z;
        
        //L
        point deltaL;
        //pDIFFERENCE(jello->p[i][j][k],jello->p[ii][jj][kk],deltaL);
        deltaL.x = jello->p[i][j][k].x - jello->p[ii][jj][kk].x;
        deltaL.y = jello->p[i][j][k].y - jello->p[ii][jj][kk].y;
        deltaL.z = jello->p[i][j][k].z - jello->p[ii][jj][kk].z;
        //delta v
        point deltaV;
        //pDIFFERENCE(jello->v[i][j][k],jello->v[ii][jj][kk],deltaV);
        deltaV.x = jello->v[i][j][k].x - jello->v[kk][jj][kk].x;
        deltaV.y = jello->v[i][j][k].y - jello->v[kk][jj][kk].y;
        deltaV.z = jello->v[i][j][k].z - jello->v[kk][jj][kk].z;
        double absL, diffVL, length, restBend;
        // magnitute of L
        absL = sqrt(deltaL.x * deltaL.x + deltaL.y * deltaL.y + deltaL.z * deltaL.z);
        // V dot L
        diffVL = deltaV.x * deltaL.x + deltaV.y * deltaL.y + deltaV.z * deltaL.z;
        
        //pNORMALIZE(deltaL);
        restBend = 0.142857*2;
        
        //Hook Law!!
        double dx;
        dx = absL - restBend;
        //Magnitude of Bend Force!!
        double magBendForce;
        magBendForce = jello->kElastic*dx*(-1);
        //bend force vector
        point bendVForce;
        //pMULTIPLY(deltaL, magBendForce, bendVForce);
        bendVForce.x = deltaL.x * magBendForce;
        bendVForce.y = deltaL.y * magBendForce;
        bendVForce.z = deltaL.z * magBendForce;
        //sum of single forces
        //pSUM(oneBndForce,bendVForce,oneBndForce);
        oneBndForce.x += bendVForce.x;
        oneBndForce.y += bendVForce.y;
        oneBndForce.z += bendVForce.z;
        //damping force
        double dampForce= (jello->dElastic*(-1) *diffVL) / absL;
        point dampVForce;
        dampVForce.x = deltaL.x * dampForce;
        dampVForce.y = deltaL.y * dampForce;
        dampVForce.z = deltaL.z * dampForce;
        
        oneBndForce.x += dampVForce.x;
        oneBndForce.y += dampVForce.y;
        oneBndForce.z += dampVForce.z;
        //pMULTIPLY(deltaL, dampForce, dampVForce);
        //pSUM(oneBndForce, dampVForce, oneBndForce);
        
    }
    bndForce->x = oneBndForce.x;
    bndForce->y = oneBndForce.y;
    bndForce->z = oneBndForce.z;
    
}

void getExtForce(struct world *jello, int i, int j, int k, struct point * extForce)
{
    point f_vect[2][2][2];
    double x, y, z, x0, y0, z0, x1, y1, z1;
    //points
    x = jello->p[i][j][k].x;
    y = jello->p[i][j][k].y;
    z = jello->p[i][j][k].z;
    int ii, jj, kk;
    for (ii=0; ii< jello->resolution; ii++)
    {
        x0 = 4 * (1.0 * ii/(jello->resolution-1 ))-2;
        x1 = 4 * (1.0 * (ii+1)/(jello->resolution-1))-2;
        if ((x>=x0)&&(x<=x1)){
            break;
        }
    }
    for (jj=0; jj< jello->resolution; jj++)
    {
        y0 = 4*(1.0 * jj/(jello->resolution-1))-2;
        y1 = 4*(1.0 *(jj+1)/(jello->resolution-1))-2;
        
        if ((y>=y0)&&(y<=y1)){
            break;
        }
    }
    for (kk=0; kk<jello->resolution; kk++)
    {
        z0 = 4 * (1.0 * kk/(jello->resolution-1))-2;
        z1 = 4 * (1.0 * (kk+1)/(jello->resolution-1))-2;
        if((z>=z0)&&(z<=z1)){
            break;
        }
    }
    
    if((ii<jello->resolution-1)&&(jj<jello->resolution-1)&&(kk<jello->resolution-1)) // point inside force field
    {
        
        int a = ii * jello->resolution * jello->resolution;
        int b = jj * jello->resolution;
        int c = (jj+1) * jello->resolution;
        int d = (ii+1) * jello->resolution * jello->resolution;
        
        f_vect[0][0][0].x = jello->forceField[a + b + k].x;
        f_vect[0][0][0].y = jello->forceField[a + b + k].y;
        f_vect[0][0][0].z = jello->forceField[a + b + k].z;
        
        f_vect[1][0][0].x = jello->forceField[d + b + k].x;
        f_vect[1][0][0].y = jello->forceField[d + b + k].y;
        f_vect[1][0][0].z = jello->forceField[d + b + k].z;
        
        f_vect[0][1][0].x = jello->forceField[a + c + k].x;
        f_vect[0][1][0].y = jello->forceField[a + c + k].y;
        f_vect[0][1][0].z = jello->forceField[a + c + k].z;
        
        f_vect[0][0][1].x = jello->forceField[a + b + (k+1)].x;
        f_vect[0][0][1].y = jello->forceField[a + b + (k+1)].y;
        f_vect[0][0][1].z = jello->forceField[a + b + (k+1)].z;
        
        f_vect[1][1][0].x = jello->forceField[d + c + k].x;
        f_vect[1][1][0].y = jello->forceField[d + c + k].y;
        f_vect[1][1][0].z = jello->forceField[d + c + k].z;
        
        f_vect[1][0][1].x = jello->forceField[d + b + (k+1)].x;
        f_vect[1][0][1].y = jello->forceField[d + b + (k+1)].y;
        f_vect[1][0][1].z = jello->forceField[d + b + (k+1)].z;
        
        f_vect[0][1][1].x = jello->forceField[a + c + (k+1)].x;
        f_vect[0][1][1].y = jello->forceField[a + c + (k+1)].y;
        f_vect[0][1][1].z = jello->forceField[a + c + (k+1)].z;
        
        f_vect[1][1][1].x = jello->forceField[d + c + (k+1)].x;
        f_vect[1][1][1].y = jello->forceField[d + c + (k+1)].y;
        f_vect[1][1][1].z = jello->forceField[d + c + (k+1)].z;
        
        //interpolation
        double alpha = x-x0;
        double beta = y-y0;
        double gamma = z-z0;
        double GL = x1-x0;
        double alphaGL = (GL-alpha)/GL ;
        double betaGL = (GL-beta)/GL ;
        double gammaGL = (GL-gamma)/GL ;
        
        double aGL = alpha/GL;
        double bGL = beta/GL;
        double gGL = gamma/GL;
        
        point forceVect;
        forceVect.x = f_vect[0][0][0].x * alphaGL * betaGL * gammaGL + f_vect[1][0][0].x * aGL   * betaGL * gammaGL + f_vect[0][1][0].x * alphaGL * bGL   * gammaGL + f_vect[0][0][1].x * alphaGL * betaGL * gGL +f_vect[1][0][1].x * aGL   * betaGL * gGL + f_vect[0][1][1].x * alphaGL * bGL   * gGL   + f_vect[1][1][0].x * aGL   * bGL   * gammaGL + f_vect[1][1][1].x * aGL*bGL* gGL;
        
        forceVect.y = f_vect[0][0][0].y * alphaGL * betaGL * gammaGL + f_vect[1][0][0].y * aGL   * betaGL * gammaGL + f_vect[0][1][0].y * alphaGL * bGL   * gammaGL + f_vect[0][0][1].y * alphaGL * betaGL * gGL   + f_vect[1][0][1].y * aGL   * betaGL * gGL   + f_vect[0][1][1].y * alphaGL * bGL   * gGL   + f_vect[1][1][0].y * aGL   * bGL   * gammaGL + f_vect[1][1][1].y * aGL   * bGL   * gGL   ;
        
        forceVect.z = f_vect[0][0][0].z * alphaGL * betaGL * gammaGL + f_vect[1][0][0].z * aGL   * betaGL * gammaGL + f_vect[0][1][0].z * alphaGL * bGL   * gammaGL + f_vect[0][0][1].z * alphaGL * betaGL * gGL   + f_vect[1][0][1].z * aGL   * betaGL * gGL   + f_vect[0][1][1].z * alphaGL * bGL   * gGL   + f_vect[1][1][0].z * aGL   * bGL   * gammaGL + f_vect[1][1][1].z * aGL   * bGL   * gGL   ;
        
        extForce->x  = forceVect.x;
        extForce->y  = forceVect.y;
        extForce->z  = forceVect.z;
    }
}
/* Computes acceleration to every control point of the jello cube,
 which is in state given by 'jello'.
 Returns result in array 'a'. */
void computeAcceleration(struct world * jello, struct point a[8][8][8])
{
    /* for you to implement ... */
    int i,j,k;
    
    for (i=0; i<8; i++)
        for (j=0; j<8; j++)
            for (k=0; k<8; k++)
            {
                
                point strForce;
                point shrForce;
                point bndForce;
                //compute force for struct, sheer, bend spring
                getStructForce(jello, i, j, k, &strForce);
                getSheerForce(jello, i, j, k, &shrForce);
                getBendForce(jello, i, j, j, &bndForce);
                
                //compute external force
                point extForce;
                getExtForce(jello, i, j, k, &extForce);
                //Total of all the forces on a mass point
                point totalForce;
                pSUM(strForce, shrForce, totalForce);
                pSUM(totalForce, bndForce, totalForce);
                pSUM(totalForce, extForce, totalForce);
                //shrForce + shrForce + bndForce + extForce;
                //check the collision with each boundary
                //All points are in
                /*for (i = 0; i < 8; i++)
                 for (j = 0; j < 8; j++)
                 for (k = 0; k < 8; k++)
                 {
                 if((jello->p[i][j][k].x>-2 && jello->p[i][j][k].x<2) &&
                 (jello->p[i][j][k].y>-2 && jello->p[i][j][k].y<2) &&
                 (jello->p[i][j][k].z>-2 && jello->p[i][j][k].z<2)  )
                 col=1;
                 else
                 col=0;
                 }
                 if(col==1){
                 col=0;
                 //jello-> = jello->p[i][j][k].x;
                 
                 }*/// check collission
                
                if(jello->isinside[i][j][k]==1)
                {
                    //printf("inside!");
                    isCollide = 0;
                    if  ((jello->p[i][j][k].x<=-2)||(jello->p[i][j][k].x>=2)||(jello->p[i][j][k].y<=-2)||(jello->p[i][j][k].y>=2) ||(jello->p[i][j][k].z<=-2)||(jello->p[i][j][k].z>=2)  )
                        
                        //if((jello->p[i][j][k].x==0)||jello->p[i][j][k].y==0||jello->p[i][j][k].z==0)
                    {
                        jello->isinside[i][j][k] = 0;
                        isCollide++;
                        
                        //first collision for collision point
                        if (isCollide==1)
                        {
                            jello->collPnt[i][j][k].x =jello->p[i][j][k].x;
                            jello->collPnt[i][j][k].y =jello->p[i][j][k].y;
                            jello->collPnt[i][j][k].z =jello->p[i][j][k].z;
                        }
                    }
                }
                else{
                    if((jello->p[i][j][k].x>=-2) && (jello->p[i][j][k].x<=2)&&(jello->p[i][j][k].y>=-2) && (jello->p[i][j][k].y<=2)&&(jello->p[i][j][k].z>=-2) && (jello->p[i][j][k].z<=2) )
                    {
                        jello->isinside[i][j][k] = 1;
                        bool side;
                        point normal;
                        
                        
                    }
                    else
                    {
                        point L;
                        pDIFFERENCE(jello->p[i][j][k],jello->collPnt[i][j][k],L );
                        
                        point diffV, collVPnt;
                        pDIFFERENCE(jello->v[i][j][k],collVPnt,diffV );
                        double absL;
                        absL = sqrt((L).x * (L).x + (L).y * (L).y + (L).z * (L).z);
                        
                        //normal vector
                        struct point Nfloor;
                        if(jello->p[i][j][k].x<=-2)
                        {
                            Nfloor.x = 1;
                            Nfloor.y = 0;
                            Nfloor.z = 0;
                        }else if(jello->p[i][j][k].x>=2)
                        {
                            Nfloor.x = -1;
                            Nfloor.y = 0;
                            Nfloor.z = 0;
                            
                        }else if(jello->p[i][j][k].y<=-2)
                        {
                            Nfloor.x = 0;
                            Nfloor.y = 1;
                            Nfloor.z = 0;
                            
                        }else if(jello->p[i][j][k].y>=2)
                        {
                            Nfloor.x = 0;
                            Nfloor.y = -1;
                            Nfloor.z = 0;
                            
                        }else if(jello->p[i][j][k].z<=-2)
                        {
                            Nfloor.x = 0;
                            Nfloor.y = 0;
                            Nfloor.z = 1;
                            
                        }else if(jello->p[i][j][k].z>=2)
                        {
                            Nfloor.x = 0;
                            Nfloor.y = 0;
                            Nfloor.z = -1;
                            
                        }
                        
                        double diffVL;
                        diffVL = diffV.x * L.x + diffV.y * L.y + diffV.z * L.z;
                        double FcolMag;
                        FcolMag = jello->kCollision * absL;
                        point FcolV;
                        pMULTIPLY(Nfloor, FcolMag, FcolV);
                        double FcolDamp;
                        FcolDamp = ( jello->dCollision * diffVL) / absL;
                        point FcolDampV;
                        pMULTIPLY(Nfloor, FcolDamp , FcolDampV);
                        pSUM(FcolV , FcolDampV, FcolV    );
                        pSUM(totalForce, FcolV, totalForce);
                    }
                    
                }
                
                
                point accel;
                pMULTIPLY( totalForce, (1/jello->mass), accel);
                a[i][j][k] = accel;
                
                if((totalForce.x>70)||(totalForce.x<-70))
                    totalForce.x/=2;
                
                if((totalForce.y>70)||(totalForce.y<-70))
                    totalForce.y/=2;
                
                if((totalForce.z>70)||(totalForce.z<-70))
                    totalForce.z/=2;
                
                
                a[i][j][k] = accel;
            }
}

/* performs one step of Euler Integration */
/* as a result, updates the jello structure */
void Euler(struct world * jello)
{
    int i,j,k;
    point a[8][8][8];
    
    computeAcceleration(jello, a);
    
    for (i=0; i<=7; i++)
        for (j=0; j<=7; j++)
            for (k=0; k<=7; k++)
            {
                jello->p[i][j][k].x += jello->dt * jello->v[i][j][k].x;
                jello->p[i][j][k].y += jello->dt * jello->v[i][j][k].y;
                jello->p[i][j][k].z += jello->dt * jello->v[i][j][k].z;
                jello->v[i][j][k].x += jello->dt * a[i][j][k].x;
                jello->v[i][j][k].y += jello->dt * a[i][j][k].y;
                jello->v[i][j][k].z += jello->dt * a[i][j][k].z;
                
            }
}

/* performs one step of RK4 Integration */
/* as a result, updates the jello structure */
void RK4(struct world * jello)
{
    point F1p[8][8][8], F1v[8][8][8],
    F2p[8][8][8], F2v[8][8][8],
    F3p[8][8][8], F3v[8][8][8],
    F4p[8][8][8], F4v[8][8][8];
    
    point a[8][8][8];
    
    
    struct world buffer;
    
    int i,j,k;
    
    buffer = *jello; // make a copy of jello
    
    computeAcceleration(jello, a);
    
    for (i=0; i<=7; i++)
        for (j=0; j<=7; j++)
            for (k=0; k<=7; k++)
            {
                pMULTIPLY(jello->v[i][j][k],jello->dt,F1p[i][j][k]);
                pMULTIPLY(a[i][j][k],jello->dt,F1v[i][j][k]);
                pMULTIPLY(F1p[i][j][k],0.5,buffer.p[i][j][k]);
                pMULTIPLY(F1v[i][j][k],0.5,buffer.v[i][j][k]);
                pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
                pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
            }
    
    computeAcceleration(&buffer, a);
    
    for (i=0; i<=7; i++)
        for (j=0; j<=7; j++)
            for (k=0; k<=7; k++)
            {
                // F2p = dt * buffer.v;
                pMULTIPLY(buffer.v[i][j][k],jello->dt,F2p[i][j][k]);
                // F2v = dt * a(buffer.p,buffer.v);
                pMULTIPLY(a[i][j][k],jello->dt,F2v[i][j][k]);
                pMULTIPLY(F2p[i][j][k],0.5,buffer.p[i][j][k]);
                pMULTIPLY(F2v[i][j][k],0.5,buffer.v[i][j][k]);
                pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
                pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
            }
    
    computeAcceleration(&buffer, a);
    
    for (i=0; i<=7; i++)
        for (j=0; j<=7; j++)
            for (k=0; k<=7; k++)
            {
                // F3p = dt * buffer.v;
                pMULTIPLY(buffer.v[i][j][k],jello->dt,F3p[i][j][k]);
                // F3v = dt * a(buffer.p,buffer.v);
                pMULTIPLY(a[i][j][k],jello->dt,F3v[i][j][k]);
                pMULTIPLY(F3p[i][j][k],0.5,buffer.p[i][j][k]);
                pMULTIPLY(F3v[i][j][k],0.5,buffer.v[i][j][k]);
                pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
                pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
            }
    
    computeAcceleration(&buffer, a);
    
    
    for (i=0; i<=7; i++)
        for (j=0; j<=7; j++)
            for (k=0; k<=7; k++)
            {
                // F3p = dt * buffer.v;
                pMULTIPLY(buffer.v[i][j][k],jello->dt,F4p[i][j][k]);
                // F3v = dt * a(buffer.p,buffer.v);     
                pMULTIPLY(a[i][j][k],jello->dt,F4v[i][j][k]);
                
                pMULTIPLY(F2p[i][j][k],2,buffer.p[i][j][k]);
                pMULTIPLY(F3p[i][j][k],2,buffer.v[i][j][k]);
                pSUM(buffer.p[i][j][k],buffer.v[i][j][k],buffer.p[i][j][k]);
                pSUM(buffer.p[i][j][k],F1p[i][j][k],buffer.p[i][j][k]);
                pSUM(buffer.p[i][j][k],F4p[i][j][k],buffer.p[i][j][k]);
                pMULTIPLY(buffer.p[i][j][k],1.0 / 6,buffer.p[i][j][k]);
                pSUM(buffer.p[i][j][k],jello->p[i][j][k],jello->p[i][j][k]);
                
                pMULTIPLY(F2v[i][j][k],2,buffer.p[i][j][k]);
                pMULTIPLY(F3v[i][j][k],2,buffer.v[i][j][k]);
                pSUM(buffer.p[i][j][k],buffer.v[i][j][k],buffer.p[i][j][k]);
                pSUM(buffer.p[i][j][k],F1v[i][j][k],buffer.p[i][j][k]);
                pSUM(buffer.p[i][j][k],F4v[i][j][k],buffer.p[i][j][k]);
                pMULTIPLY(buffer.p[i][j][k],1.0 / 6,buffer.p[i][j][k]);
                pSUM(buffer.p[i][j][k],jello->v[i][j][k],jello->v[i][j][k]);
            }
    return;  
}