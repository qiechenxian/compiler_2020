#include <cstdio>
#include <unistd.h>
#include "util.h"
#include "ast.h"
#include "semant.h"
#include "env.h"
#include "prast.h"
#include "cannon.h"
#include "print_ir_tree.h"
#include "codegen.h"

using namespace std;
static void doProc(F_frame frame, T_stm body)
{

    FILE *out=stderr;

    AS_proc proc;
    //struct RA_result allocation;
    T_stmList stmList;
    AS_instrList iList;

    F_tempMap=Temp_empty();

    stmList = C_linearize(body);
    stmList = C_traceSchedule(C_basicBlocks(stmList));
    printcannoList (stderr,stmList);


    //fprintf(out,"111");
    /* printStmList(stdout, stmList);*/
    iList  = F_codegen(frame, stmList); /* 9 */

    fprintf(out, "BEGIN %s\n", Temp_labelString(F_getName(frame)));
    AS_printInstrList (out, iList,Temp_layerMap(F_tempMap,Temp_name()));
    fprintf(out, "END %s\n\n", Temp_labelString(F_getName(frame)));
}

extern FILE *yyin;
extern FILE *yyout;
extern char *yytext;

extern int yyparse();

/**
 * 跨文件全局变量
 */
extern A_decList absyn_root;
c_string INPUT_FILE;
c_string OUTPUT_FILE;


int main(int argc, char **argv) {

    F_fragList frags;
    /**
     * 参数处理 --loyx 2020/6/15
     */
    int option;
    while ((option = getopt(argc, argv, "So:O::")) != -1) {
        switch (option) {
            case 'o':
                OUTPUT_FILE = String(optarg);
                break;
            case 'O':
                if (*optarg == '2') {
                    // todo O2
                } else {
                    // todo O0
                }
                break;
            default:
                // 此处匹配-S选项
                break;
        }
    }
    INPUT_FILE = argv[optind];


    /**
     * open the input file
     */
    yyin = fopen(INPUT_FILE, "r");
    yyout = stdout;
    FILE_NAME = INPUT_FILE; // todo refactor?


    S_table tenv = E_base_typeEntry();
    S_table venv = E_base_valueEntry(tenv);
    yyparse();
    fprintf(stderr, "before semantic ast:\n");
    pr_decList(stderr, absyn_root, 0);
    fprintf(stderr, "\n");

    frags=SEM_transProgram(venv, tenv, absyn_root);
    printStmList(stderr,frags);
    fprintf(stderr, "\nsemantic check finish !\n");
        for (;frags;frags=frags->tail){
            if (frags->head->kind == F_frag_::F_procFrag)
            {
                doProc(frags->head->u.proc.frame, frags->head->u.proc.body);
            }
            //doProc(out, frags->head->u.proc.frame, frags->head->u.proc.body);
            //else if (frags->head->kind == F_frag_::F_stringFrag)
            //fprintf(out, "%s\n", frags->head->u.stringg.str);
    }

    fprintf(stderr, "after semantic ast:\n");
    pr_decList(stderr, absyn_root, 0);
    return 0;
}





