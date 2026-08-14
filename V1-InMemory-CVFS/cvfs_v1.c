//////////////////////////////////////////////////////////
//
//  Header Files Inclusion
//
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////
//
//  User Defined Macros
//
//////////////////////////////////////////////////////////

#define MAXINODE 5
#define MAXFILESIZE 50
#define MAXOPENFILES 5

#define READ 1
#define WRITE 2
#define EXECUTE 4


#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

//////////////////////////////////////////////////////////
//
//  User Defined Macros for Error Handling
//
//////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4
 
#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8


//////////////////////////////////////////////////////////
//
// Structure Name:    BootBlock
// Description:       It holds the information to boot the 
//                    operating system
//////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];

};


//////////////////////////////////////////////////////////
//
// Structure Name:    SuperBlock
// Description:       It holds the information of complete 
//                    file system
//////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};


//////////////////////////////////////////////////////////
//
// Structure Name:    Inode
// Description:       It holds the information of file
//                   
//////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];      // this is actually in os
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;       // read and write 
    char *Buffer;         // DataBlock
    struct Inode *next;

};

typedef struct Inode INODE;
typedef struct Inode* PINODE;
typedef struct Inode** PPINODE;

//////////////////////////////////////////////////////////
//
// Structure Name:    FileTable
// Description:       It holds the information of opened
//                    files
//////////////////////////////////////////////////////////

#pragma pack(1)
struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
                                         
};

typedef struct FileTable FILETABLE;
typedef struct FileTable* PFILETABLE;

//////////////////////////////////////////////////////////
//
// Structure Name:    UAREA
// Description:       It holds the information of process
//                    
//////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
    
};

//////////////////////////////////////////////////////////
//
//  Global variabes used in the project
//
//////////////////////////////////////////////////////////

struct BootBlock bootobj;
struct SuperBlock superobj;
struct UAREA uareaobj;

PINODE head = NULL;

//////////////////////////////////////////////////////////
//
//  Function Name:      InitialiseUAREA
//  Description:        It is used to initialise UAREA
//  Author:             Shraddha khetmalis
//  Date:               31/07/2026
//
//////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    int i = 0;

    strcpy(uareaobj.ProcessName, "Myexe");

    for(i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVFS : UAREA gets initialised sucessfully\n");
}

//////////////////////////////////////////////////////////
//
//  Function Name:      InitialiseSuperBlock
//  Description:        It is used to initialise Super Block
//  Author:             Aditi Dilip Sarode
//  Date:               31/07/2026
//
//////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super Block gets initialised sucessfully\n");
    
}

//////////////////////////////////////////////////////////////////
//
//  Function Name:      CreateDILB
//  Description:        It is used to create linked list of inodes
//  Author:             Aditi Dilip Sarode
//  Date:               31/07/2026
//
//////////////////////////////////////////////////////////////////

void CreateDILB()
{
    PINODE temp = NULL;
    PINODE newn = NULL;

    int i = 0;

    temp = head;
    
    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));
        newn = (PINODE)malloc(sizeof(INODE));

        // If malloc() fails:causes a segmentation fault by newn->InodeNumber
        if(newn == NULL)
        {
            printf("Unable to allocate memory for inode\n");
            return;
        }

        newn->InodeNumber = i;
        strcpy(newn->FileName, "\0");
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;


        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }

    }

    printf("Marvellous CVFS : DILB gets created sucessfully\n");
}

////////////////////////////////////////////////////////////////////
//
//  Function Name:      StartAuxiliaryDataInitialisation
//  Description:        It is used to call all such functions which  
//                      are used to initialise auxiliary data
//  Author:             Aditi Dilip Sarode
//  Date:               31/07/2026
//
////////////////////////////////////////////////////////////////////

void StartAuxiliaryDataInitialisation()
{
    strcpy(bootobj.Information, "Booting process of Marvellous CVFS is completed");

    printf("%s\n",bootobj.Information);
    
    InitialiseUAREA();

    InitialiseSuperBlock();

    CreateDILB();

}

