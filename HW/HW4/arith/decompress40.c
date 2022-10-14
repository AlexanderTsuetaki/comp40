#include "decompress40.h"


typedef struct pix4{
        uint64_t a;
        uint64_t b;
        uint64_t c;
        uint64_t d;
        float pb;
        float pr;
} pix4;

pix4 fromcodeword(uint32_t word){
	pix4 stc;
	stc.a =  Bitpack_getu((uint64_t)word, 9, 23);
	stc.b =  Bitpack_getu((uint64_t)word, 5, 18);
	stc.c =  Bitpack_getu((uint64_t)word, 5, 13);
	stc.d =  Bitpack_getu((uint64_t)word, 5, 8);
	stc.pb = Arith40_chroma_of_index(Bitpack_getu((uint64_t)word, 4, 4));
	stc.pr = Arith40_chroma_of_index(Bitpack_getu((uint64_t)word, 4, 0));
	return stc
}

void unapplyBPack(Uarray2b_T tarr, int col, int row, pix4 px){
	comxyz * pix01 = UArray2b_at(tarr,col*2, row*2);
	(*pix01).y = px.a - px.b -px.c + px.d;	
	(*pix01).pb = px.pb;
	(*pix01).pr = px.pr;
	comxyz * pix02 = UArray2b_at(tarr,col*2+1, row*2);
	(*pix02).y = px.a - px.b + px.c - px.d;
	(*pix02).pb = px.pb;
	(*pix02).pr = px.pr;
	comxyz * pix03 = UArray2b_at(tarr,col*2, row*2+1);
	(*pix03).y = px.a + px.b - px.c - px.d;
	(*pix03).pb = px.pb;
	(*pix03).pr = px.pr;
	comxyz * pix04 = UArray2b_at(tarr,col*2+1, row*2+1);	
	(*pix04).y = px.a + px.b + px.c + px.d;
	(*pix04).pb = px.pb;
	(*pix04).pr = px.pr;
}

void toRGB(Uarray2b_T tarr){

}

uint32_t in_BE(char[4] parts){
	uint32_t word;
	word += parts[3] << 24;
	word += parts[2] << 16;
	word += parts[1] << 8;
	word += parts[0];
	return word;
}
