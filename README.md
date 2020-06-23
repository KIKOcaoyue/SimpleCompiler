# SimpleCompiler
a simple compiler including lexical analysis, syntax analysis, and semantic analysis. 

README.docx为作者的课程设计报告，包含了该编译器的一些设计和实现方法。


附录说明：

init.txt是词法分析的初始化数据，包含了语言的关键字。

tbbs.txt是语言的文法。

Action.txt是语言的属性文法。

wordtest.txt是需要编译的源代码。

output.txt是词法分析的结果。

result.txt是语法分析和语义分析的结果。

checkError.cpp是具有一些检查错误功能的语法分析和语义分析器。

WORDANALYSE.cpp是词法分析器。



首先将需要编译的源代码写进wordtest.txt，运行WORDANALYSE.cpp进行词法分析。然后运行checkError.cpp进行语法分析和语义分析，结果输出在result.txt。
