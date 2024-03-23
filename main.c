#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

struct Node {   // a struct for a BST to store a students with all data's.
    int Id;
    char* Name;
    char* HomeAddress;
    int ClassId;
    char* DataOfEnrollment;

    struct Node* Left;
    struct Node* Right;
};
typedef struct Node* TNode;


struct NodeLexicographicOrder { // a struct for BST (only stores the name of the student to be printed later in Lexicographic order ).
    char* Name;
    struct NodeLexicographicOrder* Left;
    struct NodeLexicographicOrder* Right;
};
typedef struct NodeLexicographicOrder* OrderNode;

//------------------Functions prototype--------------------

TNode MakeEmpty(TNode);
TNode Find(int, TNode);
TNode FindMin(TNode);
TNode FindMax(TNode);
TNode Insert(int, char*, char*, int, char*, TNode);
TNode Delete(int, TNode);


OrderNode MakeEmptyO(OrderNode);
OrderNode InsertO(char*,OrderNode);
OrderNode FindMaxO(OrderNode);
OrderNode DeleteO(char*,OrderNode);

OrderNode InsertToNewTree(OrderNode,TNode);
OrderNode InsertToNewTreeWithSpecificCity(OrderNode, TNode,char*);
OrderNode InsertToNewTreeSameClass(OrderNode,TNode, int);

OrderNode MoveFromTreeTOTree(TNode);
OrderNode MoveFromTreeTOTreeWithSpecificCity(TNode,char*);
OrderNode MoveFromTreeTOTreeSameClass(TNode,int);

void LexicographicOrder(TNode);
void LexicographicOrderWithSpecificCity(TNode,char*);
void LexicographicOrderWithSameClass(TNode,int);

void PrintInfoToFile(FILE*,TNode);


void menu();
void Sub_menu(TNode);
void UpdateMenu(TNode,TNode);
void StartFromZero();


int main()
{
    menu();

    return 0;
}

//------------------FUNCTIONS------------------------------

// a modified functions of the BST because Id,Name,City,.....
TNode MakeEmpty(TNode T){
    if (T != NULL) {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T->Name);
        free(T->HomeAddress);
        free(T->DataOfEnrollment);
        free(T);
    }
    return NULL;
}

TNode Find(int Id, TNode T){
    if (T == NULL)
        return NULL;
    else if (Id < T->Id)
        return Find(Id, T->Left);
    else if (Id > T->Id)
        return Find(Id, T->Right);
    else
        return T;
}

TNode FindMin(TNode T){
    if (T != NULL)
        while (T->Left != NULL)
            T = T->Left;
    return T;
}

TNode FindMax(TNode T){
    if (T == NULL)
        return NULL;
    else if (T->Right == NULL)
        return T;
    else
        return FindMax(T->Right);
}

TNode Insert(int Id,char* Name, char* HomeAddress, int ClassId,char* DataOfEnrollment, TNode T) {
    if (T == NULL) {
        T = (struct Node*)malloc(sizeof(struct Node));
        if (T == NULL) {
            printf("Out of space");
            return NULL;
        }
        T->Id = Id;
        T->Name = malloc(strlen(Name) + 1);
        strcpy(T->Name, Name);
        T->HomeAddress = malloc(strlen(HomeAddress) + 1);
        strcpy(T->HomeAddress, HomeAddress);
        T->ClassId = ClassId;
        T->DataOfEnrollment = malloc(strlen(DataOfEnrollment) + 1);
        strcpy(T->DataOfEnrollment, DataOfEnrollment);
        T->Left = T->Right = NULL;
    } else if (Id < T->Id) {
        T->Left = Insert(Id, Name, HomeAddress, ClassId, DataOfEnrollment, T->Left);
    } else if (Id > T->Id) {
        T->Right = Insert(Id, Name, HomeAddress, ClassId, DataOfEnrollment, T->Right);
    }
    return T;
}

