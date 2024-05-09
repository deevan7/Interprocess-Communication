**README Interprocess communication**
Deevankumar Gaddala


**Description**

This C program sets up a basic pipeline between a "generator" process and a "consumer" process. It takes command-line arguments to specify the generator and consumer commands, then creates a pipe to connect the output of the generator to the input of the consumer.

**Usage**

To compile the program, use a C compiler such as gcc:
`gcc my_ipc.c -o my_ipc`
 To run
`./my_ipc -g generator_command -c consumer_command`

Replace generator_command with the command you want to use as the generator.
Replace consumer_command with the command you want to use as the consumer.

**Return Codes**

`BAD_ARGNUM (255):` Indicates an incorrect number of command-line arguments. The program expects exactly 5 arguments.
`INVALID_SWITCH (254):` Indicates an invalid switch used in the command-line arguments.
`MISSING_CMD (253):` Indicates either the generator or consumer command was not provided.


sample Command to run
`./my_ipc -g "ls -l" -c "wc -l"`


**Overview**
The program sets up a pipeline between two processes: a "generator" and a "consumer".
It checks for the correct number of command-line arguments.
Parses the command-line arguments to get the generator and consumer commands.
Creates a pipe with pipe() to connect the generator's output to the consumer's input.
Forks two child processes (cpid and cpid2):
	`cpid redirects its output to the pipe's write end and executes the generator command.`
	`cpid2 redirects its input from the pipe's read end and executes the consumer command.`
The parent process closes pipe ends and waits for both children to finish with waitpid().
