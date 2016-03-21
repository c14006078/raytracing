#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "primitives.h"
#include "raytracing.h"

#define OUT_FILENAME "out.ppm"

#define ROWS 512
#define COLS 512

static void write_to_ppm(FILE *outfile, uint8_t *pixels,
                         int width, int height)
{
    fprintf(outfile, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(pixels, 1, height * width * 3, outfile);
}

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main()
{
    uint8_t *pixels;
    light_node lights = NULL;
    rectangular_node rectangulars = NULL;
    sphere_node spheres = NULL;
    color background = { 0.0, 0.1, 0.1 };
    struct timespec start, end;

#include "use-models.h"

    /* allocate by the given resolution */
    pixels = malloc(sizeof(unsigned char) * ROWS * COLS * 3);
    if (!pixels) exit(-1);

    int threadnum;
    printf( " please input the thread num:");
    scanf( " %d",  &threadnum);

    printf("\n\n# Rendering scene\n");
    /* do the ray tracing with the given geometry */
    clock_gettime(CLOCK_REALTIME, &start);

    pthread_t * tid = ( pthread_t *) malloc ( threadnum * sizeof( pthread_t));
    /*const viewpoint * vpt = &view;
    printf("fuck i go to\n");
    printf("vpt->view->vpn[0] = %lf\n",vpt->vpn[0]);*/
    int k;
    rays** pr = (rays **) malloc( threadnum * sizeof(  rays * ));
    for( k = 0; k < threadnum; k++){
        /*viewpoint * vp = (viewpoint *) malloc( sizeof(viewpoint));*/
        pr[k] = new_rays( pixels, background,
               rectangulars, spheres, lights, &view, ROWS, COLS, k, threadnum);
        pthread_create( tid+k, NULL, (void *) &raytracing, (void *) pr+k);
    }

    for( k = 0; k < threadnum; k++){
        pthread_join( tid[k], NULL);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    {
        FILE *outfile = fopen(OUT_FILENAME, "wb");
        write_to_ppm(outfile, pixels, ROWS, COLS);
        fclose(outfile);
    }

    delete_rectangular_list(&rectangulars);
    delete_sphere_list(&spheres);
    delete_light_list(&lights);
    free(pixels);
    printf("Done!\n");
    printf("Execution time of raytracing() : %lf sec\n", diff_in_second(start, end));
    return 0;
}