TNode Delete(int Id, TNode T) {
    if (T == NULL) {
        printf("Did not find the student to be deleted!\n");
        return NULL;
    }
    else if (Id < T->Id) {
        T->Left = Delete(Id, T->Left);
    }
    else if (Id > T->Id) {
        T->Right = Delete(Id, T->Right);
    }
    else { // ID found!!
        if (T->Left == NULL && T->Right == NULL) { // No children
            free(T->Name);
            free(T->HomeAddress);
            free(T->DataOfEnrollment);
            free(T);
            T = NULL;
            printf("The student is successfully deleted!\n");
        }
        else if (T->Left == NULL) { // One right child
            TNode temp = T;
            T = T->Right;
            free(temp->Name);
            free(temp->HomeAddress);
            free(temp->DataOfEnrollment);
            free(temp);
            printf("The student is successfully deleted!\n");
        }
        else if (T->Right == NULL) { // One left child
            TNode temp = T;
            T = T->Left;
            free(temp->Name);
            free(temp->HomeAddress);
            free(temp->DataOfEnrollment);
            free(temp);
            printf("The student is successfully deleted!\n");
        }
        else { // Two children
            TNode temp = FindMin(T->Right);
            T->Id = temp->Id;


            T->Name = realloc(T->Name, strlen(temp->Name) + 1);
            strcpy(T->Name, temp->Name);

            T->HomeAddress = realloc(T->HomeAddress, strlen(temp->HomeAddress) + 1);
            strcpy(T->HomeAddress, temp->HomeAddress);

            T->ClassId = temp->ClassId;

            T->DataOfEnrollment = realloc(T->DataOfEnrollment, strlen(temp->DataOfEnrollment) + 1);
            strcpy(T->DataOfEnrollment, temp->DataOfEnrollment);

            T->Right = Delete(T->Id, T->Right);
        }
    }
    return T;
}



//---------------for lexicographic order tree by name--------------------------------

// For print students in lexicographic order ,I create a  new BST which is arranged depending on the students names.
// All students are already stored in BST with all data's so in this algorithm, I trace all students in the tree and insert his/her name in the right place in the new BST (by names).
// Now all students are stored in the new BST (by names), and for print them in lexicographic order all we need to is to print the max then delete it  and so on till the end of the tree to get empty.
// These functions are same as the above functions but it stores the name of the student (because we only need to print the name of the students in  lexicographic order).
// note that the O (at the end) like MakeEmptyO is for differ these functions from the above one's.

OrderNode MakeEmptyO( OrderNode T ){
    if( T != NULL ){
        MakeEmptyO( T->Left );
        MakeEmptyO( T->Right );
        free( T );
    }
    return NULL;
}

OrderNode InsertO(char* Name, OrderNode T) {
    if (T == NULL) {
        // Create and return a 1-node tree
        T = (OrderNode)malloc(sizeof(struct NodeLexicographicOrder));
        if (T == NULL) {
            printf("Out of space\n");
        } else {
            T->Name = strdup(Name);
            T->Left = T->Right = NULL;
        }
    } else if (strcmp(Name, T->Name) < 0) {
        T->Left = InsertO(Name, T->Left);
    } else if (strcmp(Name, T->Name) > 0) {
        T->Right = InsertO(Name, T->Right);
    }
    // else, Name is in the tree already; do nothing
    return T;
}

OrderNode FindMaxO( OrderNode T ){
    if( T == NULL)
        return NULL;
    else if( T -> Right == NULL)
        return T;
    else
        return FindMaxO( T->Right);
}

OrderNode FindMinO(OrderNode T){
    if (T != NULL)
        while (T->Left != NULL)
            T = T->Left;
    return T;
}

OrderNode DeleteO(char* Name, OrderNode T) {
    if (T == NULL) {
        return NULL;
    } else if (strcmp(Name, T->Name) < 0) {
        T->Left = DeleteO(Name, T->Left);
    } else if (strcmp(Name, T->Name) > 0) {
        T->Right = DeleteO(Name, T->Right);
    } else { // Name Found!!
        if (T->Left == NULL && T->Right == NULL) { // No children
            free(T->Name);
            free(T);
            T = NULL;
        } else if (T->Left == NULL) { // One right child
            OrderNode temp = T;
            T = T->Right;
            free(temp->Name);
            free(temp);
        } else if (T->Right == NULL) { // One left child
            OrderNode temp = T;
            T = T->Left;
            free(temp->Name);
            free(temp);
        } else { // Two children
            OrderNode temp = FindMinO(T->Right);
            free(T->Name);
            T->Name = strdup(temp->Name);
            T->Right = DeleteO(temp->Name, T->Right);
        }
    }
    return T;
}


//InsertToNewTree function is a recursive function to trace all nodes(students) in the origin BST (Students with all data's) and insert a student name to the new BST (by names).

OrderNode InsertToNewTree(OrderNode NewTree, TNode T) {
    if (T != NULL) {
        NewTree = InsertO(T->Name, NewTree);
        NewTree = InsertToNewTree(NewTree, T->Left);
        NewTree = InsertToNewTree(NewTree, T->Right);
    }
    return NewTree;
}

