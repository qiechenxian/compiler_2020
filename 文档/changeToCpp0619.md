## SysY.y

1. U_reverseList的void*全部加类型转换，例如：(A_fieldList)U_reverseList($4)
2. itoa函数改为sprintf，前者只能在windows环境使用
3. 生成tab.h后加include<absyn.h>

## 多个文件加类型强制转换

1. 例如table.cpp中s = (S_symbol )TAB_pop((TAB_table) t);

## table.cpp

1. 三处(unsigned)key改为long long，否则编译不过，损失精度

## ast.cpp

1. 所有构造函数的定义加强制类型转换

2. 所有kind加作用域，例：a->kind = A_stm_::A_ifStm;

## translate.cpp tree.cpp sement.cpp

1. 同上

## lex.yy.cpp

1. 进制转换使用库函数strtol()
  