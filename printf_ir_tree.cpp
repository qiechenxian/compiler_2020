//
// Created by 且趁闲 on 22/7/2020.
//

/*
 * printtree.c - functions to print out intermediate representation (IR) trees.
 *
 */
#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "print_ir_tree.h"
#include "frame.h"
FILE* out=fopen("ir_tree_print.txt","w+");

/* local function prototype */
static void pr_tree_exp(FILE *out, T_exp exp, int d);

static void indent(FILE *out, int d) {
    int i;
    for (i = 0; i <= d; i++) printf(" ");
}

static char bin_oper[][12] = {
        "T_BINOP", "T_MEM", "T_TEMP", "T_ESEQ", "T_NAME", "T_CONST", "T_CALL","T_RELOP"};

static char rel_oper[][12] = {
        "T_lt", "T_le", "T_gt", "T_ge", "T_eq", "T_ne","T_not","T_and", "T_or"};

static void pr_stm(FILE *out, T_stm stm, int d)
{
    if(stm== nullptr)
    {
        printf("nullptr");
        return;
    }
    switch (stm->kind) {
        case T_stm_::T_SEQ:
            indent(out,d);
            printf("SEQ(\n");
            pr_stm(out, stm->u.SEQ.left,d+1);
            printf(",\n");
            pr_stm(out, stm->u.SEQ.right,d+1);
            printf(")");
            break;
        case T_stm_::T_LABEL:
            indent(out,d); printf("LABEL %s", S_name(stm->u.LABEL));
            break;
        case T_stm_::T_JUMP:
            indent(out,d); printf( "JUMP(\n"); pr_tree_exp(out, stm->u.JUMP.exp,d+1);
            printf(")");
            break;
        case T_stm_::T_CJUMP:
            indent(out,d); printf("CJUMP(%s,\n", rel_oper[stm->u.CJUMP.op]);
            pr_tree_exp(out, stm->u.CJUMP.left,d+1); printf(",\n");
            pr_tree_exp(out, stm->u.CJUMP.right,d+1); printf(",\n");
            indent(out,d+1);
            printf("%s,", S_name(stm->u.CJUMP.trues));
            printf("%s", S_name(stm->u.CJUMP.falses));
            printf(")");
            break;
        case T_stm_::T_MOVE:
            indent(out,d); printf( "MOVE(\n");
            pr_tree_exp(out, stm->u.MOVE.dst,d+1);
            printf(",\n");
            pr_tree_exp(out, stm->u.MOVE.src,d+1);
            printf(")");
            break;
        case T_stm_::T_EXP:
            indent(out,d);
            printf("EXP(\n");
            pr_tree_exp(out, stm->u.EXP,d+1);
            printf(")");
            break;
    }
}

static void pr_tree_exp(FILE *out, T_exp exp, int d)
{
    switch (exp->kind) {
        case T_exp_::T_BINOP:
            indent(out,d);
            printf("BINOP(%s,\n", bin_oper[exp->u.BINOP.op]);
            pr_tree_exp(out, exp->u.BINOP.left,d+1);
            printf(",\n");
            pr_tree_exp(out, exp->u.BINOP.right,d+1);
            printf(")");
            break;
        case T_exp_::T_MEM:
            indent(out,d);
            printf( "MEM");
            printf("(\n");
            pr_tree_exp(out, exp->u.MEM,d+1);
            printf(")");
            break;
        case T_exp_::T_TEMP:
            indent(out,d);
            printf( "TEMP t%s",Temp_look(Temp_name(), exp->u.TEMP));
            break;
        case T_exp_::T_ESEQ:
            indent(out,d);
            printf( "ESEQ(\n"); pr_stm(out, exp->u.ESEQ.stm,d+1);
            printf( ",\n");
            pr_tree_exp(out, exp->u.ESEQ.exp,d+1);
            printf( ")");
            break;
        case T_exp_::T_NAME:
            indent(out,d); printf("NAME %s", S_name(exp->u.NAME));
            break;
        case T_exp_::T_CONST:
            indent(out,d); printf("CONST %d", exp->u.CONST);
            break;
        case T_exp_::T_CALL:
        {T_expList args = exp->u.CALL.args;
            indent(out,d); printf("CALL(\n"); pr_tree_exp(out, exp->u.CALL.fun,d+1);
            for (;args; args=args->tail) {
                printf(",\n"); pr_tree_exp(out, args->head,d+2);
            }
            printf( ")");
            break;
        }
    } /* end of switch */
}

void printStmList ( F_fragList stmList)
{
    printf("↓---------------IR TREE PRINTF START---------------↓\n");
    if(out==nullptr)
    {
        printf("File open false");
    }
    for (; stmList; stmList=stmList->tail) {
        pr_stm(out, stmList->head->u.proc.body,0);
    }
    printf("\n↑---------------IR TREE PRINTF OVER---------------↑\n");
}
void printcannoList ( T_stmList stmList)
{
    printf("↓---------------IR TREE AFTER CANNO PRINTF START---------------↓\n");
    if(out==nullptr)
    {
        printf("File open false");
    }
    for (; stmList; stmList=stmList->tail) {
        pr_stm(out, stmList->head,0);
        printf("\n----dived line----\n");
    }
    printf("\n↑---------------IR TREE AFTER CANNO PRINTF OVER---------------↑\n");
}