//InsertToNewTreeWithSpecificCity same as InsertToNewTree but it only insert the students that are belonging to that city .(Example.. it insert all student that are from birzeit, students from Jifna will not be added. ).
OrderNode InsertToNewTreeWithSpecificCity(OrderNode NewTree, TNode T, char* CityName) {
    if (T != NULL) {
        if (strcmp(T->HomeAddress, CityName) == 0) {
            NewTree = InsertO(T->Name, NewTree);
        }
        NewTree = InsertToNewTreeWithSpecificCity(NewTree, T->Left, CityName);
        NewTree = InsertToNewTreeWithSpecificCity(NewTree, T->Right, CityName);
    }
    return NewTree;
}

//InsertToNewTreeSameClass same as InsertToNewTree but it only insert the students that are belonging to that class(Example..it insert all student that are studying in the 7th grade , the other students will be added to a tree depending on their class's).
OrderNode InsertToNewTreeSameClass(OrderNode NewTree,TNode T, int ClassId) {
    if (T != NULL) {
        if (T->ClassId==ClassId) {
            NewTree = InsertO(T->Name, NewTree);
        }
        NewTree = InsertToNewTreeSameClass(NewTree, T->Left, ClassId);
        NewTree = InsertToNewTreeSameClass(NewTree, T->Right, ClassId);
    }
    return NewTree;
}


// Here we use the recursive function above to move a student to new BST.
OrderNode MoveFromTreeTOTree(TNode T) {
    TNode temp = T;
    OrderNode NewTree = MakeEmptyO(NULL);
    NewTree = InsertToNewTree(NewTree, temp);
    return NewTree;
}

OrderNode MoveFromTreeToTreeWithSpecificCity(TNode T, char* CityName) {
    OrderNode NewTree = MakeEmptyO(NULL);
    NewTree = InsertToNewTreeWithSpecificCity(NewTree, T, CityName);
    return NewTree;
}

OrderNode MoveFromTreeTOTreeSameClass(TNode T, int ClassId) {
    OrderNode NewTree = MakeEmptyO(NULL);
    NewTree = InsertToNewTreeSameClass(NewTree,T,ClassId);
    return NewTree;
}


// Here a LexicographicOrder function is take the min(it should be max , but the true result when I modified it to min instead of max) and print it after that it delete it and print the next min and so on till the end of the tree to get empty.

void LexicographicOrder(TNode T) {
    TNode temp=T;
    printf("The lexicographic order of all students:\n");
    OrderNode NewTree = MakeEmptyO(NULL);

    NewTree = MoveFromTreeTOTree(temp);

    OrderNode min = MakeEmptyO(NULL);

    while (NewTree != NULL) {
        min = FindMinO(NewTree);
        printf("%s\n", min->Name);
        NewTree = DeleteO(min->Name, NewTree);
    }
}

void LexicographicOrderWithSpecificCity(TNode T, char* CityName) {
    printf("The lexicographic order of all students from %s is:\n", CityName);

    OrderNode NewTree = MoveFromTreeToTreeWithSpecificCity(T, CityName);

    while (NewTree != NULL) {
        OrderNode min = FindMinO(NewTree);
        printf("%s\n", min->Name);
        NewTree = DeleteO(min->Name, NewTree);
    }
}

void LexicographicOrderWithSameClass(TNode T, int ClassId) {
    printf("The lexicographic order of  students in %dth class:",ClassId);

    OrderNode NewTree = MakeEmptyO(NULL);

    NewTree = MoveFromTreeTOTreeSameClass(T,ClassId);

    OrderNode min = MakeEmptyO(NULL);

    while (NewTree != NULL) {
        min = FindMinO(NewTree);
        printf("%s  ", min->Name);
        NewTree = DeleteO(min->Name, NewTree);
    }
}


void PrintInfoToFile(FILE* Out, TNode T) { // a recursive function to print all students with all data's to an external file(trace all nodes)
    TNode temp=T;
    if (T != NULL) {
        fprintf(Out, "Id:%d   ", temp->Id);
        fprintf(Out, "Name:%s ", temp->Name);
        fprintf(Out, "City:%s   ", temp->HomeAddress);
        fprintf(Out, "ClassId:%d   ", temp->ClassId);
        fprintf(Out, "Date Of Enrollment:%s   ", temp->DataOfEnrollment);
        fprintf(Out, "\n");

        PrintInfoToFile(Out, temp->Left);
        PrintInfoToFile(Out, temp->Right);
    }
}



