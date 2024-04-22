Utility I built to avoid having to clean up ESlint and pylint warnings about trailing white spaces. Creates a new file with _nwts appended to the file name which has no trailing white spaces. Has the option to flag the range of lines to make changes to.

Eg. running `$ ./rm_tws remove_trailing_white_spaces.c` will result in a **remove_trailing_white_spaces_ntws.c** file with no trailing white spaces.

Flags
1. --help: will display all the flag options
1. -l/--lines : followed by the range of lines in between (inclusive of the numbers) where the check for white space will be done. Eg. `$ ./rm_tws remove_trailing_white_spaces.c -l 1 10 100 200` removes the trailing white spaces between and including lines **1-10** and **100-200**. Exits program if odd number of range values is provided. If flag is not present, the all the lines are checked for trailing white spaces.
1. -v/--verbose : utility will print out the line numbers and the number of trailing white spaces found on said line number when flag is present 
1. -s/--show : utility will not make the new file but only shows/print out the trailing white spaces in the appropriate range with **@**. 

