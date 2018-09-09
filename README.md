# Calculator for ZJU Project

* 小组成员及其分工

  * __赵梦雨__负责 `Prompt()` ，`CheckAssociation()` 部分的编写，并参与程序 bug 的寻找.
  * __凌添翼__负责部分 `stack.h` 的编写，并参与程序 bug 的寻找，参与部分实验报告的编写.
  * __陈佳伟__负责程序和实验报告的剩余部分的编写以及 bug 的修复.

* 本程序支持的功能

  * 支持基本的四则运算(`+`, `-`, `*`, `/`)，输入的数字可以是负数、小数(数字前面带有正号本程序也能理解).
  * 支持阶乘和乘方运算并且让用户以常见形式输入(如 `3!` `5^2`)，而不是以函数形式输入(`fact()`, `pow()`)，更符合数学表达习惯.
  * 支持基本的一元函数：`sin()`, `cos()`,  `tan()`, ` arcsin()`, ` arccos()`, ` arctan()`, ` lg()`, `ln()`, `exp()`,  `abs()`,  `sqrt()`；而且当参数仅为 一个数时可以不用加括号(如 `lg2` `sin1` `exp2`)，更符合数学表达习惯
  * 支持多元函数，本程序以对数函数为例，(如 `log(底数, 真数)`)，其他的多元函数可以类似地实现.
  * 支持常量的输入，当用户输入 `e` 时程序理解为输入 `2.718`；当用户输入`PI` 或 `Pi` 或 `pi` 时，程序理解为输入的是 $\pi$ : `3.14159`
  * 算式中操作符与操作数之间输入的 `SPACE` 和 `TAB` 会被忽视.

* 程序的模块化结构框架

  该程序调用了C99标准库、教材库、以及自己编写的 `stack.h` 、 `queue.h` 以及自己略加修改的数学函数库 `mathematic.h`，分别用以实现队列、堆栈、函数边界值判断的操作.

  `main()`分为三个函数

  ```c
  void Prompt(void);
  void ConvertToPostfix(void);
  void Process(void);
  ```

  其中`ConnvertToPostfix()`除了基本代码外还包含了

  ```c
  bool CheckAssociation(char *);
  int CompareAssociationPriority(char, char);
  ```

  而`Process()`除了基本的代码外包括了

  ```c
  double CallFunction(char *, struct stack_node **);
  void Output(char *);
  ```

* 各主要模块(函数)的功能及其算法

  * `Prompt()`函数的功能：输出本程序所需要的注意事项、提示信息。

  * `ConvertToPostfix()`的功能：把用户输入的表达式(中缀表达式)转化成后缀表达式(逆波兰表达式)。

    ​	把中缀表达式转化成后缀表达式采用现成的算法——调度场算法(Shutting Yard Algorithm)，该算法的具体细节见附录A.

    ​	该算法在 C 语言下的具体实现时的主要难点如下：

    * 如何判断读取的是函数还是数字还是运算符？

      ​	不断用`getchar()`读取输入流中的字符并存在分别存储在一块内存中，

      ​	__数__：如果读入的字符是数字或是小数点 `.` 那么就判定是数字，直到读到非数字且非小数点的字符，并用`ungetc()`把字符退回输入流中.

      ​	__函数__：如果读入的是字母，先判断是否是常数 `e` 或者 `pi`，如果不是那么判断为函数名，这与读入数字类似.

      ​	__运算符__：如果读入的是 `+`, `-`, `*`, `/`, `^` 运算符，假如读入的是 `+`, `-`，那么先判断这个符号表达的是减号(加号)还是负号(正号)。判断方法是：如果上一个读取的是 `(` 或者没有读取(意味着这是算式的开头)那么判断这个符号代表的是负号(正号)，否则认为是减号(加号)。对于负号，则存入 `-1` 到输出队列中，并把一个乘号 push 入堆栈(这相当于把负号理解为 `(-1) *`)；如果是正号则不进行操作.

    * 输出队列和堆栈如何存储运算数和运算符？

       输出队列和堆栈存储的 `char * ` 类型的指针，指向对应操作数或操作符或函数名的内存.

* `Process()`的功能：读取后缀表达式并调用相应函数进行计算并输出答案。

    ​	读取后缀表达式的算法见附录B.

    ​	在读取后缀表达式时，调用`CallFuntion()`将函数名(或操作符名)传入并调用对应函数

* `stack.h`中定义了结构：

    ```c
    struct stack_node {
        struct stack_node *next;
        STACKTYPE value;
    };
    ```

    并且定义了函数：

    ```c
    void NewStack(struct stack_node **rootP);
    STACKTYPE Pop(struct stack_node **rootP); 
    void Push(struct stack_node **rootP, STACKTYPE input);
    STACKTYPE Top(struct stack_node **rootP);
    int StackSize(struct stack_node *root);
    void DestroyStack(struct stack_node **rootP);
    ```

    其中 `STACKTYPE` 由 `#define `进行声明来具体确定 value 的类型.

    `stack.h` 中的堆栈由链表来实现，除了 `StackSize()` 函数外传入的时指向链表根指针(root pointer)的指针.

* `mathematic.h` 中定义了函数：

    ```c
    double factorial(double n);
    double Abs(double x);
    double LOG(double base, double antilog);
    double SQRT(double x);
    double LG(double antilog);
    double LN(double antilog);
    ```

    其中 `Abs()` 函数用来克服标准库中绝对值函数无法输入浮点数的不足；其余函数都是为了在边界值问题上发出 `Error()` 提示。

