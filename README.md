# pipex

A project at School21. [Subject](pipex.pdf) has all the details.

In short, it's an implementation of a program `pipex` which simulates "|" behaviour in shell.

```bash
./pipex infile cmd1 cmd2 outfile
```
is equivalent to
```bash
< infile cmd1 | cmd2 > outfile
```
And
```bash
./pipex here_doc LIMIT cmd1 cmd2 outfile
```
is equivalent to
```bash
cmd1 << LIMIT | cmd2 > outfile
```
All code is written in correspondence with [Norm](https://github.com/42School/norminette) codestyle.