////////////////////////////////////////////////////////////////////
//
//  Function Name:      DisplayHelp
//  Description:        It is used to display help to the user of  
//                      project
//  Author:             Aditi Dilip Sarode
//  Date:               01/08/2026
//
////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("---------------------------------------------------\n");
    printf("----------- Marvellous CVFS Help page ------------\n");
    printf("---------------------------------------------------\n");

    printf("man : It is used to display the manual page\n");
    printf("clear : It is used to clear the terminal screen\n");
    printf("creat : It is used to create new regular file\n ");
    printf("open : It is used to open regular file\n");
    printf("close : It is used to close regular file\n");
    printf("write : It is used to write the data into the file\n");
    printf("read : It is used to read data from the file\n");
    printf("stat : It is used to display statistical information of file\n");
    printf("unlink : It is used to delete the file\n");
    printf("exit : It is used to terminate Marvellous CVFS\n");

    printf("---------------------------------------------------\n");
    
}


////////////////////////////////////////////////////////////////////
//
//  Function Name:      ManPageDisplay
//  Description:        It is used to display man page of specific  
//                      command
//  Input :             Name of command
//  Author:             Aditi Dilip Sarode
//  Date:               01/08/2026
//
////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp(Name, "exit") == 0)
    {
        printf("About : It is used to terminate the project\n");
        printf("Usage : exit\n");
    }
    else if(strcmp(Name, "ls") == 0)
    {
        printf("About : It is used to list all files from current directory\n");
        printf("Usage : ls\n");
    }
    else if(strcmp(Name, "clear") == 0)
    {
        printf("About : It is used to clear the terminal\n");
        printf("Usage : clear\n");
    }
    else if(strcmp(Name, "creat") == 0)
    {
        printf("About : It is used to create new file\n");
        printf("Usage : creat file_name permission\n");
        printf("File_name Nmae of file that we want to create\n");

        printf("Permission:Permision of new file\n");

        printf("permission : read->1\n");
        printf("permission : write->2\n");
        printf("permission : read+write->3\n");
    }
    else if(strcmp(Name, "unlink") == 0)
    {
        printf("About : It is used to delete existing file\n");
        printf("Usage : unlink file_name \n");

        printf("File_name: Nmae of file that we want to delete\n");
    }
    else if(strcmp(Name, "stat") == 0)
    {
        printf("About : It is used to get information of file\n");
        printf("Usage : stat file_name \n");

        printf("File_name: name of file whose information should be fetched\n");
    }
    else
    {
        printf("No manual entry found for %s\n", Name);
    }

}

////////////////////////////////////////////////////////////////////
//
//  Function Name:   IsFileExist
//  Description:     It is used to check whether file is present   
//                   or not
//  Input :          name of file 
//  Output :         true if present 
//                   false if not present
//  Author:          Aditi Dilip Sarode
//  Date:            01/08/2026
//
////////////////////////////////////////////////////////////////////

bool IsFileExist(
                  char name[]     // Name of file
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0)
        {
            bFlag = true;
            break;
        }

        temp = temp->next;
    }
    return bFlag;
}

////////////////////////////////////////////////////////////////////
//
//  Function Name:      CreateFile
//  Description:        It is used to create new file
//  Input :             Name of file & Permissions
//  Output :            File Descriptor
//  Author:             shraddha prakash khetmalis
//  Date:               01/08/2026
//
////////////////////////////////////////////////////////////////////

int CreateFile(
                    char name[],          // Name of file
                    int permission        // File Permission
              )
{
    int i = 0;     

    PINODE temp = head;

    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;

    }

    // If permission value is wrong
    // Permission = 1 -> READ
    // Permission = 2 -> WRITE
    // Permission = 3 -> READ + WRITE

    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // search for empty inode 
    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }

        temp = temp->next;
    }

    // Rare case
    if(temp == NULL)
    {
        return ERR_NO_INODES;
    }

    // Search empty UFDT entry
    // Reserve first 3 FD's
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    if(uareaobj.UFDT[i] == NULL)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Initialize File Table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    // Connect File Table with Inode
    uareaobj.UFDT[i]->ptrinode = temp;
    
    // Initialize all members of inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName, name);

    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;

    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;

    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;

    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate memory for files data (Data Block)

    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    if(uareaobj.UFDT[i]->ptrinode->Buffer == NULL)
    {
        free(uareaobj.UFDT[i]);
        uareaobj.UFDT[i] = NULL;

        return ERR_INSUFFICIENT_SPACE;
    }

    superobj.FreeInodes--;
   
    return i;
}

