# SeaShell  

A linux shell implementation written in C as part of an assignment during my Operating Systems (CSE-231) course at IIITD (Monsoon 2022).

To compile the project run `make` inside the project root. You need to have `make` installed on your OS.

To test and run the shell, execute the `shell` binary that is created inside the project root after `make` finishes compiling the project:
```
$ ./shell
```
You should see a welcome message and a prompt asking you to enter a command:
```
Welcome to SeaShell!
>> 
```

All external commands are suffixed with 'ss', so to run any external command, for example `mkdir`, run `mkdirss ...` instead.

SeaShell supports execution using threads instead of forking a new child process. To run a command in a thread, enter the command you wish to execute with an `&t` at the end:
```
mkdirss test &t
```

For testing symbolic links, first create a symbolic link:
```
$ ln -s /home testlink
```
This will create a symlink from `testlink` to `/home`. Now, you can test the -L and -P flags in the `cd` and `pwd` commands.


SeaShell supports the following 8 commands. Two command line options for each have been implemented.

Internal:

* [cd](#cd)
* [pwd](#pwd)
* [echo](#echo)

External:

* [ls](#ls)
* [cat](#cat)
* [rm](#rm)
* [mkdir](#mkdir)
* [date](#date)


## Commands

This section describes each command in detail: the syntax, arguments supported and any assumptions made.

### <a name="cd"></a>cd
`cd [-L | -P] [dir]`

Change the shell working directory.

Change the current directory to DIR.  The default DIR is the value of the HOME shell variable.

Options:

      -L        force symbolic links to be followed
      -P        use the physical directory structure without following
                symbolic links

The default is to follow symbolic links, as if `-L` were specified.

`..` is processed by removing the immediately previous pathname component
back to a slash or the beginning of DIR.

Edge cases:

- Trying to cd into a directory that does not exist gives a 'file does not exist' error.
- Trying to cd into a file gives a 'Not a directory' error.
- Doing a `cd ..` from `\` has no effect.

### <a name="pwd"></a>pwd
`pwd [-LP]`

Print the name of the current working directory.

Options:

      -L        print the value of $PWD, following symbolic links
      -P        print the physical directory, without any symbolic links

By default, `pwd` behaves as if `-L` were specified.

Edge cases:

- Giving an argument to the command prints a 'Too many arguments' error.
- When both options are set in the same command, `-P` overrides `-L`. 


### <a name="echo"></a>echo
`echo [OPTION]... [STRING]...`

Display a line of text

Options:

      -n        do not output the trailing newline
      -e        enable interpretation of backslash escapes

      If -e is in effect, the following sequences are recognized:

       \\     backslash
       \a     alert (BEL)
       \b     backspace
       \e     escape
       \f     form feed
       \n     new line
       \r     carriage return
       \t     horizontal tab
       \v     vertical tab

Backslash interpretations are disabled by default.

Edge cases:

- Without an argument, it just exits.
- Handle backslash escapes without the need of quotes.

### <a name="ls"></a>ls
`lsss [OPTION]... [FILE]...`

List information about the FILEs (the current directory by default).

Options:

      -a        do not ignore entries starting with .
      -l        use a long listing format

By default, all entries are listed in directory order.

Edge cases:

- Running the command on a non-existing file prints the 'No such file or directory' error.
- For symlinks, contents of the directory they are pointing to are printed.

### <a name="cat"></a>cat
`catss [OPTION]... [FILE]...`

Concatenate FILE(s) to standard output.

Options:

      -E        display $ at end of each line
      -n        number all output lines

Edge cases:

- Running the command on a non-existing file prints the 'No such file or directory' error.
- Multiple files are printed in the order in which they are listed in the command.
- When no files are specified, shell prints the 'too few arguments' error.

### <a name="rm"></a>rm
`rmss [OPTION]... [FILE]...`

Remove (unlink) the FILE(s).

Options:

      -f        ignore nonexistent files and arguments, never prompt
      -r        remove directories and their contents recursively

By default, `rm` does not remove directories.  Use the `-r` option to remove each listed directory, too, along with all of its contents.

Edge cases:

- Attempting to delete a directory without the `-r` flag gives the 'Is a directory' error.
- Deleting an already deleted file in the same command gracefully prints a 'No such file or directory' error.

### <a name="mkdir"></a>mkdir
`mkdirss [OPTION]... DIRECTORY...`

Create the DIRECTORY(ies), if they do not already exist.

Options:

      -p        make parent directories as needed
      -v        print a message for each created directory

Edge cases:

- Creating an already existing directory gives a 'file exists' error.
- Creating nested directories when not existing gives a 'No such file or directory' error without the `-p` flag.

### <a name="date"></a>date
`datess [OPTION]... [FORMAT]`

Display date and time in the given FORMAT.

Options:

      -R        output date and time in RFC 5322 format.
                Example: Mon, 14 Aug 2006 02:34:56 -0600
      -u        print the Coordinated Universal Time (UTC)

Edge cases:

- When both options are set in the same command, `-u` overrides `-R`.
- Entered format when a flag is set is ignored.


## Test cases

#### # Test case 1
Command: `>> cd ..` 

Expected output: the pwd should now point to one folder back.


#### # Test case 2
Command: `>> pwd`

Expected output: the current working directory.

#### # Test case 3
Command: `>> echo -e hello\nworld!`

Expected output: 
```
hello
world!
```

#### # Test case 4
Command: `>> lsss -l`

Expected output: 
```
-rwxr-xr-x 1 raman raman      26152 Oct 14 22:00 lsss
-rw-r--r-- 1 raman raman       3248 Oct 14 22:00 date.o
-rw-r--r-- 1 raman raman       4136 Oct 14 22:00 utils.o
-rw-r--r-- 1 raman raman        710 Oct 14 20:23 Makefile
-rwxr-xr-x 1 raman raman      21792 Oct 14 22:00 catss
-rw-r--r-- 1 raman raman       2076 Oct 14 21:02 utils.c
-rwxr-xr-x 1 raman raman      21784 Oct 14 22:00 datess
-rw-r--r-- 1 raman raman       3800 Oct 14 22:00 rm.o
-rwxr-xr-x 1 raman raman      21696 Oct 14 22:00 mkdirss
-rw-r--r-- 1 raman raman       5047 Oct 14 21:13 ls.c
-rw-r--r-- 1 raman raman       1463 Oct 14 20:14 date.c
-rw-r--r-- 1 raman raman       1295 Oct 14 21:05 cat.c
-rwxr-xr-x 1 raman raman      32032 Oct 14 22:00 shell
-rw-r--r-- 1 raman raman       5911 Oct 14 22:46 README.md
-rw-r--r-- 1 raman raman       6328 Oct 14 22:00 ls.o
-rw-r--r-- 1 raman raman        705 Oct 14 21:18 utils.h
-rw-r--r-- 1 raman raman       3360 Oct 14 22:00 mkdir.o
-rw-r--r-- 1 raman raman       3104 Oct 14 22:00 cat.o
-rw-r--r-- 1 raman raman       1513 Oct 13 18:00 mkdir.c
-rwxr-xr-x 1 raman raman      21928 Oct 14 22:00 rmss
-rw-r--r-- 1 raman raman       6946 Oct 14 21:46 shell.c
-rw-r--r-- 1 raman raman       1674 Oct 14 21:14 rm.c
```

#### # Test case 5
Command: `>> catss -n utils.h`

Expected output: 
```
    1  const int dateTimeLength = 100;
    2  const int filePathLength = 100;
    3  const int longStringLength = 1000;
    4  const int wordLength = 100;
    5  
    6  char** tokeniseString(char* string, int *args, char token); // split string using 
specified delimiter
    7  
    8  char **getFilesArray(); // malloc memory for files array
    9  
   10  void extractArguments(char **files, char *argv[], int argc, char option1, char option2, 
int *setOption1, int *setOption2, int *numFiles); // extract arguments from argv
   11  
   12  void getAbsolutePath(char **files, int numFiles); // absolute path of file
   13  
   14  int isDir(char *path); // check if path is a directory
   15  
   16  int isLink(char *path); // check if path is a symlink
   17  
   18  int isFile(char *path); // check if path is a regular file
```

#### # Test case 6
Command: `>> mkdirss -p test/dir`

Expected output: a new directory `test/dir` should be created in the current working directory.

#### # Test case 7
Command: `>> rmss -r test`

Expected output: the directory `test` should be deleted with all of its contents.

#### # Test case 8
Command: `>> datess -u`

Expected output: prints the UTC time, for example: `Friday 14 October 2022 05:27:25 PM UTC`
