#include "compress40.h"
#include <stdio.h>
#include "uarray2b.h"

typedef struct pix4{
        int a;
        int b;
        int c;
	int d;
	int pb;
	int pr;
}pix4;

Pnm_ppm trimWidth(Pnm_ppm img){
	Uarray2b_T * temparr = UArray2b_new((UArray2b_height(img)/2)*2,
					    (UArray2b_width(img)/2)*2,
					    UArray2b_size(img));
	if(UArray2b_height(*(img->pixels))%2 == 1 || 
	   UArray2b_width(*(img->pixels))%2 ==1){
		UArray2b_map(img->pixels, copy_some, temparr);
		return temparr;
	}
	else{
		return img;
	}
}

void copy_some(int col, int row, Uarray2b_t arr, void *elem, void *cl){
	pnm_rgb * temp;
	if(col != UArray2b_width(arr) || col%2 == 0){
		if(row != UArray2b_height(arr) || row%2 == 0){
			temp = UArray2b_at(*((Uarray2b_t*)cl), col, row);
			(*temp)= (*(pnmrgb*)elem);
		}
	}
}


uint32_t tocodeword(pix4 code){
	uint32_t word = 0;
	word = Bitpack_newu(word, 9, 23, code.a); 
	word = Bitpack_newu(word, 5, 18, code.b); 
	word = Bitpack_newu(word, 5, 13, code.c); 
	word = Bitpack_newu(word, 5, 8, code.d); 
	word = Bitpack_newu(word, 4, 4, 
			    (uint64_t)Arith40_index_of_chroma(code.pb)); 
	word = Bitpack_newu(word, 4, 0, 
			    (uint64_t)Arith40_index_of_chroma(code.pr));
	return word;
}

void compress(Uarray2b_T st){
	printf("COMP40 Compressed image format 2/n%u %u", 
	       UArray2b_width(st), UArray2b_height(st));
	pix4 px[(UArray2b_width(st)/UArray2b_size(st))][(UArray2b_height(st)
							 /UArray2b_size(st))];
	UArray2b_map(st, applyBPack, px); 
	for(int i = 0; i < (UArray2b_width(st)/UArray2b_size(st)); i++){
		for(int j = 0; j < (UArray2b_height(st)/UArray2b_size(st));i++){
			out_BE(tocodeword(px[i][j]));
		}
	}
}
void compressPix(int col, int row, Uarray2b_T array2b, void *elem, void *cl){
	pix4 * px = ;
	(*elem).y/4 
	if(col%2 == 0 && row%2  = 0){
		(*px).a += (*elem).y/4.0;
		(*px).b -= (*elem).y/4.0;
		(*px).c -= (*elem).y/4.0;
		(*px).d += (*elem).y/4.0;
	}
	else if(col%2 == 1 && row%2  = 0){
		(*px).a += (*elem).y/4.0;
		(*px).b -= (*elem).y/4.0;
		(*px).c += (*elem).y/4.0;
		(*px).d -= (*elem).y/4.0;
	}
	else if(col%2 == 0 && row%2  = 1){
		(*px).a += (*elem).y/4.0;
		(*px).b += (*elem).y/4.0;
		(*px).c -= (*elem).y/4.0;
		(*px).d -= (*elem).y/4.0;
	}
	else{
		(*px).a += (*elem).y/4.0;
		(*px).b += (*elem).y/4.0;
		(*px).c += (*elem).y/4.0;
		(*px).d += (*elem).y/4.0;
	}	
}




void out_BE(uint32_t word){
	char a = word >> 24;
	char b = (word >> 16) && 0xff;
	char c = (word >> 8) && 0xff;
	char d = word && 0xff;
	putchar(d);
	putchar(c);
	putchar(b);
	putchar(a);
}

uint
