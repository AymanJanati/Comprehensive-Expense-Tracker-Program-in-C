#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
typedef struct {int day;
                int month;
                int year;} date;
typedef struct {char name[50];
                float price;
                int id;
                char category[50];
                date when;} expense;
typedef struct {char catname[50];
                int how_many_ex; } category;
expense mylist[200];
category cattab[100] = {
    {"health", 0},    // Initialize cattab[0] with "health"
    {"groceries", 0}  // Initialize cattab[1] with "groceries"
    // You can add more initializations here if needed
};
int k=0,catsize=2; //k is size of mylist and catsize is size of cattab
// Function prototypes
void load_expenses_from_file(const char *filename);
void save_expenses_to_file(const char *filename);
//////////adding new categories//////////
void add_category()
{if(catsize<200)
{printf("enter the category name:");
scanf("%[^\n]",cattab[catsize].catname);
cattab[catsize].how_many_ex=0;
catsize++;}
else
printf("you can't add more categories :(\n");
}
//////////adding new expenses//////////
void add_expense() {
    int i, keep = 1, ipick;
    while (keep == 1 && k < 200) {
        printf("Enter the name: ");
        scanf(" %[^\n]", mylist[k].name); // Taking in count the case of spaces inside the name

        printf("\nEnter the cost: ");
        scanf("%f", &mylist[k].price);
        mylist[k].id = k + 1;

        printf("\nSelect expense category:\n");
        for (i = 0; i < catsize; i++) {
            printf("%d_%s\n", i + 1, cattab[i].catname);
        }
        printf("999_Add new category\n--->");
        scanf("%d", &ipick);
        while (getchar() != '\n');
        if (ipick == 999) {
            add_category();
            strcpy(mylist[k].category, cattab[catsize - 1].catname); // Assign the newly added category
        } else if (ipick > 0 && ipick <= catsize) {
            strcpy(mylist[k].category, cattab[ipick - 1].catname);
        } else {
            printf("Invalid category selection. Setting category to 'Uncategorized'.\n");
            strcpy(mylist[k].category, "Uncategorized");
        }

        printf("\nEnter the date of this expense (day month year): ");
        scanf("%d %d %d", &mylist[k].when.day, &mylist[k].when.month, &mylist[k].when.year);

        k++;

        printf("-------------------\nDo you want to:\n0. Exit.\n1. Enter another expense.\n--->");
        scanf("%d", &keep);
        printf("\n");
    }

    if (k >= 200) {
        printf("===== You ran out of space =====\n");
    }
}
//////////remove an expense//////////
void remove_expense(int ind)
{int i;
for (i=ind;i<k-1;i++)
{
 mylist[i]=mylist[i+1];
 mylist[i].id=i+1;
}
k--;
}
//////////view one expense with the given indice//////////
void view_one_exp(int ind) {
      printf("------------------------------\n");
        printf("+>Expense name: %s\n", mylist[ind].name);
        printf("+>ID: %d\n", mylist[ind].id);
        printf("+>It Costed: %.2f\n", mylist[ind].price);
        printf("+>Date of adding: %d-%d-%d\n", mylist[ind].when.day, mylist[ind].when.month, mylist[ind].when.year);
        printf("+>Category of expense: %s\n", mylist[ind].category);
      printf("------------------------------\n");
}
//////////view all stored expenses//////////
void view_expenses() {
    int i;
    printf("====Your Stored Expenses Are====\n");
    for(i = 0; i < k; i++) {
        printf("+>Expense name: %s\n", mylist[i].name);
        printf("+>ID: %d\n", mylist[i].id);
        printf("+>It Costed: %.2f\n", mylist[i].price);
        printf("+>Date of adding: %d-%d-%d\n", mylist[i].when.day, mylist[i].when.month, mylist[i].when.year);
        printf("+>Category of expense: %s\n", mylist[i].category);
        printf("------------------------------\n");
    }
    if(k==0) printf("No Expenses stored at the moment.\n");
}
//////////getting how much money you spent on a specific category, we will need this one in the report function//////////
float i_spent(char cat[50])
{int i;
float res=0;
for(i=0;i<k;i++)
{if (strcmp(mylist[i].category,cat)==0) res+=mylist[i].price;
}
return res;}
//////////get a well structured report about your expenses//////////
void get_report()
{int i;
float costotal=0,percentage,cost;
date min=mylist[0].when,max=mylist[0].when;
//getting the minimun date and lastest date
for(i=1;i<k;i++)
{if((max.year<mylist[i].when.year) || (max.year==mylist[i].when.year && max.month<mylist[i].when.month) || (max.year==mylist[i].when.year && max.month==mylist[i].when.month && max.day<mylist[i].when.day)) max=mylist[i].when;
if ((min.year>mylist[i].when.year) || (min.year==mylist[i].when.year && min.month>mylist[i].when.month) || (min.year==mylist[i].when.year && min.month==mylist[i].when.month && min.day>mylist[i].when.day)) min=mylist[i].when;}
//getting the total amount of money spent
for(i=0;i<k;i++)
{costotal+=mylist[i].price;}
//report reveal
printf("in the duration between %d-%d-%d and %d-%d-%d you spent a total cost of %.2f in %d expenses, and speaking of their categories the costs were split between the categories like this:\n",min.day,min.month,min.year,max.day,max.month,max.year,costotal,k);

//getting the amount of money spent in every category and it's percentage from the total cost
for(i=0;i<catsize;i++)
{cost=i_spent(cattab[i].catname);
if(costotal==0) percentage=0; //prevent division by zero error, in case the cost of some category=0
else
percentage=(cost*100)/costotal;
printf("the category %s costed you %.2f and it is around %.2f%% of the total budget you put in the expenses in this duration\n",cattab[i].catname,cost,percentage);}
}
//////////making a date based search on the expenses//////////
void date_based_search()
{int i;
 bool found=false;
date srch={0,0,0};
while((srch.day==0 || srch.day>31) || (srch.month==0 || srch.month>12) || (srch.year==0 || srch.year<2000)) //checking if the input was made succesfully and correctly(PS: we can add more advanced checking for the correct input later)
{printf("Enter the date to search dd-mm-yyyy:\n");
printf("-->");
scanf("%d%d%d",&srch.day,&srch.month,&srch.year);}
printf("\n+>>>The Expenses found in the date you given are:\n");
for(i=0;i<k;i++)
{
 if (mylist[i].when.day==srch.day && mylist[i].when.month==srch.month && mylist[i].when.year==srch.year)
 {view_one_exp(i);
  found=true;} //making found to true to set that there is an expense that will be shown, we will need this one in the next lines
}
if(!found) printf("Sorry, Can't find an expense in the date you gave.\n");//if no expense is found with the given date
}
//////////making a category based search on the expenses//////////
void category_based_search()
{int i,ipick;
bool found=false;
printf("select expense category to search: ");
        do {for(i=0;i<catsize;i++)
        {printf("\n%d_%s",i+1,cattab[i].catname);
        }
        printf("\n--->");
        scanf("%d",&ipick);} while(ipick>catsize || ipick<=0);
        ipick--; //decrementation for making it in the array's indice logic (starting from 0)
        printf("\n+>>>The expenses found using the given category are:\n");
        for(i=0;i<k;i++)
        {if(strcmp(cattab[ipick].catname,mylist[i].category)==0) {view_one_exp(i); found=true;}}
        if(!found) printf("Sorry, can't find an expense with the given category.\n");
}
//////////making a name based search on the expenses//////////
void name_based_search()
{int i;
bool found=false;
char givename[50];
printf("Enter the name you want to search for\n");
printf("\n --->");
scanf(" %[^\n]",givename); printf("\n");
for(i=0;i<k;i++)
{if (stricmp(mylist[i].name,givename)==0){found=true; break;}
}
if(found)
{ printf("Expenses found with the name given are:\n");
    for(i=0;i<k;i++)
    {if (stricmp(givename,mylist[i].name)==0) view_one_exp(i);}
}
else
{printf("Sorry, Can't find an expense with the given name.\n");}
}
////////id based search/////////
void id_based_search()
{int indice;
do{printf("Enter the id of the expense you want to search for:\n");
   printf("(Enter 999 if you want to exit :) )\n");
   printf("--->");
   scanf("%d",&indice);
   printf("\n");
   if(indice==999) break;
   } while(indice>k || indice<=0);
if(indice!=999) {indice--;
                 view_one_exp(indice);}
}
//////////making a price based search on the expenses showing expenses that have a price equal with the given price or superior//////////
void price_supp_search()
{int i;
float price=-1;
bool found=false;
while(price<0)
{printf("Enter the price to search for:\n");
 printf("--->");
 scanf("%f",&price);
 printf("\n");
}
printf("The expenses found with a price equal or superior to the given price are:\n");
for(i=0;i<k;i++)
{
    if(mylist[i].price>=price) {view_one_exp(i); found=true;}
}
if(!found) printf("Sorry, no expenses found\n");
}
//////////making a price based search on the expenses showing expenses that have a price equal with the given price or inferior//////////
void price_inf_search()
{int i;
float price=-1;
bool found=false;
while(price<0)
{printf("Enter the price to search for:\n");
 printf("--->");
 scanf("%f",&price);
 printf("\n");
}
printf("The expenses found with a price equal or inferior to the given price are:\n");
for(i=0;i<k;i++)
{
    if(mylist[i].price<=price){view_one_exp(i); found=true;}
}
if(!found) printf("Sorry, no expenses found\n");
}
//////////clear all expenses, returning the list to zero expenses//////////
void clear_expenses()
{k=0;}
int main() {
    load_expenses_from_file("expenses.dat");

    int choice;

    do {
        printf("\n====== Expense Tracking System ======\n");
        printf("1. Add Expense\n");
        printf("2. Remove Expense\n");
        printf("3. View All Expenses\n");
        printf("4. Search Expenses\n");
        printf("5. Generate Expense Report\n");
        printf("6. Clear All Expenses\n");
        printf("7. Exit\n");
        printf("=====================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_expense();
                break;
            case 2: {
                int id;
                printf("Enter ID of the expense to remove: ");
                scanf("%d", &id);
                id--;
                remove_expense(id);
                printf("expense deleted, IDs updated.\n");
                break;
            }
            case 3:
                view_expenses();
                break;
            case 4: {
                int searchChoice;
                printf("\n====== Search Options ======\n");
                printf("1. Date Based\n");
                printf("2. Category Based\n");
                printf("3. Name Based\n");
                printf("4. ID Based\n");
                printf("5. Price Equal or Superior\n");
                printf("6. Price Equal or Inferior\n");
                printf("7. Back to Main Menu\n");
                printf("============================\n");
                printf("Enter your choice: ");
                scanf("%d", &searchChoice);

                switch (searchChoice) {
                    case 1:
                        date_based_search();
                        break;
                    case 2:
                        category_based_search();
                        break;
                    case 3:
                        name_based_search();
                        break;
                    case 4:
                        id_based_search();
                        break;
                    case 5:
                        price_supp_search();
                        break;
                    case 6:
                        price_inf_search();
                        break;
                    case 7:
                        // Return to main menu
                        break;
                    default:
                        printf("Invalid choice\n");
                }
                break;
            }
            case 5:
                get_report();
                break;
            case 6:
                clear_expenses();
                printf("All expenses cleared.\n");
                break;
            case 7:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    // Save expenses to file before exiting
    if (choice==1 || choice==2 || choice==6){save_expenses_to_file("expenses.dat");}
    } while (choice != 7);

    // Save expenses to file before exiting
    save_expenses_to_file("expenses.dat");

    return 0;
}
void load_expenses_from_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("No existing expenses file found. Starting with an empty expense list.\n");
        return;
    }

    // Read expenses from the file
    fread(&k, sizeof(int), 1, file); // Read the number of expenses
    fread(mylist, sizeof(expense), k, file); // Read the expenses array
    fread(&catsize, sizeof(int), 1, file); // Read the number of categories
    fread(cattab, sizeof(category), catsize, file); // Read the categories array

    fclose(file);
    printf("Expenses loaded from file successfully.\n");
}

void save_expenses_to_file(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Unable to create/open the file for writing.\n");
        return;
    }

    // Write the number of expenses, expenses array, number of categories, and categories array to the file
    fwrite(&k, sizeof(int), 1, file);
    fwrite(mylist, sizeof(expense), k, file);
    fwrite(&catsize, sizeof(int), 1, file);
    fwrite(cattab, sizeof(category), catsize, file);

    fclose(file);
    printf("=>NOTE:Expenses list file updated successfully.\n");
}