////////////////////////////////////////////////////////////////////
//
//  Function Name:      LsFile
//  Description:        It is used to display names of all files
//  Input :             None
//  Output :            None
//  Author:             shraddha prakash khetmalis
//  Date:               01/08/2026
//
////////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("---------------------------------------------------\n");
    printf("------ Marvellous CVFS Files Information ------\n");
    printf("---------------------------------------------------\n");
    
    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s\n", temp->FileName);
        }

        temp = temp->next;
    }
}

////////////////////////////////////////////////////////////////////
//
//  Function Name:      LsFile_All
//  Description:        It is used to display all deatails of all file(gives basic metadata of every existing file)
//  Input :             None
//  Output :            None
//  Author:             shraddha prakash khetmalis
//  Date:               02/08/2026
//
////////////////////////////////////////////////////////////////////

void LsFile_All()
{
    PINODE temp = head;

    printf("---------------------------------------------------\n");
    printf("------ Marvellous CVFS Files Information ------\n");
    printf("---------------------------------------------------\n");
    
    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s %d %d\n", 
                temp->FileName,temp->InodeNumber,temp->ActualFileSize);
        }

        temp = temp->next;
    }
}
////////////////////////////////////////////////////////////////////
//
//  Function Name:      stat_file()
//  Description:        It is used to display all deatails of specific files
//  Input :             File name
//  Output :            Exist status of function 
//  Author:             shraddha prakash khetmalis
//  Date:               02/08/2026
//
////////////////////////////////////////////////////////////////////

int stat_file(char name[]){
    PINODE temp=NULL;
    int Permission=0;
    int Type=0;


        if(IsFileExist(name)==false){
            return ERR_FILE_NOT_EXIST;
        }

        temp=head;

        while(temp!=NULL){
            if(strcmp(temp->FileName,name)==0){
                printf("---------------------------------------------------\n");
                printf("------ Statistical information of file------\n");
                printf("---------------------------------------------------\n");

                printf("File name:%s\n",temp->FileName);

                printf("Inode number:%d\n",temp->InodeNumber);
                
                printf("File size:%d\n",temp->FileSize);

                printf("Actual file size:%d\n",temp->ActualFileSize);

                printf("Reference count:%d\n",temp->ReferenceCount);

                Permission=(temp->Permission);

                if(Permission==READ){
                    printf("File permission:Read\n");
                }
                else if (Permission==WRITE)
                {
                    printf("File permission:Write\n");
                }
                else if(Permission==WRITE+READ){
                    printf("File permission:Read+Write\n");
                }
                Type=temp->FileType;
                if(Type==REGULARFILE){
                    printf("File Type:Regular File\n");
                }
                else if(Type==SPECIALFILE){
                    printf("File Type:special File\n");
                }
                printf("---------------------------------------------------\n");                
                break;
            }
            temp=temp->next;
        }
        return EXECUTE_SUCCESS;


}
////////////////////////////////////////////////////////////////////
//
//  Function Name:      unlink_file()
//  Description:        It is used to delete specific file
//  Input :             File name
//  Output :            Exist status of function
//  Author:             shraddha prakash khetmalis
//  Date:               02/08/2026
//
////////////////////////////////////////////////////////////////////

int unlink_file(
        char name[]//Name of file
        )
{
    int i;
    if(IsFileExist(name)==false){
        return ERR_FILE_NOT_EXIST;
    }
    //Travel the UFDt
    for(i=0;i<MAXOPENFILES;i++){
        if(uareaobj.UFDT[i]!=NULL){
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName,name)==0){

                //Deallocate memeory of bufffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer=NULL;
                strcpy(uareaobj.UFDT[i]->ptrinode->FileName,"\0");
                uareaobj.UFDT[i]->ptrinode->FileSize=0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize=0;
                uareaobj.UFDT[i]->ptrinode->FileType=0;//inode is available for another file
                uareaobj.UFDT[i]->ptrinode->Permission=0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount=0;

                //Deallocate memory of file table
                free(uareaobj.UFDT[i]);

                uareaobj.UFDT[i]=NULL;
                superobj.FreeInodes++;

                break;//IMPORTANT
            }
        }//end of if
    }//end of 
    
    return EXECUTE_SUCCESS;

}//end of unlink_function

