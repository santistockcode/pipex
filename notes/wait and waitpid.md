# Bash Redirection and Pipelines

In Bash, the symbols `<`, `>`, and `|` are used for redirection and pipelines, allowing you to control input and output streams of commands.

## Input Redirection

The `<` symbol is used for input redirection. It allows you to redirect the input of a command from a file. For example:

```bash
$ command < input.txt
```

This will execute `command` with the contents of `input.txt` as its input.

## Output Redirection

The `>` symbol is used for output redirection. It allows you to redirect the output of a command to a file. For example:

```bash
$ command > output.txt
```

This will execute `command` and save its output to `output.txt`.

## Appending Output

If you want to append the output of a command to a file instead of overwriting it, you can use the `>>` symbol. For example:

```bash
$ command >> output.txt
```

This will append the output of `command` to the end of `output.txt`.

## Pipelines

The `|` symbol is used for creating pipelines. It allows you to chain multiple commands together, where the output of one command becomes the input of the next. For example:

```bash
$ command1 | command2
```

This will execute `command1` and pass its output as the input to `command2`.

Pipelines are a powerful feature of Bash, enabling you to combine commands and perform complex operations.

## Examples about pipex < archivo1 comando1 | comando2 > archivo2

< archivo1: This part of the command uses the input redirection operator < to take the contents of archivo1 as input for the subsequent command.

comando1: This is the first command in the pipeline. It will receive the contents of archivo1 as input.

|: The pipe operator | is used to connect the output of comando1 to the input of comando2. It allows the output of one command to be used as the input for another command.

comando2: This is the second command in the pipeline. It will receive the output of comando1 as input.

> archivo2: This part of the command uses the output redirection operator > to redirect the output of comando2 to archivo2. The contents produced by comando2 will be written to archivo2.

```
sort < names.txt
< names.txt sort
< names.txt cat | sort
```
This three commands are equivalent. They all read the contents of names.txt and sort them alphabetically. The output is displayed on the terminal in the first two commands, while the third command redirects the sorted output to a file named names_sorted.txt.

### Key Points
< names.txt redirects the contents of names.txt to standard input that waits until sorts take it
