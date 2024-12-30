Compile And Debug: Shift + F5

## 思路

1. 输入的时候、直接检查这个语句的语法树对不对，然后尝试构建语法树、如果构建失败就报错（除了GOTO）

   - 在输入的时候就检查指令是否合法，不合法需要报错并忽略该指令，并继续读取其他指令。这里所说的检查指令是指检查指令是否符合语法要求，对于类似GOTO的行号不存在这种问题，在输入指令时无法检查是否存在该问题，因此在运行时才需要对这种错误报错。

2. LOAD 指令，从文件中读取命令，即和输入时的操作一样就行了，即getline、然后每行检查、如果有语句的语法树不对、就报错、除了GOTO

3. INPUT 的时候，用户只能输入一个数字，如果输入的不是数字，就报错

4. 输入指令LIST时，直接忽略，不用报错。

5. CLEAR指令除了清理界面，还需要清除程序中的所有状态。

6. INPUT命令要显示的"?"是显示在命令输入窗口中的。"?" 类似于shell的提示符，用户不需要删除，直接在后面输入即可。

7. 对于那些在命令输入窗口中，直接使用不带行号的INPUT或LET语句定义的变量，其他语句能不能使用这些变量？

   - 代码框中的代码，语法分析树和调试器不会受这些变量影响，也不需要在运行时考虑这些变量。但不带行号的PRINT语句则需要考虑这些变量的值。也就是说，带行号的语句定义的变量和不带行号的变量应该完全分成两套体系来管理。
   - 那么其实就相当于这两个的处理逻辑不一样、但是可以复用实现、这里可以先实现输入进文本框的语句、再考虑不带行号的语句

8. LOAD 文件里面都是 行号+statement 的形式，不包括上述情况。

9. GOTO 后面是常量，表示行号，如果不是数字，就报错

10. 文件中的命令会存在不按行号顺序排列的情况，即还是和输入时一样、需要插入命令到指定的位置

11. LOAD文件需要清空原先代码

12. 变量名需要遵守C/C++的要求，即不能是关键字（比如IF），也不能以数字开头。例如1a和IF这样的变量名是非法的。

13. 指令内有多余的空格或者缺少空格都是合法的。比如100     LET          X1=X2+        X3  是合法的

14. 指令内不会出现tab字符，只会出现空格

15. 如果删除了不存在的行号，忽略即可

16. 行号需要为不超过1000000的正整数，否则提示用户行号错误

17. 负号和数字之间可以有空格

18. INPUT语句的测试输入中不含有浮点数