* 设计、编程过程中碰到的主要问题及解决思路

  * 在利用链表实现堆栈的时候发现根本不起作用，会莫名其妙出错.

    解决：把参数改成指针的指针，这样就能够更改指针的值.

  * 如何读入含有小数部分的数字？

    解决：把数字当成是字符串读入.

  * 如何识别并处理正号和负号？

    解决：当读入的前一个字符是 `(` 或者前面没有读字符的时候认为是减号和加号，遇到减号和加号把它看作是 `(-1) *` 和 `1 *`.

  * 如何进行代码修改时的版本控制？

    解决：使用 Github 代码托管平台进行版本控制。

* 实验心得体会

    (此部分由__凌添翼__执笔)

    ​	这次 C 程 project 作业，我们小组付出了非常多的心血。我们团队成员，特别是程序主要编写人陈佳伟同学，不论是在前期的选课题阶段的深度思考和对逆波兰表示法、调度场算法、逆波兰式转换等核心算法的学习，还是在编写程序中日产数百行代码、绞尽脑汁优化程序，目睹程序一次次崩溃又一次次尝试修复的艰辛，亦或是后期我们齐心协力 debug，黑色的命令提示窗口犹如一个黑洞，有时仿佛真的能让人有一种要被其所吞噬的绝望感。幸运的是，我们终于成功走了过来，并用自己的智慧创造出了第一个属于我们团队自身的 project。

    ​	与其他大部分小组组队速度缓慢、程序开发阻滞不前不同，周二上完课后的没过两天，我们就快速组好了自己的队伍，经过充分讨论后确定了计算器课题。同时，我们明确了自己的小组分工。我们不只是满足于与完成 + - * / 的 level 1 级别的简单操作，我们主动要求“加戏”，决定直接跨过要求的level 2 进入更高级别的level 3：不光能实现 + - * / sin cos !，还能实现对数函数，反三角函数，e/π的自动识别等更实用且适用的功能。完成前期规划后，我们立即马不停蹄地开始进行程序开发与 debug工作。但是，功能的增多意味着bug出现概率几何级数的增长，因此，之后一段时间我们可以说是为了 debug 耗尽心力，只是希望能够让我们的程序更优秀一点、更与众不同一点。

    ​	最后我想说，在整个project开发过程中，非常感谢佳伟组长和赵赵一路的提携与帮助，也非常庆幸能够成为这个小团队的一员，感受并肩作战的快感，感受我和大佬之间深深的差距。希望我们团队能在之后新的project 开发中继续精诚合作，收获更多。

* 附录A

  Shutting Yard Algorithm:

  > - 当还有记号可以读取时：
  >   - 读取一个记号。
  >   - 如果这个记号表示一个数字，那么将其添加到输出队列中。
  >   - 如果这个记号表示一个函数，那么将其压入栈当中。
  >   - 如果这个记号表示一个函数参数的分隔符（例如，一个半角逗号 `,` ）：
  >     - 从栈当中不断地弹出操作符并且放入输出队列中去，直到栈顶部的元素为一个左括号为止。如果一直没有遇到左括号，那么要么是分隔符放错了位置，要么是括号不匹配。
  >
  >
  > ​
  >
  > - 当再没有记号可以读取时：
  >
  >   - 如果这个记号表示一个操作符，记做o1，那么：
  >
  >     - 只要存在另一个记为o2的操作符位于栈的顶端，并且
  >
  >       ​	如果o1是左结合性的并且它的运算符优先级要小于或者等于o2的优先级，或者
  >
  >       ​	如果o1是右结合性的并且它的运算符优先级比o2的要低，那么
  >
  >       将o2从栈的顶端弹出并且放入输出队列中（循环直至以上条件不满足为止）；	
  >
  >     - 然后，将o1压入栈的顶端。
  >
  >
  > ​
  >
  > - 如果这个记号是一个`(`，那么就将其压入栈当中。
  >   - 如果这个记号是一个`)`，那么：
  >     - 从栈当中不断地弹出操作符并且放入输出队列中，直到栈顶部的元素为`(`为止。
  >     - 将`(`从栈的顶端弹出，但并不放入输出队列中去。
  >     - 如果此时位于栈顶端的记号表示一个函数，那么将其弹出并放入输出队列中去。
  >     - 如果在找到一个`(`之前栈就已经弹出了所有元素，那么就表示在表达式中存在不匹配的括号。
  >   - 当再没有记号可以读取时：
  >     - 如果此时在栈当中还有操作符：
  >       - 如果此时位于栈顶端的操作符是一个括号，那么就表示在表达式中存在不匹配的括号。
  >       - 将操作符逐个弹出并放入输出队列中。
  >
  >
  > - 退出算法。

  ​                                                                      ——引自 Wikipedia "Shutting Yard Algorithm"

* 附录B

  Reverse Polish notation:

  > - while有输入符号
  >   - 读入下一个符号X
  >   - IF X是一个操作数
  >     - 入栈
  >   - ELSE IF X是一个操作符
  >     - 有一个先验的表格给出该操作符需要n个参数
  >     - IF堆栈中少于n个操作数
  >       - **（错误）** 用户没有输入足够的操作数
  >     - Else，n个操作数出栈
  >     - 计算操作符。
  >     - 将计算所得的值入栈
  > - IF栈内只有一个值
  >   - 这个值就是整个计算式的结果
  > - ELSE多于一个值
  >   - **（错误）** 用户输入了多余的操作数

  ​									——引自 Wikipedia "Reverse Polish notation"