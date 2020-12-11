# miniC

## Directory Structure
- `antlr_miniC` folder consists of the miniC language parser in the file `miniC.g4`.
- `sample_programs` folder consists of some positive and negative sample programs for the miniC language with the planned syntax and semantics.
- `Language Manual miniC.pdf` consists of the description of the miniC language.


## To Run
- Go into the folder `antlr_miniC` and do `make`.
- Do `./miniC <path of source code>`.
- `clang-11 ir_llvm.ll`
- `./a.out`

<hr>

# Expression Parsing
## Directory Structure
- `antlr` folder consists of the expression parser in the file `Expr.g4`.
- `tests` folder consists of the test files for the aforementioned expression parser.

## To Run expression parsing

```bash
$ cd antlr
$ make 
```
General command:
```bash
$ grun <name_of_grammar> <root_node/Start Symbol> <path_to_test_file> <option>
```

Command here:
```bash
$ grun Expr prog ../tests/<test_file> -gui
```









<!--
## Installation

### Java:
You can refer to Installation guide [here](https://www3.ntu.edu.sg/home/ehchua/programming/howto/JDK_Howto.html).

To check if Java is installed:

```bash
$ java -version
$ javac -version
```


### Antlr:
Refer to the [antlr](https://www.antlr.org/) main website.You can find how to install on Linux, OS X, Windows.

*PS: Make sure you put following lines in your source file(.bashrc, .zshrc etc.,)*

```bash
export CLASSPATH=".:/usr/local/lib/antlr-4.8-complete.jar:$CLASSPATH"
alias antlr4='java -jar /usr/local/lib/antlr-4.8-complete.jar'
alias grun='java org.antlr.v4.gui.TestRig'
```
```bash
$ source ~/.bashrc
```

## Usage
-->
