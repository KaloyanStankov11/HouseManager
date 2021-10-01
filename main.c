#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *fp;
const int floorsNum=5;
const int apartementsOnFloor=3;
struct Apartment{
    int number;
    int rooms;
    int adults;
    int children;
    char surname[20];
    char date[11];
    double rent;
}blankApart;
typedef struct Apartment apart;

struct List
{
    apart apartment;
    struct List* next;
};
typedef struct List List;


List *new_item(int apartNum);
List *createList(int size);
void print(List* root);
void free_list(List* root);

void moveIn(int floor, int apartNumber, List *root);
void moveOut(int apartNumber, List *root);
int feeCalc(List *root);
int elevatorPos(List *root);
int isFree(int apartNumber, List *root);
int validateDate(char date[11]){
    if (date[0]<'0' || date[0]>3) return 0;
    if (date[1])<0 || date[1]>9) return 0;
    if(date[3]<0 || date[3]>1) return 0;
    if ((date[3]==0)&&(date[4]<0||date[4]>9)) return 0;
    if ((date[3]==1)&&(date[4]<0 || date[4]>2)) return 0;

}
int main()
{
    int floor, app, choosen;
    /*apart app1;
    if((fp=fopen("Inhabitants.bin", "wb"))==NULL){
        printf("Cannot open file!");
        exit(1);
    }
    for(int i=1; i<=15; i++){
        app1.number=i;
        app1.rooms=3;
        app1.adults=0;
        app1.children=0;
        strcpy(app1.surname, "empty");
        strcpy(app1.date, "empty");
        app1.rent=0;
        fwrite(&app1, sizeof(apart), 1, fp);
    }
    fclose(fp);*/
    List *root=NULL;
    if((fp=fopen("Inhabitants.bin", "rb"))==NULL){
        printf("Cannot open file!");
        exit(1);
    }
    root=createList(floorsNum*apartementsOnFloor);
    fclose(fp);
    //print(root);

    do{
        printf("1. Move in\n2. Move out\n3. Calculate entrance fee\n4. Find the optimal position for the elevator\n5. Exit\nChoose a command: ");
        scanf("%d", &choosen);
        if(choosen==1){
            system("cls");
            printf("Floor number: "); scanf("%d", &floor);
            do{
                printf("Apartment number: "); scanf("%d", &app);
            }while((3*floor-app<0)||(3*floor-app>2));
            if(!isFree(app, root)){
                printf("Apartment is not free!\n");
            }
            else moveIn(floor, app, root);
        }
        else if(choosen==2){
            system("cls");
            printf("Apartment number: "); scanf("%d", &app);
            moveOut(app, root);
        }
        else if(choosen==3){
            system("cls");
            printf("Entrance fee is: %dlv \n", feeCalc(root));
        }
        else if(choosen==4){
            system("cls");
            printf("Best floor for the elevator is: %d floor \n", elevatorPos(root));
        }
    }while(choosen!=5);

    if (remove("Inhabitants.bin") != 0)
        printf("Unable to delete the file");

    if((fp=fopen("Inhabitants.bin", "wb"))==NULL){
        printf("cannot open file!\n"); exit(1);
    }
    List *curr_item=root;
    while(curr_item != NULL)
    {
        if(fwrite(&curr_item->apartment, sizeof(apart), 1, fp)!=1){
            printf("Error saving data!"); exit(1);
        }
        curr_item=curr_item->next;
    }
    fclose(fp);
    free_list(root);
    return 0;
}

