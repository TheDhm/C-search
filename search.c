/* Created by MELEK Abderrahmane "TheDHM" */

char *MAN = "Search [Directory_Name] [-options] 'File_Name' \n"
"\n"
"filename should preferably be between ' ' or \" \" especially when using * and ?\n"
" [-options]:\n"
" -0: search in directory \"directory name\".\n only"
" -1: traverse only one level of the tree.\n"
" -n: traverse n levels of the tree.\n"
" no levels -> search in all levels\n"
"\n"
" -d: display the creation date and the date of the last use (consultation or modification) of the file.\n"
" -m: display the date of the last modification of the file.\n"
" -s: show file size.\n"
" -t: show file type.\n"
" -p: show file protection.\n"
" -a : display all characteristics (d,s,t,p).\n"
" -h: display this manual.\n";
            


#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fnmatch.h>
#include <linux/limits.h>
#include <time.h>
#include <sys/sysmacros.h>



/*------------------| Flags|----------------------*/

int a=0, d=0, m=0, s=0, t=0, p=0, all_lvl=0, lvl=-1;;

/*--------| first path and file name variables |---------*/
char filename[FILENAME_MAX], head_path[PATH_MAX];

 
/*------------------------------------| get_subdirs |---------------------------------------*/
/*---------------------- recursive function calls search_file -------------------------*/
int get_subdirs(const char *path ,const int lvl){
    
    if(lvl >= 0 || all_lvl == 1){ /* stop conditions */

        struct dirent* dent;
        DIR* srcdir = opendir(path);

        if (srcdir == NULL)
        {
        return 0;
        }
        search_file(path,filename); /*  call of search_file*/

        char tmp[1024]="";

        while((dent = readdir(srcdir)) != NULL)
        {
            struct stat st;

            if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
                continue;

            if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0)
            {
                return 1;
            }

            /* if file is directory */
            if (S_ISDIR(st.st_mode)){
                strcpy(tmp,path);
                
                
                if(strcmp("/",&tmp[strlen(tmp)-1]) != 0){
                strcat(tmp,"/");
                }
                strcat(tmp,dent->d_name);
                get_subdirs(tmp,lvl-1); /* recursive call with level-1*/
            }
        }
        closedir(srcdir);
    }
    
    return 0;
}

/*------------------------------------| SERACH FILE |------------------------------------*/
/*----------------|function that search a file in a directory |---------------*/
int search_file(char *path,char *filename){
    DIR *dp;
    struct dirent *dirp;
    int i;
    char tmp[1024]="";

    

if ((dp = opendir(path)) == NULL)
{
return 1;
}


while ((dirp = readdir(dp)) != NULL)
{       strcpy(tmp,path);

        if(fnmatch(filename,dirp->d_name,i) == 0){
             if(strcmp("/",&tmp[strlen(tmp)-1]) != 0){
                strcat(tmp,"/");
                }

            strcat(tmp,dirp->d_name);
            printf("\n--------------------------/ %s /--------------------------\n\npath : %s \n",dirp->d_name,tmp);
            struct stat st;
            stat(tmp,&st);
            aff_birthtime(st);
            aff_modtime(st);
            aff_acctime(st);
            aff_size(st);
            aff_type(st);
            aff_protec(st);
        }
}

closedir(dp);
    return 0;
}
/* -----------------------fonctions pour afficher les proprieté ------------------------------*/
int aff_type(struct stat st){
if(a || t){
printf("type de fichier: ");

           switch (st.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }
}
}

int aff_size(struct stat st){
if(a || s){
    printf("taille : %ld octets \n",st.st_size);
}
}

int aff_modtime(struct stat st){
if(m){
    printf("date de derniere modification : %s",ctime(&st.st_mtime));
}
}

int aff_acctime(struct stat st){
if(a || d){
    printf("dernier accès : %s \n",ctime(&st.st_atime));
}
}

int aff_protec(struct stat st){
if(a || p){
    printf("protection : %d | ",st.st_mode);
    printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
    printf( (st.st_mode & S_IRUSR) ? "r" : "-");
    printf( (st.st_mode & S_IWUSR) ? "w" : "-");
    printf( (st.st_mode & S_IXUSR) ? "x" : "-");
    printf( (st.st_mode & S_IRGRP) ? "r" : "-");
    printf( (st.st_mode & S_IWGRP) ? "w" : "-");
    printf( (st.st_mode & S_IXGRP) ? "x" : "-");
    printf( (st.st_mode & S_IROTH) ? "r" : "-");
    printf( (st.st_mode & S_IWOTH) ? "w" : "-");
    printf( (st.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}
}

int aff_birthtime(struct stat st){
if(a || d){
    printf("date de creation : / | "); /* couldn't find st.s_birthtime because it's not available in all versions */

    if(st.st_atime > st.st_mtime){
        printf("date de derniere utilisation %s",ctime(&st.st_atime));
    }else{
        printf("date de derniere utilisation %s",ctime(&st.st_mtime));
    }
}
}


/*----------------------| MAIN |-----------------------*/
int main(int argc, char *argv[])
{
int opt =0;
int i=0;
char slvl[8]="";



if(argc < 2){  /*si pas d'arguments */
    printf(MAN);
    exit(0);
}

/* getting options : special case : if -8 -9 were given it'll be considered as -89 */
while ((opt = getopt(argc, argv, "admstp0123456789h::")) != -1) {
    switch(opt) {
    case 'a':
    a = 1;
    break;
    case 'd':
    d = 1;
    break;
    case 'm':
    m = 1;
    break;
    case 's':
    s = 1;
    break;
    case 't':
    t = 1;
    break;
    case 'p':
    p = 1;
    break;
    case '0':
    strcat(slvl,"0");
    break;
    case '1':
    strcat(slvl,"1");
    break;
    case '2':
    strcat(slvl,"2");
    break;
    case '3':
    strcat(slvl,"3");
    break;
    case '4':
    strcat(slvl,"4");
    break;
    case '5':
    strcat(slvl,"5");
    break;
    case '6':
    strcat(slvl,"6");
    break;
    case '7':
    strcat(slvl,"7");
    break;
    case '8':
    strcat(slvl,"8");
    break;
    case '9':
    strcat(slvl,"9");
    break;
    case 'h':
        printf(MAN);
        exit(0);
    break;
    case '?':
        printf("Unknown option `-%c'.\n",optopt);
        printf(MAN);
        exit(0);
    break;
    }
};

if(argc == optind){ /* case : no args */
    printf(MAN);
    exit(0);
    
}else if ((argc - optind) >= 1 )
{
    
    strcpy(filename ,argv[argc-1]);
    strcpy(head_path,argv[optind]);

    if ((argc-1) == optind) /* case : no directory given => search in current directory */
    {
        /* head_path = current path */
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("search on Current working dir: %s\n", cwd);
            strcpy(head_path,cwd);
        } else {
            perror("getcwd() error");
            return 1;
   }
    }
    
    
}

/* get search level */
if(strcmp(slvl,"")){
lvl = atoi(slvl);
}else{
    all_lvl = 1;
}

/* start search */
i = get_subdirs(head_path,lvl);

exit(i);
}
