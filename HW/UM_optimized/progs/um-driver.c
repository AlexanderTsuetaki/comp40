#include <seq.h>
#include <inttypes.h>
#include <bitpack.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef uint32_t seg_id;
typedef uint32_t word;
#define HALT 0x70000000

void test_um();

word *get_prog(FILE *file);

void run_program(FILE *file);

Seq_T setup_memory(word *prog);

Seq_T setup_recycle();

uint32_t *setup_registers();

word get_instruction(Seq_T mem, uint32_t prog_counter);

int parse_instruction(Seq_T mem, Seq_T freed_mem, uint32_t *registers,
                      word inst, uint32_t *count);

void run_instruction(Seq_T mem, Seq_T freed_mem, uint32_t *registers,seg_id op,
                     seg_id rega, seg_id regb, seg_id regc);


seg_id translate_address(Seq_T ids, uint32_t id);

//op code instructions + helpers
void conditional_move(uint32_t * regs, seg_id rega, seg_id regb, seg_id regc);

void segmented_load(Seq_T mem, uint32_t * regs, seg_id rega, seg_id regb,
                    seg_id regc);

void segmented_store(Seq_T mem, uint32_t * regs,  seg_id rega, seg_id regb,
                     seg_id regc);

void addition(uint32_t * regs, seg_id rega, seg_id regb, seg_id regc);

void multiplication(uint32_t * regs, seg_id rega, seg_id regb, seg_id regc);

void division(uint32_t * regs, seg_id rega, seg_id regb, seg_id regc);

void nand(uint32_t * regs, seg_id rega, seg_id regb, seg_id regc);

void map_segment(Seq_T mem, uint32_t * regs, Seq_T freed_mem, seg_id rega,
                 seg_id regb, seg_id regc);

seg_id map_segment_helper(Seq_T mem, Seq_T freed_mem, word *seg);

void unmap_segment(Seq_T mem, uint32_t * regs, Seq_T freed_mem, seg_id rega,
                   seg_id regb, seg_id regc);

void unmap_segment_helper(Seq_T mem, Seq_T freed_mem, seg_id index);

void output(uint32_t * regs, seg_id rega, seg_id regb, seg_id regc);

void input(uint32_t * regs, seg_id rega, seg_id regb, seg_id regc);

void load_program(Seq_T mem, uint32_t * regs, uint32_t *p_counter, seg_id rega,
                  seg_id regb, seg_id regc);

void load_value(uint32_t * regs, seg_id rega, uint32_t value);


/*
  main
  Purpose: runs the program
  Args: argc: the number of arguments being passed into the function
  argv: the arguments
  Returns: an int denoting success or failure in the running of the run_program
  it is in pristine condition.
*/
int main (int argc, char *argv[])
{
        if(argc != 2) {
                fprintf(stderr, "Incorrect number of arguments. Exiting.\n");
                exit(EXIT_FAILURE);
        }
        FILE *file = fopen(argv[1],"r");
        if (!file) {
                fprintf(stderr, "No file found. Exiting.\n");
                exit(EXIT_FAILURE);
        }

        run_program(file);

        fclose(file);
        return 0;
}