void moveIn(int floor, int apartNumber, List *root){
    system("cls");
    List *curr_item=root;
    while(curr_item != NULL)
    {
        if(curr_item->apartment.number==apartNumber){
            do{
                printf("Enter rooms number: "); scanf("%d", &curr_item->apartment.rooms);
            }while(curr_item->apartment.rooms<1);
            do{
                printf("Enter adults number: "); scanf("%d", &curr_item->apartment.adults);
            }while(curr_item->apartment.adults<1);
            do{
                printf("Enter children number: "); scanf("%d", &curr_item->apartment.children);
            }while(curr_item->apartment.children<0);
            printf("Enter surname: "); scanf("%s", curr_item->apartment.surname);
            printf("Enter date: "); scanf("%s", curr_item->apartment.date);
            do{
                printf("Enter monthly rent: "); scanf("%lf", &curr_item->apartment.rent);
            }while(curr_item->apartment.rent<=0);
        }
        curr_item=curr_item->next;
    }
    system("cls");
    printf("Family accommodated successfully!\n");
}

void moveOut(int apartNumber, List *root){
    system("cls");
    List *curr_item=root;
    while(curr_item != NULL)
    {
        if(curr_item->apartment.number==apartNumber){
            curr_item->apartment.adults=0;
            curr_item->apartment.children=0;
            strcpy(curr_item->apartment.surname, "empty");
            strcpy(curr_item->apartment.date, "empty");
            curr_item->apartment.rent=0;
        }
        curr_item=curr_item->next;
    }
    printf("Family moved out successfully!\n");
}

int elevatorPos(List *root){
    List *curr_item=root;
    List *start_item=root;
    for(int i=1; i<=3; i++) start_item=start_item->next;
    int peoplenum1=0, peoplenum2=0, bestfloor=2, mid=2;
    for(int i=1; i<=3; i++){
        for(int j=1; j<=3; j++){
            peoplenum1=peoplenum1+curr_item->apartment.adults+curr_item->apartment.children;
            curr_item=curr_item->next;
        }
    }

    while(start_item->apartment.number<10)
    {
        curr_item=start_item;
        mid++;
        for(int i=1; i<=3; i++){
            for(int j=1; j<=3; j++){
                peoplenum2=peoplenum2+curr_item->apartment.adults+curr_item->apartment.children;
                curr_item=curr_item->next;
            }
        }
        if(peoplenum2>peoplenum1){
            peoplenum1=peoplenum2;
            bestfloor=mid;
        }
        for(int i=1; i<=3; i++) start_item=start_item->next;
        peoplenum2=0;
    }
    return bestfloor;
}

int feeCalc(List *root){
    int fee=0;
    List *curr_item=root;
    while(curr_item != NULL)
    {
        if((curr_item->apartment.number>=1)&&(curr_item->apartment.number<=6)) fee=fee+3*curr_item->apartment.adults+curr_item->apartment.children;
        else fee=fee+5*curr_item->apartment.adults+3*curr_item->apartment.children;
        curr_item=curr_item->next;
    }
    return fee;
}

int isFree(int apartNumber, List *root){
    List *curr_item=root;
    int free=0;
    while(curr_item != NULL)
    {
        if(curr_item->apartment.number==apartNumber){
            if(curr_item->apartment.adults==0) free=1;
        }
        curr_item=curr_item->next;
    }
    return free;
}

List *new_item(int apartNum)
{
    apart newApart;
    List *result=NULL;
    result = (List *)malloc(sizeof(List));
    fread(&newApart, sizeof(apart), 1, fp);
    result->apartment=newApart;
    result->next = NULL;
    return result;
}

List *createList(int size)
{
    List *res_root = NULL;
    List *end_item = NULL;
    int i;
    for(i=0; i<size; i++)
    {
        if(res_root == NULL)
        {
            res_root = new_item(i+1);
            end_item = res_root;
        }
        else
        {
            end_item->next = new_item(i+1);
            end_item = end_item->next;
        }
    }
    return res_root;
}

void print(List* root)
{
    List* curr_item = root;
    while(curr_item != NULL)
    {
        printf("Item has value %d\n",curr_item->apartment.number);
        printf("Item has value %d\n",curr_item->apartment.adults);
        curr_item=curr_item->next;
    }
}

void free_list(List* root)
{
    List* curr_item = root;
    while(root != NULL)
    {
        root = root->next;
        free(curr_item);
        curr_item= root;
    }
}