//--------------------MENU Functions-----------------------------------------
void menu() {
    //int readFromFile=0;
    FILE* inp;
    TNode StudentsTree = MakeEmpty(NULL);
    char filename[100];

    printf("Do you want to Load students from a File?\n");
    printf("1) Yes\n");
    printf("2) No, Start from zero (No students in the school)\n");
    printf("Your choice: ");
    int choice1;
    scanf("%d", &choice1);
    printf("\n");

    if (choice1 == 1) {
        printf("Note that the file format is like->1201264 Jubran Jerusalem 12 19-1-2020\n\n");
        printf("Enter the input File Name:");
        scanf("%s", filename);
        printf("\n");
        inp = fopen(filename, "r");
        if (inp != NULL) {
            int Id;
            char Name[100];
            char HomeAddress[100];
            int ClassId;
            char DataOfEnrollment[100];

            int counter = 0;
            printf("The students in the file are:\n");
            while (fscanf(inp, "%d %s %s %d %s", &Id, Name, HomeAddress, &ClassId, DataOfEnrollment) == 5) {
                StudentsTree = Insert(Id, strdup(Name), strdup(HomeAddress), ClassId, strdup(DataOfEnrollment), StudentsTree);
                counter++;
                printf("%d) Id: %d   Name: %s   HomeAddress: %s   ClassId: %d   DateOfEnrollment: %s\n",counter, Id, Name, HomeAddress, ClassId, DataOfEnrollment);

            }

            fclose(inp);

            printf("\nThe students in the file are successfully added!\n");
            //choice1 = 2;
            //readFromFile=1;
            Sub_menu(StudentsTree);
        } else {
            printf("-------Cannot find the file!-------\n");
        }
    }

    if (choice1 == 2) {
        StartFromZero();

    } else  {
            printf("------Invalid choice------\n");
    }
}

void Sub_menu(TNode StudentsTree){
    int choice;
    printf("\nChoose One Of The following Options:\n");
    printf("1) Insert a student.\n");
    printf("2) Find a student.\n");
    printf("3) List all students in lexicographic order.\n");
    printf("4) Search for a city and list all students from that city in lexicographic order.\n");
    printf("5) List all students by their class in lexicographic order.\n");
    printf("6) Delete a student.\n");
    printf("7) Save all students in a file.\n");
    printf("8) Exit.\n");
    printf("Your Choice: ");
    scanf("%d",&choice);
    printf("\n");

    if (choice == 1) {
    TNode temp=StudentsTree;
    int Id;
    char Name[100];
    char HomeAddress[100];
    int ClassId;
    char DataOfEnrollment[100];

    printf("Enter the student Id to be added: ");
    scanf("%d",&Id);
    printf("\n");

    TNode checkIfExists =MakeEmpty(NULL); // to check if the student is already exists by his/ her Id
    checkIfExists= Find(Id, temp);
    if(checkIfExists==NULL){
        printf("Enter the student Name: ");
        scanf("%s",Name);
        printf("\n");

        printf("Enter the city name: ");
        scanf("%s",HomeAddress);
        printf("\n");

        printf("Enter the ClassId: ");
        scanf("%d",&ClassId);
        printf("\n");

        printf("Enter the Date Of Enrollment: ");
        scanf("%s",DataOfEnrollment);
        printf("\n");

        temp = Insert(Id, strdup(Name), strdup(HomeAddress), ClassId, strdup(DataOfEnrollment), temp);

        printf("The student is successfully added.\n");

        Sub_menu(temp);

    }else if(checkIfExists!=NULL){
        printf("--The student is already exists!--\n\n");
        Sub_menu(temp);
        }

    } else if (choice == 2) {
         TNode temp=StudentsTree;
         int Id;
         printf("Please enter the student Id:");
         scanf("%d",&Id);
         printf("\n");
         TNode find= Find(Id,temp);// node contain the info of the student
         if(find==NULL){
            printf("The student with Id %d is not found!\n",Id);
         }else if (find!=NULL){
            printf("The student Id:%d \n",find->Id);
            printf("The student Name:%s \n",find->Name);
            printf("City:%s\n",find->HomeAddress);
            printf("Class Id:%d\n",find->ClassId);
            printf("Date of enrollment:%s\n",find->DataOfEnrollment);

            UpdateMenu(find,temp);

         }

        Sub_menu(temp);
    } else if (choice == 3) {
        TNode temp=StudentsTree;
        LexicographicOrder(temp);

        Sub_menu(temp);
    } else if (choice == 4) {
        TNode temp=StudentsTree;
        char CityName[100];
        printf("Enter the city name: ");
        scanf("%s", CityName);
        LexicographicOrderWithSpecificCity(temp,CityName);


        Sub_menu(temp);
    } else if (choice == 5) {
        TNode temp=StudentsTree;
        for(int i=1;i<=12;i++){
            LexicographicOrderWithSameClass(temp,i);
            printf("\n");
        }

        Sub_menu(temp);
    } else if (choice == 6) {
        TNode temp=StudentsTree;
        int Id;
        printf("Please enter the student Id to be deleted:");
        scanf("%d",&Id);
        printf("\n");

        temp= Delete(Id,temp);

        Sub_menu(temp);
    } else if (choice == 7) {
            TNode temp=StudentsTree;
            char filename[100];
            printf("Enter the input File Name:");
            scanf("%s", filename);
            printf("\n");
            FILE* Out = fopen(filename,"w");
            if (Out != NULL) {
                PrintInfoToFile(Out,temp);
                fclose(Out);
                printf("Done! check your %s file.\n",filename);
            }else {
                printf("Failed to open the %s file.\n",filename);
              }

        Sub_menu(temp);
    } else if (choice == 8) {
        printf("------You Exit!------\n");
        return;

    } else {
        printf("------Invalid choice------");
    }
}