////////////////////////////////////////////////////////////////////
//
//  Function Name:      write_file()
//  Description:        It is used to write the data into specific file
//  Input :             File descriptior 
//                      Data that we want to write
//                      Size of data(number of bytes to write)
//  Output :            Number of bytes sussesfully writtem
//  Author:             shraddha prakash khetmalis
//  Date:               02/08/2026
//
////////////////////////////////////////////////////////////////////
int write_file(
                int fd,
                char *data,
                int size
                )
    {
        int offset=0;

        printf("File descriptor:%d\n",fd);
        printf("Data that we want to write:%s\n",data);
        printf("Size of data:%d\n",size);

        if(fd<0 || fd>=MAXOPENFILES){
            return ERR_INVALID_PARAMETER;
        }

        //check whether file is there or not if it is not there then it might create segmenation fault
        if(uareaobj.UFDT[fd] == NULL)
        {
            return ERR_FILE_NOT_EXIST;
        }

        //if writting permission is not there 
        if(uareaobj.UFDT[fd]->ptrinode->Permission<WRITE){
            return ERR_PERMISSION_DENIED;
        }

        //to check whether space is there or not
        if((MAXFILESIZE-uareaobj.UFDT[fd]->WriteOffset)<size){
            return ERR_INSUFFICIENT_SPACE;
        }

        //offset=(uareaobj.UFDT[fd]->ptrinode->Buffer+uareaobj.UFDT[fd]->WriteOffset);

        //Actual data writting
        strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer
                    +uareaobj.UFDT[fd]->WriteOffset,data,size);//dont copy complete data copy specific data (Start writing from the current write position instead of overwriting existing data from position 0)

        //update the write offset
        uareaobj.UFDT[fd]->WriteOffset=uareaobj.UFDT[fd]->WriteOffset+size;

        //update actual file size
        uareaobj.UFDT[fd]->ptrinode->ActualFileSize=uareaobj.UFDT[fd]->ptrinode->ActualFileSize+size;

        return size;

}
////////////////////////////////////////////////////////////////////
//
//  Function Name:      read_file()
//  Description:        It is used to read the data into specific file
//  Input :             File descriptior 
//                      address of empty buffer
//                      Size of data
//  Output :            Number of bytes sussesfully writtem
//  Author:             shraddha prakash khetmalis
//  Date:               02/08/2026
//
////////////////////////////////////////////////////////////////////

int read_file(
                int fd,
                char *data,
                int size
            )
{
        //Invalid FD
        if(fd<0 || fd>=MAXOPENFILES){
            return ERR_INVALID_PARAMETER;
        }

        if(size<0){
            return ERR_INVALID_PARAMETER;
        }

         if(uareaobj.UFDT[fd]==NULL){
            return ERR_FILE_NOT_EXIST;
        }

        //Filter for the permission 
        if(uareaobj.UFDT[fd]->ptrinode->Permission<READ){
            return ERR_PERMISSION_DENIED;
        }


        //insufficent data
        if((uareaobj.UFDT[fd]->ptrinode->ActualFileSize - uareaobj.UFDT[fd]->ReadOffset) < size)
            {
            return ERR_INSUFFICIENT_DATA;
            }

        //read the data
        strncpy(data,
                uareaobj.UFDT[fd]->ptrinode->Buffer+uareaobj.UFDT[fd]->ReadOffset,
                size);

        
        // Update read offset
        uareaobj.UFDT[fd]->ReadOffset += size;
        
        return size;
}

//////////////////////////////////////////////////////////
//
//  Entry Point Function of CVFS Project
//
//////////////////////////////////////////////////////////

