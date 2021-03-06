//
// Created by loyx on 2020/5/8.
//

#ifndef COMPILER_LOYX_TREE_H
#define COMPILER_LOYX_TREE_H

#include "temp.h"

typedef struct T_stm_* T_stm;
typedef struct T_exp_* T_exp;
typedef struct T_expList_ *T_expList;
typedef struct T_stmList_ *T_stmList;
typedef enum {
    T_add, T_sub, T_mul, T_div,
     T_lshift, T_rshift, T_arshift,
    T_xor,T_mod,
} T_binOp;
typedef enum {
    T_lt, T_le, T_gt, T_ge, T_eq, T_ne,T_not,T_and, T_or
} T_relOp;


/** class */
struct T_stm_{
    enum {T_SEQ, T_LABEL, T_JUMP, T_CJUMP, T_MOVE, T_EXP, T_STMFD,T_LDMFD} kind;
    union {
        struct {T_stm left; T_stm right;} SEQ;
        Temp_label LABEL;
        struct {T_exp exp; Temp_labelList jumps;} JUMP;
        struct {T_relOp op; T_exp left, right; Temp_label trues, falses;}CJUMP;
        struct {T_exp dst, src;} MOVE;
        struct {T_expList exp_list;} STMFD;
        struct {T_expList exp_list;} LDMFD;
        T_exp EXP;
    }u;
};

struct T_exp_{
    enum {T_BINOP, T_MEM, T_TEMP, T_ESEQ, T_NAME, T_CONST, T_CALL,T_RELOP}kind;
    union {
        struct {T_binOp op; T_exp left, right;}BINOP;
        T_exp MEM;
        Temp_temp TEMP;
        struct {T_stm stm; T_exp exp;}ESEQ;
        Temp_label NAME;
        int CONST;
        struct {T_exp fun; T_expList args;}CALL;
    }u;
};

struct T_stmList_{T_stm head; T_stmList tail;};
struct T_expList_{T_exp head; T_expList tail;};


/**functions*/
T_stmList T_StmList(T_stm head, T_stmList tail);
T_expList T_ExpList(T_exp head, T_expList tail);

T_stm T_Seq(T_stm left, T_stm right);
T_stm T_Label(Temp_label label);
T_stm T_Jump(T_exp exp, Temp_labelList labels);
T_stm T_Cjump(T_relOp op, T_exp left, T_exp right, Temp_label, Temp_label);
T_stm T_Move(T_exp dst, T_exp src);
T_stm T_Exp(T_exp exp);
T_stm T_Stmfd(T_expList exp);
T_stm T_Ldmfd(T_expList exp);

T_exp T_Binop(T_binOp op, T_exp left, T_exp right);
T_exp T_Mem(T_exp exp);
T_exp T_Temp(Temp_temp temp);
T_exp T_Eseq(T_stm stm, T_exp exp);
T_exp T_Name(Temp_label label);
T_exp T_Const(int consti);
T_exp T_Call(T_exp fun, T_expList args);
T_relOp T_not_op(T_relOp op);
#endif //COMPILER_LOYX_TREE_H