void UpdateMenu(TNode Student_INFO,TNode StudentsTree){ // menu to give the user options to update a student info when the student is found
    int choice;
    printf("\nDo you want to update any data?\n");
    printf("1)Yes\n");
    printf("2)No\n");
    printf("Your choice:");
    scanf("%d",&choice);
    printf("\n");
    if(choice==1){
        printf("which of the following you want to update?\n");

        int choice2;
        printf("1)The student Name.\n");
        printf("2)City.\n");
        printf("3)Class Id.\n");
        printf("4)Date of enrollment.\n");
        printf("Your choice:");

        scanf("%d",&choice2);
        printf("\n");
        if(choice2==1){
            char*OldName= strdup(Student_INFO->Name);
            printf("Enter the  new Name:");
            scanf("%s",Student_INFO->Name);
            printf("\n");
            printf("DONE! The student name with Id %d is changed from %s to %s",Student_INFO->Id,OldName,Student_INFO->Name);
            UpdateMenu(Student_INFO,StudentsTree);
        }
        if(choice2==2){
            char*OldCity= strdup(Student_INFO->HomeAddress);
            printf("Enter the  new City:");
            scanf("%s",Student_INFO->HomeAddress);
            printf("\n");
            printf("DONE! The student city with Id %d is changed from %s to %s",Student_INFO->Id,OldCity,Student_INFO->HomeAddress);
            UpdateMenu(Student_INFO,StudentsTree);
        }
        if(choice2==3){
            int OldClassId = Student_INFO->ClassId;
            printf("Enter the new Class Id: ");
            scanf("%d", &(Student_INFO->ClassId));
            printf("\n");
            printf("DONE! The student ClassId with Id %d is changed from %d to %d", Student_INFO->Id, OldClassId, Student_INFO->ClassId);
            UpdateMenu(Student_INFO,StudentsTree);
        }
        if(choice2==4){
            char*OldDate= strdup(Student_INFO->HomeAddress);
            printf("Enter the new Date of enrollment:");
            scanf("%s",Student_INFO->DataOfEnrollment);
            printf("\n");
            printf("DONE! The student Date of enrollment with Id %d is changed from %s to %s",Student_INFO->Id,OldDate,Student_INFO->DataOfEnrollment);
            UpdateMenu(Student_INFO,StudentsTree);
        }else{
                printf("------Invalid choice------");
        }

    }else if(choice==2){

        Sub_menu(StudentsTree);
    }else{
        printf("------Invalid choice------");
    }

}

void StartFromZero(){
    TNode StudentsTree=MakeEmpty(NULL);
    TNode temp=StudentsTree;
    printf("--At first You have to insert at least one student --\n\n");
    int Id;
    char Name[100];
    char HomeAddress[100];
    int ClassId;
    char DataOfEnrollment[100];

    printf("Enter the student Id: ");
    scanf("%d",&Id);
    printf("\n");

    printf("Enter the student Name: ");
    scanf("%s",Name);
    printf("\n");

    printf("Enter the city name: ");
    scanf("%s",HomeAddress);
    printf("\n");

    printf("Enter the ClassId: ");
    scanf("%d",&ClassId);
    printf("\n");

    printf("Enter the Date Of Enrollment: ");
    scanf("%s",DataOfEnrollment);
    printf("\n");

    temp = Insert(Id, strdup(Name), strdup(HomeAddress), ClassId, strdup(DataOfEnrollment), temp);

    printf("The student is successfully added.\n");

    Sub_menu(temp);
}