int main()
{
    // Input command
    char str[80] = {'\0'};

    // Tokenized command
    char Command[5][20] = {{'\0'}};

    // Data for write system call
    char InputBuffer[MAXFILESIZE] = {'\0'};

    // Buffer used to store data read from file
    char *EmptyBuffer = NULL;

    int size = 0;
    int iRet = 0;
    int iCount = 0;

    //////////////////////////////////////////////////////////
    //
    // Start CVFS initialisation
    //
    //////////////////////////////////////////////////////////

    StartAuxiliaryDataInitialisation();

    printf("---------------------------------------------------\n");
    printf("------ Marvellous CVFS started successfully ------\n");
    printf("---------------------------------------------------\n");


    //////////////////////////////////////////////////////////
    //
    // Infinite Listening Shell
    //
    //////////////////////////////////////////////////////////

    while(1)
    {
        //////////////////////////////////////////////////////
        // Reset input buffers
        //////////////////////////////////////////////////////

        memset(str, '\0', sizeof(str));
        memset(Command, '\0', sizeof(Command));

        printf("\nMarvellous CVFS : > ");

        //////////////////////////////////////////////////////
        // Read complete command
        //////////////////////////////////////////////////////

        if(fgets(str, sizeof(str), stdin) == NULL)
        {
            printf("\nInput error or EOF detected\n");
            break;
        }

        //////////////////////////////////////////////////////
        // Tokenize command
        //////////////////////////////////////////////////////

        iCount = sscanf(
                    str,
                    "%19s %19s %19s %19s %19s",
                    Command[0],
                    Command[1],
                    Command[2],
                    Command[3],
                    Command[4]
                );


        //////////////////////////////////////////////////////
        // No valid command
        //////////////////////////////////////////////////////

        if(iCount <= 0)
        {
            continue;
        }


        //////////////////////////////////////////////////////
        //
        // COMMAND WITH ONE ARGUMENT
        //
        // Examples:
        // help
        // ls
        // clear
        // exit
        //
        //////////////////////////////////////////////////////

        if(iCount == 1)
        {
            ////////////////////////////////////////////////////
            // exit
            ////////////////////////////////////////////////////

            if(strcmp(Command[0], "exit") == 0)
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all resources of Marvellous CVFS\n");

                break;
            }


            ////////////////////////////////////////////////////
            // help
            ////////////////////////////////////////////////////

            else if(strcmp(Command[0], "help") == 0)
            {
                DisplayHelp();
            }


            ////////////////////////////////////////////////////
            // clear
            ////////////////////////////////////////////////////

            else if(strcmp(Command[0], "clear") == 0)
            {
                #ifdef _WIN32

                    system("cls");

                #else

                    system("clear");

                #endif
            }


            ////////////////////////////////////////////////////
            // ls
            ////////////////////////////////////////////////////

            else if(strcmp(Command[0], "ls") == 0)
            {
                LsFile();
            }


            ////////////////////////////////////////////////////
            // Unknown command
            ////////////////////////////////////////////////////

            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }


        //////////////////////////////////////////////////////
        //
        // COMMAND WITH TWO ARGUMENTS
        //
        // Examples:
        // man creat
        // ls a
        // stat abc.txt
        // unlink abc.txt
        // write 3
        //
        //////////////////////////////////////////////////////

        else if(iCount == 2)
        {
            ////////////////////////////////////////////////////
            // man command
            ////////////////////////////////////////////////////

            if(strcmp(Command[0], "man") == 0)
            {
                ManPageDisplay(Command[1]);
            }


            ////////////////////////////////////////////////////
            // ls a
            ////////////////////////////////////////////////////

            else if(
                    strcmp(Command[0], "ls") == 0 &&
                    strcmp(Command[1], "a") == 0
                   )
            {
                LsFile_All();
            }


            ////////////////////////////////////////////////////
            // stat filename
            ////////////////////////////////////////////////////

            else if(strcmp(Command[0], "stat") == 0)
            {
                iRet = stat_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File does not exist\n");
                }
            }


            ////////////////////////////////////////////////////
            // unlink filename
            ////////////////////////////////////////////////////

            else if(strcmp(Command[0], "unlink") == 0)
            {
                iRet = unlink_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File does not exist\n");
                }
            }


            ////////////////////////////////////////////////////
            // write FD
            ////////////////////////////////////////////////////

            else if(strcmp(Command[0], "write") == 0)
            {
                printf("Enter the data that you want to write into the file\n");

                if(fgets(InputBuffer, MAXFILESIZE, stdin) == NULL)
                {
                    printf("Error : Unable to read input data\n");
                    continue;
                }

                size = strlen(InputBuffer);

                iRet = write_file(
                            atoi(Command[1]),
                            InputBuffer,
                            size
                        );


                //////////////////////////////////////////////////
                // Handle write errors
                //////////////////////////////////////////////////

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                    printf("Please check the man page\n");
                }

                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File does not exist\n");
                }

                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : There is no permission to write the data\n");
                }

                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Insufficient space in file\n");
                }

                else
                {
                    printf(
                        "%d bytes successfully written into the file\n",
                        iRet
                    );
                }
            }


            ////////////////////////////////////////////////////
            // Unknown command
            ////////////////////////////////////////////////////

            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }


        //////////////////////////////////////////////////////
        //
        // COMMAND WITH THREE ARGUMENTS
        //
        // Examples:
        //
        // creat abc.txt 3
        // read 3 10
        //
        //////////////////////////////////////////////////////

        else if(iCount == 3)
        {
            ////////////////////////////////////////////////////
            // creat filename permission
            ////////////////////////////////////////////////////

            if(strcmp(Command[0], "creat") == 0)
            {
                iRet = CreateFile(
                            Command[1],
                            atoi(Command[2])
                        );


                //////////////////////////////////////////////////
                // Handle CreateFile errors
                //////////////////////////////////////////////////

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because there is no free inode\n");
                }

                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because parameters of command are invalid\n");
                    printf("Please use man page to get actual parameters\n");
                }

                else if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because the file name is already present\n");
                    printf("Please use ls command to check names of all files\n");
                }

                else if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because the UFDT is full\n");
                    printf("Please close some opened file\n");
                }

                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because memory allocation failed\n");
                }

                else
                {
                    printf(
                        "Files successfully created with FD : %d\n",
                        iRet
                    );
                }
            }


            ////////////////////////////////////////////////////
            // read FD size
            ////////////////////////////////////////////////////

            else if(strcmp(Command[0], "read") == 0)
            {
                size = atoi(Command[2]);


                //////////////////////////////////////////////////
                // Validate requested size before malloc
                //////////////////////////////////////////////////

                if(size <= 0)
                {
                    printf("Error : Invalid read size\n");
                    continue;
                }


                //////////////////////////////////////////////////
                // Allocate one extra byte for '\0'
                //////////////////////////////////////////////////

                EmptyBuffer = (char *)malloc(size + 1);

                if(EmptyBuffer == NULL)
                {
                    printf("Error : Memory allocation failed\n");
                    continue;
                }


                //////////////////////////////////////////////////
                // Read data from CVFS
                //////////////////////////////////////////////////

                iRet = read_file(
                            atoi(Command[1]),
                            EmptyBuffer,
                            size
                        );


                //////////////////////////////////////////////////
                // Handle read errors
                //////////////////////////////////////////////////

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters\n");
                }

                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File does not exist\n");
                }

                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data\n");
                }

                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied\n");
                }

                else
                {
                    //////////////////////////////////////////////////
                    // Null terminate the data
                    //////////////////////////////////////////////////

                    EmptyBuffer[iRet] = '\0';

                    printf("Read operation successful\n");
                    printf("Data read from file : %s\n", EmptyBuffer);
                    printf("Bytes read : %d\n", iRet);
                }


                //////////////////////////////////////////////////
                // Free dynamically allocated read buffer
                //////////////////////////////////////////////////

                free(EmptyBuffer);
                EmptyBuffer = NULL;
            }


            ////////////////////////////////////////////////////
            // Unknown command
            ////////////////////////////////////////////////////

            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }


        //////////////////////////////////////////////////////
        //
        // FOUR ARGUMENTS
        //
        // Currently not supported in V1
        //
        //////////////////////////////////////////////////////

        else if(iCount == 4)
        {
            printf("Command not supported in V1\n");
            printf("Please refer help option to get more information\n");
        }


        //////////////////////////////////////////////////////
        //
        // More than four arguments
        //
        //////////////////////////////////////////////////////

        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
            printf("Please refer manual page of command using man\n");
        }

    } // End of while


    //////////////////////////////////////////////////////////
    //
    // Program termination
    //
    //////////////////////////////////////////////////////////

    return 0;

} // End of main