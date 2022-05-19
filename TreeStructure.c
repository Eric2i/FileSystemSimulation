#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define OK 0;
#define ERR 1;

enum FileType {File, Dir};

enum CmdType {md, create, cd, dir, eexit, cmdErr};

const int MAX_LENGTH = 4096;

struct file {
    enum FileType type;
    struct file *subdir;
    struct file *brodir;
    int numSubDir;
    int numBroDir;
    char Name[MAX_LENGTH];
} root;

int InitialFileSystem(struct file *root) {
    InitialFileNode(root, "/(root)#/");
}

int InitialDirNode(struct file *p, char *name) {
    if(strcpy(p->Name, name)) {
        p->type = Dir;
        p->subdir = NULL;
        p->brodir = NULL;
        p->numBroDir = 0;
        p->numSubDir = 0;
        return OK;
    }
    else {
        return ERR;
    }
}

enum CmdType userCmdParsing(char *argv, char ***cmd, int *argc) {
    // preprocess userCmd
    argv[strlen(argv) - 1] == '\n' ? 
    (argv[strlen(argv) - 1] = ' ') :
    (argv[strlen(argv) + 1] = 0,argv[strlen(argv)] = ' ');
    // printf("[parsing test]add suffix white space, \nnow length is:%lu\n", strlen(argv));

    // Get Command's each augment
    int i, j;
    int numArguments = 0;

    i = 0;
    while(argv[i]) if(argv[i++] == ' ') numArguments++;
    *argc = numArguments;
    // printf("[parsing test]number of arguments: %d\n", numArguments);

    int idx = 0;
    *cmd = (char **)malloc(sizeof(char*) * numArguments);
    i = 0; j = i + 1;
    while(argv[j]) {
        if(argv[j++] == ' ') {
            char *eachCmd = (char*) malloc(sizeof(char) * (j-i));
            eachCmd[j-i-1] = 0;
            strncpy(eachCmd, argv+i, j-i-1);
            (*cmd)[idx++] = eachCmd;
            i = j;
        }
    }

    // Get Command's Head
    enum CmdType type = cmdErr;
    // printf("Cmd head is: %s\n", **cmd);
    switch(strlen(**cmd)) {
        case 2:
            if(!strcmp(**cmd, "cd")) {type = cd;}
            else if(!strcmp(**cmd, "md")) {type = md;}
            break;
        case 3:
            if(!strcmp(**cmd, "dir")) {type = dir;}
            break;
        case 4:
            if(!strcmp(**cmd, "exit")) {type = eexit;}
            break;
        case 6:
            if(!strcmp(**cmd, "create")) {type = create;}
            break;
    }
    // printf("[parsing test]cmdHead: %d\n", type);

    // Arguments Test
    // int tt = 0;
    // while(tt < numArguments) {
    //     printf("%d argument, ", tt+1);
    //     printf("[parsing test]%s\n", (*cmd)[tt++]);
    // }

    return type;
};

int makedir(struct file *p, int argc, char **argv){
    int i;
    for(i = 1; i < argc; i++) {
        // TODO: what if subdir is not empty?
        p->subdir = (struct file *)malloc(sizeof(struct file));
        p = p->subdir;
        InitialDirNode(p, argv[i]);
    }
    return OK;
}

int createfile(struct file *p, int argc, char **argv){
    
    return OK;
}

int changedir(struct file *p, int argc, char **argv) {
    
    return OK;
}

int list(struct file *p, int argc, char **argv) {
    printf("%s\n", p->Name);
    int i = 0;
    p = p->numSubDir;
    while(p->brodir) {
        printf("\t%s\n", p->Name);
        p = p->brodir;
    }
    if(p->numSubDir == 0) printf("\t<Empty Directory!>\n");
    return OK;
}

int main() {
    if(!InitialFileSystem(&root)) {
        printf("Welcome to Eric2i's FileSystem, File System initialized successfully.\n\n");
        char userCmd[MAX_LENGTH];
        struct file *current = &root;
        int userExit = 0;

        while(!userExit) {
            // always printf current path
            printf("[Eric2i File System Simulation]%s $ ", current->Name);

            // Read from user's command;
            char **cmd;
            int argc;
            fgets(userCmd, sizeof(userCmd), stdin);
            switch(userCmdParsing(userCmd, &cmd, &argc)) {
                case md:
                    makedir(current, argc, cmd);
                    break;
                case create:
                    createfile(current, argc, cmd);
                    break;
                case cd:
                    changedir(current, argc, cmd);
                    break;
                case dir:
                    list(current, argc, cmd);
                    break;
                case eexit:
                    userExit = 1;
                    break;
                case cmdErr:
                    printf("command not found: %s\n", cmd[0]);
            }
        }
        return 0;
    }
    else {
        printf("[Error]: Failed to Create File System!\n Exiting Program...");
        return ERR;
    }
}