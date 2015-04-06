#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

#include "umlab.h"

extern void Um_write_sequence(FILE *output, Seq_T asm);

extern void emit_halt_test(Seq_T asm);
extern void emit_goto_test(Seq_T asm);
extern void emit_load_test(Seq_T asm);
extern void emit_out_test(Seq_T asm);
extern void emit_add_test(Seq_T asm);
extern void emit_add_neg_test(Seq_T asm);
extern void emit_in_test(Seq_T asm);
extern void emit_multiply_test(Seq_T asm);
extern void emit_divide_test(Seq_T asm);
extern void emit_nand_test(Seq_T asm);
extern void emit_segload_test(Seq_T asm);
extern void emit_cmov_test(Seq_T asm);
extern void emit_segstore_test(Seq_T asm);
extern void emit_mapseg_test(Seq_T asm);
extern void emit_unmapseg_test(Seq_T asm);
extern void emit_loadprog_test(Seq_T asm);

static struct test_info {
        const char *name;
        const char *test_input;            /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*emit_test) (Seq_T stream);
} tests[] = {
        {"halt", NULL, "", emit_halt_test},
	{"load", NULL, "", emit_load_test},
	{"output", NULL, "Out\n", emit_out_test},
	{"add", NULL, "4+2=f\n", emit_add_test},
	{"input", "A", "A\n", emit_in_test},
	{"multiply", NULL, "=$\n", emit_multiply_test},
	{"divide", NULL, "=4\n", emit_divide_test},
	{"nand", NULL, "d\n", emit_nand_test},
        {"seg_load", NULL, "dddd\n", emit_segload_test},
        {"goto", NULL, "GOTO passed.\n", emit_goto_test},
        {"cmov", NULL, "Yes!\n", emit_cmov_test},
        {"seg_store", NULL, "{}", emit_segstore_test},
        {"map", NULL, "Pass!\n", emit_mapseg_test},
        {"unmap", NULL, "Y\n", emit_unmapseg_test},
        {"loadprog", NULL, "Y", emit_loadprog_test},
};

#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T asm = Seq_new(0);
        test->emit_test(asm);
        Um_write_sequence(binary, asm);
        Seq_free(&asm);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name), test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}

int main(int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
        } else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed;   /* failed nonzero == exit nonzero == failure */
}

static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input);
                fputs(contents, input);
                fclose(input);
        }
        free(path);
}

static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
