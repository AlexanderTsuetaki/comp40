/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <seq.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        uint32_t inst = 0 | (op << 28);
	inst = inst | (ra << 6);
	inst = inst | (rb << 3);
	inst = inst | (rc);
	printf("op %u, ra %d, rb %d, rc %d \n", op,ra,rb,rc);
	printf("inst: %"PRIx32"\n",inst); 
	return inst;
}
Um_instruction loadval(unsigned ra, unsigned val)
{
	uint32_t inst = 13 << 28;
        inst = inst | (ra << 25);
	inst = inst | val;

	printf("ra %u, val %u \n", ra,val);
	printf("inst: %"PRIx32"\n",inst); 
	return inst;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

Um_instruction output(Um_register c)
{
	return three_register(OUT , 0 , 0 , c);
}

/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

void Um_write_sequence(FILE *output, Seq_T stream){
	for(int i = 0; i < Seq_length(stream);i++){
		void *sec = Seq_get(stream, i);
		uint32_t data = (uint32_t)(uintptr_t)sec;
		printf("inst: %"PRIx32"\n",data); 
		uint32_t mask = 0x000000ff;
		
		char a = (data >> 24) & mask;
		char b = (data >> 16) & mask;
		char c = (data >> 8) & mask;
		char d = data & mask;
		fputc(a,output);
		fputc(b,output);
		fputc(c,output);
		fputc(d,output);
	}
		
}


/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        emit(stream, halt());
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
}

void emit_add(Seq_T stream){
	emit(stream, add(r1, r2, r3));
	emit(stream, halt());
}

void emit_print_six(Seq_T stream){
	
        emit(stream, loadval(r1, 48));
        emit(stream, loadval(r1, 6));
	emit(stream, three_register(ADD, r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}
