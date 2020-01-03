# Интерпретатор модельного языка DL
## Синтаксис языка в форме Бэкуса-Наура
* \<integer\> - целое число
* \<id\> - идентификатор
* \<expression> ::= \<val\> | \<var\> | \<add\> | \<if\> | \<let\> | \<function\> | \<call\>
* \<val\> ::= (val \<integer\>)
* \<var\> ::= (var \<id\>)
* \<add\> ::= (add \<expression\> \<expression\>)
* \<if\> ::= (if \<expression\> \<expression\> then \<expression\> else \<expression\>)
* \<let\> ::= (let \<id\> = \<expression\> in \<expression\>)
* \<function\> ::= (function \<id\> \<expression\>)
* \<call\> ::= (call \<expression\> \<expression\>)
* \<set\>::= (set \<id\> \<expression\>)
* \<block\> ::= =(block \<expression\>\+)
## Семантика
Пусть Eval - функция выполнения инструкции. 

Scope - область видимости, коллекция пар (\<id\>, \<expression\>), так же может иметь родительский Scope.

ScopeStack - стэк областей видимости.

CurrentScope - текущая область видимости, вершина ScopeStack.

GetVar(\<id\>) - функция получения значения из CurrentScope или его родителей.

SetVar(\<id\>, \<expression\>) - установка значения в Scope, ошибка, если id не найдено в текущем Scope и его родителях.

CreateVar(\<id\>, \<expression\>) - создание значения в CurrentScope.

CreateScope(parent = Scope) - создание и добавление на вершину стэка нового Scope с parent в качестве родительского (по умолчанию текущий Scope).

DeleteScope() - удаление CurrentScope с вершины стэка.

GetValue(<expression>) - возвращает значение, если вызвана от <val>, иначе вызывает ошибку

Тогда
+ Eval(\<val i\>) = \<val i\>
+ Eval(\<var id\>) = GetVar(id)
+ Eval(\<add e1 e2\>) = GetValue(Eval(e1)) + GetValue(Eval(e2))
+ Eval(\<if e1 e2 e_then e_else) = { GetValue(Eval(e1)) <= GetValue(Eval(e2)) -> Eval(e_then), иначе Eval(e_else) }
+ Eval(\<let id value body\> = CreateScope(CurrentScope), CreateVar(id, Eval(value)), value = Eval(body), DeleteScopen(), value
+ Eval(\<set id value\>) = SetVar(id, Eval(value)), value
+ Eval(\<function id expression\>) = \<closure id expression CurrentScope\>
+ Eval(\<closure id expression scope\>) = \<closure id expression scope\>
+ Eval(\<call func arg\>) = Если Eval(func) = \<closure id expression scope\> то CreateScope(scope), CreateVar(id, Eval(arg)), value = Eval(expression), DeleteScope(), value. Если Eval(func) не \<closure\>, то ошибка.
+ Eval(\<block expressions\>) = (∀expression ∈ expressions | value = Eval(expression)), value

## Немного о реализации
Интерпретатор состоит из трёх основных этапов-компонентов: лексического анализатора, синтаксического анализатора и собственно исполнителя.

Лексический анализатор, реализованный как конечный автомат, читает входной поток символов и разбивает его на лексемы (токены). 

Синтаксический анализатор, реализованный с помощью алгоритма рекурсивного спуска, читает поток лексем и строит по нему абстрактное синтаксическое дерево (AST), которое является промежуточным представлением для данного интерпретатора.

Исполнитель принимает на вход AST, полученное от синтаксического анализатора и исполняет программу, рекурсивно спускаясь по её AST, путём применения правил, описанных в разделе семантика.
