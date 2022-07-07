# C-search
A command that search files and directories providing more infos

Run activate.sh to add the command to your system.

How to use it :

search [Directory_Name] [-options] 'File_Name'

    filename should preferably be between ' ' or " " especially when using * and ?
    
    [-options]:
      -0: search in directory "directory name" only
      -1: traverse only one level of the tree
      -n: traverse n levels of the tree
        no levels -> search in all levels

      -d: display the creation date and the date of the last use (consultation or modification) of the file
      -m: display the date of the last modification of the file
      -s: show file size
      -t: show file type
      -p: show file protection
      -a : display all characteristics (d,s,t,p)
      -h: display this manual