/*
  run_program
  Purpose: to run the .um program instructions
  Args: file: a file comtaining the instructions for a program
  Returns: none
*/
void run_program(FILE *file) {
        char *char_seq = malloc(64);
        char chr = fgetc(file);
        uint32_t char_ctr = 0;
        uint32_t max_len = 64;
        while (!feof(file)) {
                char_seq[char_ctr++] = chr;
                if (char_ctr == max_len) {
                        char *temp_seq = malloc(max_len);
                        for (uint32_t i = 0; i < max_len; i++)
                                temp_seq[i] = char_seq[i];
                        free(char_seq);
                        char_seq = NULL;
                        char_seq = malloc(max_len * 2);
                        for (uint32_t i = 0; i < max_len; i++)
                                char_seq[i] = temp_seq[i];
                        max_len *= 2;
                        free(temp_seq);
                        temp_seq = NULL;
                }
                chr = fgetc(file);
        }
        assert(char_ctr % 4 == 0);
        word *prog= malloc((char_ctr / 4) * sizeof(word));
        word temp = 0;
        for(uint32_t i = 0; i < char_ctr; i++){
                temp = Bitpack_newu(temp, 8 , 24 - 8 * (i % 4), 
                                    (char_seq[i]) & 0xff);
                if(i % 4 == 3){
                        prog[i >> 2] = temp;
                        temp = 0;
                }
        }
        free(char_seq);
        uint32_t mem_max_len = 256;
        uint32_t mem_curr_len = 0;
        uint32_t **mem = malloc(mem_max_len * sizeof(uint32_t *));
        mem[mem_curr_len++] = prog;

        uint32_t freed_mem_max_len = 128;
        uint32_t freed_mem_curr_len = 0;
        uint32_t *freed_mem = malloc(mem_max_len * sizeof(uint32_t));

        uint32_t prog_counter = 0;

        uint32_t *registers = malloc(8*sizeof(uint32_t));
        for (int i = 0; i < 8; i++){
                registers[i] = 0;
        }

        while (1) {
                word inst = (mem[0])[prog_counter];
                prog_counter++;
        
                uint32_t op = inst >> 28;
                if (op == 7) {
                        free(prog);
                        mem[0] = NULL;
                        break;
                }
                if (op == 13){
                        uint32_t a  = inst << 4;
                        a = a >> 29;
                        uint32_t value  = inst << 7;
                        value = value >> 7;
                        registers[a] = value;
                }
            
                uint32_t a  = inst << 23;
                a = a >> 29;
                uint32_t b  = inst << 26;
                b = b >> 29;
                uint32_t c  = inst << 29;
                c = c >> 29;
            
                if (op == 12) {
                        if (registers[b] != 0)
                                mem[0] = mem[registers[b]];
                        prog_counter = registers[c];
                }
                else{
                        if (op==1) {
                                registers[a]=(mem[registers[b]])[registers[c]];
                        }
                        if (op==2) {
                                (mem[registers[a]])[registers[b]]=registers[c];
                        }
                        if (op==8) {
                                //fprintf(stderr, "%u\n", registers[c]);
                                uint32_t *seg = 
                                        malloc((registers[c])*
                                               sizeof(uint32_t));
                                for(seg_id i = 0; i < registers[c]; i++)
                                        seg[i] = 0;
                                
                                if(freed_mem_curr_len > 0) {
                                        seg_id index =
                                                freed_mem[freed_mem_curr_len-1];
                                        freed_mem[freed_mem_curr_len-1] = 0;
                                        freed_mem_curr_len--;
                                        mem[index] = seg;
                                        registers[b] = index;
                                }
                                else{
                                        mem[mem_curr_len++] = seg;
                                        if (mem_curr_len == mem_max_len) {
                                                uint32_t **temp_seq = 
                                                        malloc(mem_max_len *
                                                        sizeof(uint32_t *));
                                                for (uint32_t i = 0; 
                                                     i < mem_max_len; i++)
                                                        temp_seq[i] = mem[i];
                                                free(mem);
                                                mem = NULL;
                                                mem = malloc(mem_max_len*2 *
                                                         sizeof(uint32_t *));
                                                for (uint32_t i = 0; 
                                                     i < mem_max_len; i++)
                                                        mem[i] = temp_seq[i];
                                                mem_max_len *= 2;
                                                //free(temp_seq);
                                                temp_seq = NULL;
                                        }
                                        registers[b] = mem_curr_len-1;
                                }
                        }
                        if (op==9) {
                                seg_id index = registers[c];
                                if(mem[index] == NULL){
                                        fprintf(stderr, "Double free at %d\n",
                                                index);
                                        uint32_t arr[1] = {HALT};
                                        mem[0] = arr;
                                        return;
                                }
                                freed_mem[freed_mem_curr_len++] = index;
                                if (freed_mem_curr_len == freed_mem_max_len) {
                                        uint32_t *temp_seq =
                                                malloc(freed_mem_max_len *
                                                       sizeof(uint32_t));
                                        for (uint32_t i = 0; 
                                             i < freed_mem_max_len; i++)
                                                temp_seq[i] = freed_mem[i];
                                        free(freed_mem);
                                        freed_mem = NULL;
                                        freed_mem = malloc(freed_mem_max_len*
                                                           2*sizeof(uint32_t));
                                        for (uint32_t i = 0; 
                                             i < freed_mem_max_len; i++)
                                                freed_mem[i] = temp_seq[i];
                                        freed_mem_max_len *= 2;
                                        free(temp_seq);
                                        temp_seq = NULL;
                                }
                                uint32_t *temp = mem[index];
                                free(temp);
                                mem[index] = NULL;
                        }
                        if (op==0) {
                                if(registers[c] != 0)
                                        registers[a] = registers[b];
                        }
                        if (op==3) {
                                registers[a] = registers[b] + registers[c];
                        }
                        if (op==4) {
                                registers[a] = (registers[b]) * (registers[c]);
                        }
                        if (op==5) {
                                registers[a] = (registers[b]) / (registers[c]);
                        }
                        if (op==6) {
                                registers[a] = ~(registers[b] & registers[c]);
                        }
                        if (op==10) {
                                char chr = registers[c];
                                putchar(chr);
                        }
                        if (op==11) {
                                char chr;
                                scanf("%c", &chr);
                                if(feof(stdin)){
                                        uint32_t temp = 0;
                                        temp = ~temp;
                                        registers[c] = temp;
                                } else {
                                        registers[c] = chr;
                                }
                        }
                }
        }
        free(registers);
        while(mem_curr_len > 0)
                {
                        uint32_t *data = mem[mem_curr_len-1];
                        mem_curr_len--;
                        if(data)
                                free(data);
                }
        free(mem);
        free(freed_mem);
}

