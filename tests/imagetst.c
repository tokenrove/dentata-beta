/* 
 * imagetst.c
 * Created: Mon Jun 25 23:19:35 2001 by tek@wiw.org
 * Revised: Mon Jun 25 23:55:59 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/error.h>
#include <dentata/random.h>

#define TESTWIDTH     640
#define TESTHEIGHT    480
#define TESTBPP       24
#define NRANDTESTITER 100

bool test_imagenew(d_random_t *);
bool haserrors(void);

int main(void)
{
    bool status;
    d_random_t *rh;

    /* set 1: d_image_new tests */
    status = test_imagenew(rh);
    if(status == failure)
        d_error_fatal("set 1 (d_image_new tests) failed.");
    return 0;
}

bool test_imagenew(d_random_t *rh)
{
    d_rasterdescription_t desc;
    d_image_t *p;
    int i;

    /* test 1: create a valid image with each alpha */
    desc.w = TESTWIDTH; desc.h = TESTHEIGHT;
    desc.bpp = TESTBPP;
    desc.cspace = RGB;
    desc.paletted = false;
    /* zero alpha */
    desc.alpha = 0;

    p = d_image_new(desc);

    if(p == NULL || haserrors() == true) {
        d_error_push("test 1 failed.");
        return failure;
    }

    d_image_delete(p);
    /* non-zero alpha */
    for(i = 1; i < 8; i <<= 1) {
        desc.alpha = i;

        p = d_image_new(desc);

        if(p == NULL || haserrors() == true) {
            d_error_push("test 1 failed.");
            return failure;
        }

        d_image_delete(p);
    }

    /* test 2: create an invalid image with each alpha */
    desc.w = 0; desc.h = 0;
    desc.bpp = 0;
    desc.cspace = RGB;
    desc.paletted = false;
    /* zero alpha */
    desc.alpha = i;

    p = d_image_new(desc);

    if(p != NULL || haserrors() == false) {
        d_error_push("test 2 failed.");
        return failure;
    }
    /* non-zero alpha */
    for(i = 1; i < 8; i <<= 1) {
        desc.alpha = i;

        p = d_image_new(desc);

        if(p != NULL || haserrors() == false) {
            d_error_push("test 2 failed.");
            return failure;
        }
    }

    /* test 3: random values */
    for(i = 0; i < NRANDTESTITER; i++) {
        desc.w = d_random_gen(rh, 1<<31);
        desc.h = d_random_gen(rh, 1<<31);
        desc.bpp = d_random_gen(rh, 1<<31);
        desc.paletted = d_random_gen(rh, 1<<31);
        desc.cspace = d_random_gen(rh, 1<<31);

        p = d_image_new(desc);
        if(p != NULL)
            d_image_delete(p);
        /* clean up the error stack -- we don't care, just as
         * long as the test didn't crash. */
        while(d_error_pop() != NULL);
    }

    return success;
}

bool haserrors(void)
{
    char *s;

    s = d_error_pop();
    if(s != NULL) {
        d_error_push(s);
        d_error_dump();
        return true;
    }
    return false;
}
    
/* EOF imagetst.c */
