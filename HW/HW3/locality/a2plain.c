#include <string.h>

#include <a2plain.h>
#include <uarray2.h>

/************************************************/
/* Define a private version of each function in */
/* A2Methods_T that we implement.               */
/************************************************/
typedef A2Methods_UArray2 A2;

static A2 new(int width, int height, int size)
{
	return UArray2_new(width, height, size); 
}

static A2 new_with_blocksize(int width, int height, int size, int blocksize)
{
	(void) blocksize;
	return UArray2_new(width, height, size); 
}

static void a2free(A2 *arr){
	UArray2_free((UArray2_T*)arr);
}

static int width(A2 arr){
	return UArray2_width(arr);
}
static int height(A2 arr){
	return UArray2_height(arr);
}
static int size(A2 arr){
	return UArray2_size(arr);
}
static int blocksize(A2 arr) {
	(void) arr;
	return 1;
}

static A2Methods_Object *at(A2 array2, int i , int j){
	return UArray2_at(array2,i,j);
}

typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);

static void map_row_major(A2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
	UArray2_map_row_major(uarray2, (applyfun *)apply, cl);
}

static void map_col_major(A2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
	UArray2_map_col_major(uarray2, (applyfun *)apply, cl);
}

struct small_closure {
  A2Methods_smallapplyfun *apply; 
  void                    *cl;
};

static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
  struct small_closure *cl = vcl;
  (void)i;
  (void)j;
  (void)uarray2;
  cl->apply(elem, cl->cl);
}

static void small_map_row_major(A2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
  struct small_closure mycl = { apply, cl };
  UArray2_map_row_major(a2, apply_small, &mycl);
}

static void small_map_col_major(A2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
  struct small_closure mycl = { apply, cl };
  UArray2_map_col_major(a2, apply_small, &mycl);
}


static struct A2Methods_T uarray2_methods_plain_struct = {
  new,
  new_with_blocksize,
  a2free,
  width,
  height,
  size,
  blocksize,
  at,
  map_row_major,
  map_col_major,
  NULL,
  map_row_major,
  small_map_row_major,
  small_map_col_major,
  NULL,
  small_map_row_major
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
