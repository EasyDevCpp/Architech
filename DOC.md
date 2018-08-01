## Architech - Documentation

> The elements written in **bold** are necessary while the elements written in *italic* are optional.

### Table of contents:

* [Keywords](#keywords)
  * [var](#var)
  * [func](#func)
  * [as](#as)
  * [ptr](#ptr)
  * [ref](#ref)
  * [ret](#ret)
  * [end](#end)
* [Basic Libary](#basic-libary)
  * [print](#print)
  * [println](#println)
  * [read](#read)
  * [readln](#readln)

---

### Keywords

-------

#### var

> **What it does:** 
>
> - The var-keyword creates & initializes a variable.
>
> **Syntax:**
>
> 	- **var $name** *= value*
>
> **Remarks:**
>
> - A variable can't be create in the global scope.
>
> - In Architech variables are identified by the use of the **'$'** sign.
>
> - To define a variable with a specific-type use this syntax:
>
> 	- **var $name as type** *ptr = value*

________

#### func

> **What it does:**
>
> - The func-keyword creates a function or method based on the scope you use it in.
>
> **Syntax:**
>
> 	- **func name** *as type ptr***(** *$param1*, *...*, *$param_n***)**
>
> **Remarks:**
>
> - If you define a function with a specific-type you have to **return** a value of the type.
>
> - Each parameter can have a specific-type to, just apply the **as-syntax**.

_____________

#### as

> **What it does:**
>
> - The as-keyword is used to *force* a specific-type. You can also use it for pointer creation.
>
> **Syntax:**
>
> 	- **as type** *ptr*

> **Remarks:**
>
> - This keyword also changes the **var-syntax** in some cases. For example if you create an object from a certain class the syntax might change to the following:
>
> 		- **var $name as class** *ptr* **= ()** #Calls the constructor of the used class+

__________

#### ptr

> **What it does:**
>
> - The ptr-keyword creates a pointer value.
>
> **Remarks:**
>
> - This keyword can only be used in combination with the **as-keyword**!

____________

#### ref

> **What it does:**
>
> - The ref-keyword creates a refrence of a variable.
>
> **Syntax:**
>
>  - *ret* **$name ref**
>
> **Remarks:**
>
> - A refrence can be used to convert a variable to the pointer-type.

____

#### ret

> **What it does:**
>
> - The ret-keyword is used to return a certain value out of a function or method.
> - The returned type has to match up with the function-type
>
> **Syntax:**
>
> - **ret** *value*
>
> **Remarks:**
>
> - You can just use **ret** to return the default value of the used type.

__________

#### end

> **What it does:**
>
> - Some keywords need a closing tag. To close those just use **end** at the end.
>
> **Syntax:**
>
> ```assembly
> func ...()
> 	...
> end
> ```

_____

### Basic Libary

______

#### print

> **What it does:**
>
> - prints a given text(Terminal only!)
>
> **Syntax:**
>
> - **print(var $text)**
>
> **Remarks:**
>
> - Returns void

____

#### println

> **What it does:**
>
> - prints a given text and adds a newline(Terminal only!)
>
> **Syntax:**
>
> - **println(var $text)**
>
> **Remarks:**
>
> - Returns void

____

#### read

> **What it does:**
>
> - reads one word and returns it(Terminal only!)
>
> **Syntax:**
>
> - **read()**
>
> **Remarks:**
>
> - returns the value as var

____

#### readln

> **What it does:**
>
> - reads one line and returns it(Terminal only!)
>
> **Syntax:**
>
> - **readln()**
>
> **Remarks:**
>
> - returns the value as var

____

