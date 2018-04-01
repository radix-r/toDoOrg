# toDoOrg

compile: gcc -o org toDoOrg.c

run: ./org <my todo list name> 

**or** if you want to add to your list throughout the day and have it orginize every hour

run: ./orgScript.bash 

I'm pretty sure this only work with Linux. This is best used in conjunction with Dropbox of some other file sharing scheme. 

## Basic format of to-do list:

Lines that start with the  '-' character are considered the header. They will be brought to the top of the file. Lines that start with the '(' character are considered done and will go to the bottom of the file. I usually mark finished items with (X) or (×) to indicate that they were successfully competed. Failed items I usually mark with (:C) or (:c). All other lines are the active to-do list. If a line has a date in the format <month>/<day> (ex: 3/31) it is given a weight in the form 32*<month>+<day>. Earlier dates will appear higher on the list i.e 9/20 will appear above 10/2. Lines without a date are by default given a large weight(MAX_INT-1) and will appear below lines with dates. Also all empty lines ('\r') will be put between the to-do section and the done section.

## Example
```
-To Do Fall 2017 - Spring 2018:
-------------------------------
-----priorities-------
-physics 
-systems software 
-computer science 2
-statistics
-----TO-DO-----------------
Get gud
ur mom 2/15
(X)sell gf
get a life
```

becomes

```
>-To Do Fall 2017 - Spring 2018:
>-------------------------------
>-----priorities-------
>-physics 
>-systems software 
>-computer science 2
>-statistics
>-----TO-DO-----------------
>ur mom 2/15
>Get gud
>get a life
>(X)sell gf
```
    

