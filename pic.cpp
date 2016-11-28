#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pic.h"

/*
 * pic_alloc: allocate picture memory.
 * If opic!=0, then memory from opic->pix is reused (after checking that
 *    size is sufficient), else a new pixel array is allocated.
 * Great caution should be used when freeing, if pix memory is reused!
 */
Pic *pic_alloc(int nx, int ny, int bytes_per_pixel, Pic *opic) 
{
  Pic *p;
  int size = ny*nx*bytes_per_pixel;

  ALLOC(p, Pic, 1);
  p->nx = nx;
  p->ny = ny;
  p->bpp = bytes_per_pixel;
  if (opic && opic->nx*opic->ny*opic->bpp >= p->nx*p->ny*p->bpp) 
  {
    p->pix = opic->pix;
    /* now opic and p have a common pix array */
  }
  else
    ALLOC(p->pix, Pixel1, size);
  return p;
}

void pic_free(Pic *p) 
{
  free(p->pix);
  free(p);
}

/*
 * pic_xxx routines will call ppm_xxx or tiff_xxx depending on the
 * type of file.
 *
 * Currently, this is rather inefficient.  We open the file, check the
 * magic number, close it, and then reopen for the actual operation.
 * It would probably be good to add a further level of interface which
 * allowed us to pass around file descriptors in addition to file names.
 *
 * Michael Garland      17 Jan 96
 *
 */

Pic_file_format pic_file_type(char *file)
{
  unsigned char byte[10];
    int i;

  FILE *pic = fopen(file, "r");
  if( !pic )
    return PIC_UNKNOWN_FILE;

  return PIC_PPM_FILE;

/*
 for (i = 0; i < 10; i++) 
   byte[i] = getc(pic);
		
  fclose(pic);
		
  if( byte[0]=='P' && (byte[1]=='3' || byte[1]=='6') )
    return PIC_PPM_FILE;
  else 
    if( (byte[0]==0x4d && byte[1]==0x4d) || (byte[0]==0x49 && byte[1]==0x49) )
      return PIC_TIFF_FILE;
    else if ( byte[0]==0xff && byte[1]==0xd8 && byte[2]==0xff && byte[3]==0xe0 && strncmp(&byte[6], "JFIF", 4))
      return PIC_JPEG_FILE;
    else
      return PIC_UNKNOWN_FILE;
*/
}

Pic_file_format pic_filename_type(char *file)
{
  char *suff;

  suff = strrchr(file, '.');
  if (!strcmp(suff, ".jpg")) return PIC_JPEG_FILE;
  if (!strcmp(suff, ".tiff") || !strcmp(suff, ".tif")) return PIC_TIFF_FILE;
  if (!strcmp(suff, ".ppm")) return PIC_PPM_FILE;
  return PIC_UNKNOWN_FILE;
}

int pic_get_size(char *file, int *nx, int *ny)
{
  switch( pic_file_type(file) )
  {
    case PIC_TIFF_FILE:
	//return tiff_get_size(file, nx, ny);
    break;
			
    case PIC_PPM_FILE:
	return ppm_get_size(file, nx, ny);
    break;

    case PIC_JPEG_FILE:
	//return jpeg_get_size(file, nx, ny);
    break;
			
    default:
        return FALSE;
  }
}

/*
 * pic_read: read a TIFF or PPM file into memory.
 * Normally, you should use opic==NULL.
 * If opic!=NULL, then picture is read into opic->pix (after checking that
 * size is sufficient), else a new Pic is allocated.
 * Returns Pic pointer on success, NULL on failure.
 */
Pic *pic_read(char *file, Pic *opic)
{
  switch( pic_file_type(file) )
  {
    case PIC_TIFF_FILE:
      //return tiff_read(file, opic);
    break;
			
    case PIC_PPM_FILE:
      return ppm_read(file, opic);
    break;

    case PIC_JPEG_FILE:
      //return jpeg_read(file, opic);
    break;

    default:
      return NULL;
    }
}

/*
 * pic_write: write pic to file in the specified format
 * returns TRUE on success, FALSE on failure
 */
int pic_write(char *file, Pic *pic, Pic_file_format format)
{
  switch( format )
  {
    case PIC_TIFF_FILE:
      //return tiff_write(file, pic);
    break;
			
    case PIC_PPM_FILE:
      return ppm_write(file, pic);
    break;

    case PIC_JPEG_FILE:
     //return jpeg_write(file, pic);
    break;
			
    default:
      fprintf(stderr, "pic_write: can't write %s, unknown format\n", file);
      return FALSE;
  }
}

