#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
struct login_states {
    int state ;
    char name[20] ;
};
struct users {
    char code[20] ;
    char name[20] ;
    char password[20] ;
    int type ;
};

struct books {
    int id;
    char name[20];
    char Author[20];
    float Price;
    char publish_year[5] ;
    int no_of_copy ;
    char cat[13];
};

void ShowConsoleCursor (int flag);
void draw_box (int startX, int startY, int w, int h);
int Menu (int menu_size, char **main_list);
void gotoxy (int x, int y);
int get_startX(int w) ;
int get_startY(int h) ;
int get_screen_rows_count () ;
int get_screen_columns_count () ;
void installation();
int  loginMenu () ;
void login_form() ;
void register_form();
void mainMenu () ;
void addbook() ;
void deletebooks() ;
void load_books_data_from_files();
void save_books_data_on_files();
void display_books() ;
void update_session (char * name , int status) ;
void delete_session() ;
int horizontal_button (int menu_size, char **menu) ;
int vertical_button (int menu_size, char **menu) ;

char* find_user_name (char *code ) ;
int find_user (char *code , char *password) ;
char* get_password(int max_count) ;
char* validate_string(int max_count) ;
void print_header(int startX , int startY);
int get_list_menu_width (int menu_size, char **menu) ;



void add_user_to_list(struct users user) ;
void save_users_data_on_files (void) ;
void load_users_data_from_files (void) ;

void add_book_to_list(struct books book) ;

COORD coord = { 0, 0 };		// this is global variable
struct users users[100] ;
int users_size = 0 ;
struct books books[100];
int books_size = 0 ;
//struct save session for login states
struct login_states session = { 0 , ""} ;
void main ()
{
        load_users_data_from_files();
        load_books_data_from_files();

        if (users_size == 0)
            installation();
        while(1){
        system("cls");
        loginMenu() ;
        }
}

void gotoxy (int x, int y)
{
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), coord);
}

void draw_box (int startX, int startY, int w, int h)
{
  int i, j ;

  gotoxy (startX, startY);
  printf ("%c", 201);
  for (i = startX; i < startX + w; i++)
    printf ("%c", 205);
  gotoxy (startX + w, startY);
  printf ("%c", 187);
  for (i = 1; i <= h; i++)
    {
      gotoxy (startX, startY + i);
      printf ("%c", 186);
      for (j = 0; j < w; j++)
	{
	  gotoxy (startX + 1 + j, startY + i + 1);
	  printf ("%c", 32);

	}
      gotoxy (startX + w, startY + i);
      printf ("%c", 186);
    }
  gotoxy (startX, startY + h);
  printf ("%c", 200);
  for (i = startX; i < startX + w; i++)
    printf ("%c", 205);
  gotoxy (startX + w, startY + h);
  printf ("%c", 188);


}

int Menu (int menu_size, char **menu)
{
  HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);

  int i, j , startX , startY , max_weidth , max_height , pos ;

  char ch;

//get max_weidth and  max_height to dynamic draw box
    max_weidth = get_list_menu_width(menu_size , menu );
    max_height = menu_size;
    startX = get_startX(max_weidth);
    startY = get_startY(menu_size);


  //to draw box
  draw_box (startX, startY, max_weidth + 1, max_height + 1);
    //to draw header word
    print_header(startX , startY);
  //hide  Cursor
  ShowConsoleCursor (0);

  //print welcome user
    if (session.state == 3){
            gotoxy(startX , startY+max_height+2);
            printf(" Welcome geust ");
            }
    if (session.state == 1){
            gotoxy(startX , startY+max_height+2);
            printf(" Welcome admin %s" , session.name);
            }
    if (session.state == 2){
            gotoxy(startX , startY+max_height+2);
            printf("Welcome user %s" , session.name);
            }


  pos = 0;

  do
    {

      for (i = 0; i < menu_size; i++)
	{
	  int size_one_menu = strlen (menu[i]);

	  if (pos == i)
	    SetConsoleTextAttribute (hConsole, 240);
	  else
	    SetConsoleTextAttribute (hConsole, 15);
	  gotoxy (startX + 1, startY + 1 + i);
	  printf ("%s", menu[i]);
	  for (j = 0; j < max_weidth - size_one_menu; j++)
	    printf (" ");

	}

      ch = getch ();
      if (ch == 32) ch = getch ();
      if (ch == 72)
	pos = (pos-- == 0) ? max_height - 1 : pos;

      if (ch == 80)
	pos = ++pos % max_height;

    }
  while (ch != 13);
	    SetConsoleTextAttribute (hConsole, 15);

  return pos;

}
void mainMenu (){
    int main_list_size;
    int select;

    char *main_list[6] = {
        "1) Add book",
        "2) Remove book",
        "3) Display books        ",
        "4) Save books",
        "5) Load books",
        "6) Exit",
      };

    main_list_size = sizeof (main_list) / sizeof (main_list[0]);



  while (1){
    select = Menu (main_list_size, main_list);
    system("cls");

    switch (select)
        {
        case 0:
          system("cls");
          addbook();
          break;
        case 1:
          deletebooks();
          break;
        case 2:
          system("cls");
          display_books();
          break;
        case 3:
            if (session.state == 1 ){
                save_books_data_on_files() ;
                printf ("\nData saved successfully\n");
                printf ("press any key to continue \n");
                fflush(stdin);
                getch();
            }else for_admin_only() ;
            break;
        case 4:
            load_books_data_from_files();
            printf ("\nData loaded successfully\n");
            printf ("press any key to continue \n");
            fflush(stdin);
            getch();
            break;

        case 5:
            delete_session();
            save_books_data_on_files() ;
            save_users_data_on_files() ;
            system("cls");

            printf("\nGood bye ! see you soon");
            getch();
            break;



        }
        break ;
  }



}
int for_admin_only(){
    int accept = 0  ;
    int startX , startY ;
    if (session.state == 1 || session.state == 0)
        accept = 1 ;
    else
    {

    char *ok_list[2] = {
        "<OK>",
      };

    startX = get_startX(40) ;
    startY = get_startY(5) ;


    system("cls");

    draw_box(startX , startY , 40 , 5);
    gotoxy(startX +1 , startY+2);
    printf("You do not have permission to be here,");
    gotoxy(startX +1 , startY+3);
    printf("Log in first as admin");
    gotoxy(startX +18 , startY+4);
    horizontal_button(1 , ok_list) ;
    }
    return accept ;
}
int loginMenu (){
    int main_list_size;
    int select;

    char *main_list[3] = {
        "Login                     ",
        "Register",
        "Guest",
      };

    main_list_size = sizeof (main_list) / sizeof (main_list[0]);

    //printf("%d" , session.state);
    //getch();
    switch (session.state)
      {
              case 0 :
                      select = Menu (main_list_size, main_list);
                      system ("cls");

                            switch (select)
                                {
                                case 0:
                                    login_form();
                                  break;
                                case 1:
                                  register_form() ;
                                  break;
                                case 2:
                                  update_session("" , 3);
                                  break;

                                }
                                break ;
                case 1 :
                case 2 :
                case 3 :
                                mainMenu();
                                break ;

      }



}
void register_form()
{
    int startX , startY ;
    int main_list_size ;
    int ok_list_size ;
    char code[30] ;
    char name[30] ;
    char password[30] ;
    int type ;
    char *main_list[2] = {
        "[1]admin ",
        "[2]user  ",
      };
    char *ok_list[2] = {
        "<OK>",
        "<Cancel>",

      };

    struct users user ;

    main_list_size  = sizeof (main_list) / sizeof (main_list[0]);

    ok_list_size    = sizeof (ok_list)  / sizeof (ok_list[0]);

    startX = get_startX(30) ;
    startY = get_startY(3) ;


    system("cls");

    draw_box(startX , startY , 30 , 7);

    gotoxy(startX , startY-1);
    printf("login from ");
    gotoxy(startX+1 , startY+1);
    printf("code     : ");
    gotoxy(startX+1 , startY+2);
    printf("name     : ");
    gotoxy(startX+1 , startY+3);
    printf("Password : ");
    gotoxy(startX+1 , startY+4);
    printf("type     : ");

    ShowConsoleCursor(1);

    gotoxy(startX+11 , startY+1);
    fflush(stdin);
    strcpy(user.code,validate_string(17));
    gotoxy(startX+11 , startY+2);
    fflush(stdin);
    strcpy(user.name,validate_string(17));
    gotoxy(startX+11 , startY+3);
    fflush(stdin);
    strcpy(user.password,get_password(17));
    gotoxy(startX+11 , startY+4);
    fflush(stdin);
    user.type = horizontal_button(main_list_size , main_list) ;
    user.type = user.type == 0  ? 1 : 2 ;
    gotoxy(startX+10 , startY+6);
    if (horizontal_button(ok_list_size , ok_list) == 0){
      //  add user to global array users[]
        add_user_to_list(user) ;
        save_users_data_on_files();
        update_session(user.name , user.type) ;
    }

}
void login_form()
{
    int startX , startY ;
    int ok_list_size , type_r ;
    char code[30];
    char password[30];
    char *ok_list[2] = {
        "<OK>",
        "<Cancel>",
      };

    ok_list_size    = sizeof (ok_list)  / sizeof (ok_list[0]);

    startX = get_startX(30) ;
    startY = get_startY(3) ;


    system("cls");

    draw_box(startX , startY , 30 , 4);

    gotoxy(startX , startY-1);
    printf("login from ");
    gotoxy(startX+1 , startY+1);
    printf("code     : ");
    gotoxy(startX+1 , startY+2);
    printf("Password : ");
    jump :
    ShowConsoleCursor(1);
    gotoxy(startX+11 , startY+1);
    fflush(stdin);
    strcpy(code,validate_string(17));
    gotoxy(startX+11 , startY+2);
    fflush(stdin);
    strcpy(password,get_password(17));
    //getch();
    gotoxy(startX+11 , startY+3);
    if ( horizontal_button(ok_list_size , ok_list) == 0){
      //  find user in global array users[]
        type_r = find_user(code , password );
        if (type_r != 0) {
            update_session( find_user_name(code) , type_r) ;
        }
        else{
            update_session("" , 0) ;
            gotoxy(startX , startY+5);
            printf("-code or password are not correct ");
            goto jump ;
            }
    }
}
void add_user_to_list(struct users user)
{
    users[users_size]=user;
    users_size++;
}
int find_user (char *code , char *password)
{
    //find user and return type of user admin or user
    int i ;
    int succees  = 0  ;
    for(i = 0 ; i < users_size ; i++)
    {
        if (strcmp(users[i].code , code) == 0  && strcmp(users[i].password , password) == 0) {
            succees = users[i].type ;
            break ;
        }

    }

    return succees ;
}
char* find_user_name (char *code )
{
    //find user and return name of user
    int i ;
    char *name   ;
    for(i = 0 ; i < users_size ; i++)
    {
        if (strcmp(users[i].code , code) == 0 ) {
            name = users[i].name ;
            break ;
        }

    }

    return name ;
}
void save_users_data_on_files (void)
{
        FILE * users_file ;
        int i ;

        users_file=fopen("data\\users.dat","wb");

        if (users_size > 0 )
            for (i = 0 ; i < users_size ; i++)
                fwrite(&users[i],sizeof(users[i]),1,users_file);
        fclose(users_file);


}
void load_users_data_from_files (void)
{

        FILE *users_file ;
        struct users user ;
        int i = 0 ;
        users_file=fopen("data\\users.dat","rb");
        if( users_file != NULL)
            while(fread(&user,sizeof(struct users),1,users_file))
                {
                    add_user_to_list(user);
                    i++ ;
                }
        users_size = i ;

}
void display_books (){
    int startX , startY ;

    startX = get_startX(105) ;
    startY = get_startY(books_size+2) ;


    system("cls");
    draw_box(startX , startY , 103  , books_size+2);
    gotoxy(startX + 1  , startY+1) ;
    printf("%-5s%-30s%-15s%-10s%-15s%-15s%-12s" , "id","Name","Author","Prise","publish year","No. of copy ","Category");


    for (int i = 0 ; i < books_size ; i++) {
        gotoxy(startX + 1   , startY+2+i) ;
        printf("%-5d%-30s%-15s%-10.2f%-15s%-15d%-12s" , books[i].id,books[i].name,books[i].Author,books[i].Price,books[i].publish_year,books[i].no_of_copy,books[i].cat);
    }
    getch();
}
void addbook (){
    int startX , startY ;
    int main_list_size ;
    int ok_list_size ;
    int cat_list_size ;
    int select ;
    char *main_list[2] = {
        "[1]admin ",
        "[2]user  ",
      };
    char *ok_list[2] = {
        "<OK>",
        "<Cancel>",

      };
    char *catagories[6]= {"Computer","Electronics","Electrical","Civil","Mechnnical","Architecture"};

    struct books book ;

    if (for_admin_only() == 0) goto end ;

    main_list_size  = sizeof (main_list) / sizeof (main_list[0]);

    ok_list_size    = sizeof (ok_list)  / sizeof (ok_list[0]);

    cat_list_size    = sizeof (catagories)  / sizeof (catagories[0]);

    startX = get_startX(30) ;
    startY = get_startY(3) ;


    system("cls");

    draw_box(startX , startY , 50 , 14);

    gotoxy(startX , startY-1);
    printf("Add new book ");
    gotoxy(startX+1 , startY+1);
    printf("Id           :");
    gotoxy(startX+1 , startY+2);
    printf("Name         :");
    gotoxy(startX+1 , startY+3);
    printf("Author       :");
    gotoxy(startX+1 , startY+4);
    printf("Price        :");
    gotoxy(startX+1 , startY+5);
    printf("Publish_year :");
    gotoxy(startX+1 , startY+6);
    printf("No_of_copy   :");
    gotoxy(startX+1 , startY+7);
    printf("Category     :");

    ShowConsoleCursor(1);

    gotoxy(startX+15 , startY+1);
    fflush(stdin);
    scanf("%d" , &book.id);
//    strcpy(user.code,validate_string(17));
    gotoxy(startX+15 , startY+2);
    fflush(stdin);
    strcpy(book.name,validate_string(35));

    gotoxy(startX+15 , startY+3);
    fflush(stdin);
    strcpy(book.Author,validate_string(35));

    gotoxy(startX+15 , startY+4);
    fflush(stdin);
    scanf("%f" , &book.Price);

    gotoxy(startX+15 , startY+5);
    fflush(stdin);
    strcpy(book.publish_year,validate_string(4));

    gotoxy(startX+15 , startY+6);
    fflush(stdin);
    scanf("%d" , &book.no_of_copy);

    gotoxy(startX+15 , startY+6);
    fflush(stdin);

    select = vertical_button(cat_list_size , catagories  ) ;
    strcpy(book.cat,catagories[select]);



    gotoxy(startX+20 , startY+13);

    if (horizontal_button(ok_list_size , ok_list) == 0){
      //  add book to global array book[]
          add_book_to_list(book) ;
      //  save_users_data_on_files();
    }
end :
    putch("");
}

void deletebooks()
{
    FILE *ft,*fp;
    int startX , startY ;
    startX = get_startX(35) ;
    startY = get_startY(5) ;

    if (for_admin_only() == 0) goto end ;

    system("cls");


    int d,findBook , i ;
    char another='y';
    while(another=='y')
        {
        findBook = 0 ;
        system("cls");
        draw_box(startX , startY , 35 , 8);

        gotoxy(startX+1,startY+1);
        printf("Enter the Book ID to  delete:");
        scanf("%d",&d);
        for (i = 0 ; i < books_size ; i++) {

            if(books[i].id==d)
            {
                gotoxy(startX+1,startY+2);
                printf("The book record is available");
                gotoxy(startX+1,startY+3);
                printf("books name is %s",books[i].name);
                gotoxy(startX+1,startY+4);
                printf("Do you want to delete it?(Y/N):");
                if(getch() == 121 )
                {
                    for (int j = i ; j < books_size-1 ; j++ ) books[j] = books[j+1]  ;
                    books_size-- ;

                    gotoxy(startX+1,startY+5);
                    printf("The record is sucessfully deleted");
                    findBook = 1 ;
                    break ;
                }
            }

        }


        if(findBook == 0)
        {
            gotoxy(startX+1,startY+6);
            printf("No record is found");
            getch();
        }
        gotoxy(startX+1,startY+7);
        printf("Delete another record?(Y/N)");
        fflush(stdin);
        another=getch();
    }

    end :
        putch("");
}


void add_book_to_list(struct books book)
{
    books[books_size]=book;
    books_size++;
}
void save_books_data_on_files (void)
{
        FILE * books_file ;
        int i ;

        if (for_admin_only() == 0) goto end ;

        books_file=fopen("data\\books.dat","wb");

        if (books_size > 0 )
            for (i = 0 ; i < books_size ; i++)
                fwrite(&books[i],sizeof(books[i]),1,books_file);
        fclose(books_file);

        end :
            putch("");

}
void load_books_data_from_files (void)
{
        FILE *books_file ;
        struct books book ;
        int i = 0 ;


        books_file=fopen("data\\books.dat","rb");
        if( books_file != NULL)
            while(fread(&book,sizeof(struct books),1,books_file))
                {
                    add_book_to_list(book);
                    i++ ;
                }
        books_size = i ;
}

int horizontal_button (int menu_size, char **menu)
{
  HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);

  int i, j , startX , startY , max_weidth , max_height , pos , sum;
  int main_list_size ;
  char ch;

//get max_weidth and  max_height to dynamic draw box
    max_weidth = get_list_menu_width(menu_size , menu );
    max_height = menu_size;
    startX = get_startX(max_weidth);
    startY = get_startY(menu_size);


    ShowConsoleCursor (0);
    pos = 0;

  do
    {

        sum = 0 ;
    for (i = 0; i < menu_size; i++)
	{
	  int size_one_menu = strlen (menu[i]);
        sum+=size_one_menu ;
	  if (pos == i)
	    SetConsoleTextAttribute (hConsole, 240);
	  else
	    SetConsoleTextAttribute (hConsole, 15);
	  //gotoxy (startX + i*10 + 1  , startY +  max_height + 1);
	  printf ("%s", menu[i]);
        }
     for (j = 0 ; j<sum ; j++) printf("\b") ;

      ch = getch ();
      if (ch == 32) ch = getch ();
      if (ch == 75)
	pos = (pos-- == 0) ? max_height - 1 : pos;

      if (ch == 77)
	pos = ++pos % max_height;

    }
  while (ch != 13);
	    SetConsoleTextAttribute (hConsole, 15);

  return pos;

}
int vertical_button (int menu_size, char **menu)
{
  HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);

  int i, j , startX , startY , max_weidth , max_height , pos , sum;
  int main_list_size ;
  char ch;

//get max_weidth and  max_height to dynamic draw box
    max_weidth = get_list_menu_width(menu_size , menu );
    max_height = menu_size;
    startX = get_startX(max_weidth);
    startY = get_startY(menu_size);


    ShowConsoleCursor (0);
    pos = 0;

  do
    {

        sum = 0 ;
    for (i = 0; i < menu_size; i++)
	{
	  int size_one_menu = strlen (menu[i]);
        sum+=size_one_menu ;
	  if (pos == i)
	    SetConsoleTextAttribute (hConsole, 240);
	  else
	    SetConsoleTextAttribute (hConsole, 15);
	  gotoxy (startX +6 , startY +  8 + i);
	  printf ("%s", menu[i]);
        }
     for (j = 0 ; j<sum ; j++) printf("\b") ;

      ch = getch ();
      if (ch == 32) ch = getch ();
      if (ch == 72)
	pos = (pos-- == 0) ? max_height - 1 : pos;

      if (ch == 80)
	pos = ++pos % max_height;

    }
  while (ch != 13);
	    SetConsoleTextAttribute (hConsole, 15);

  return pos;

}
char* get_password(int max_count)
{
    char *password, c;
    int index = 0 , i;

    for (i = 0 ; i<max_count ; i++) { putch(' ');  }
    for (i = 0 ; i<max_count ; i++) { putch('\b'); }
    password[0] = '\0';

       while((c = getch()) != 13){
           if(index < 0)
               index = 0;
           /* 8 is ASCII value of BACKSPACE character */
           if(c == 8){
                if (index != 0){
                        putch('\b');
                        putch(' ');
                        putch('\b');
                        password[index] = '\0';
                        index--;


                 }
           continue;
           }
           if (index < max_count)
                {
                    password[index++] = c;
                    password[index] = '\0' ;
                    putch('*');

                }
       }

    return password ;
}
char* validate_string(int max_count)
{
    char *string, c;
    int index = 0 , i ;
    for (i = 0 ; i<max_count ; i++) { putch(' ');  }
    for (i = 0 ; i<max_count ; i++) { putch('\b'); }
    string[0] = '\0';
    fflush(stdin);
       while((c = getch()) != 13){
           if(index < 0)
               index = 0;
           /* 8 is ASCII value of BACKSPACE character */
           if(c == 8){
                if (index != 0){
                        putch('\b');
                        putch(' ');
                        putch('\b');
                        string[index] = '\0' ;
                        index--;


                 }
           continue;
           }
           if (index < max_count)
                {
                    string[index++] = c;
                    string[index] = '\0' ;
                    putch(c);

                }
       }

    return string ;
}
void update_session (char * name , int status){


       session.state = status ;
       strcpy(session.name , name);


}
void delete_session (){
       strcpy(session.name , "");
       session.state = 0 ;
}
void installation() {
    printf("Wellcome in library program \n");
    printf("This program used to manage library system  \n");
    printf("This program was developed as a graduation project for a C programming language course by õENG. Youssef Shawky \n");
    printf("This program was developed by  Eslam Eldeep \n");
    printf("                               Ahmed Aladaash \n");
    printf("                               khaled A.Ahmed \n");
    printf("                               Mohamed Eltoki \n");
    printf("You are in installation stage \n");
    printf("Sign up As admin to start use the system\n\n\n");
    printf("-Press any letter to continue ");
    getch();
    register_form();
}
void print_header (int startX , int startY){

    startX = startX - 20 ;
    startY = startY-6 ;
  gotoxy (startX,startY);
  printf("  _        _____   ____    _____               _____   __     __");
  gotoxy (startX,startY+1);
  printf(" | |      |_   _| |  _ \\  |  __ \\      \/\\     |  __ \\  \\ \\   \/ \/");
  gotoxy (startX,startY+2);
  printf(" | |        | |   | |_) | | |__) |    \/  \\    | |__) |  \\ \\_\/ \/ ");
  gotoxy (startX,startY+3);
  printf(" | |        | |   |  _ <  |  _  \/    \/ \/\\ \\   |  _  \/    \\   \/  ");
  gotoxy (startX,startY+4);
  printf(" | |____   _| |_  | |_) | | | \\ \\   \/ ____ \\  | | \\ \\     | |   ");
  gotoxy (startX,startY+5);
  printf(" |______| |_____| |____\/  |_|  \\_\\ \/_\/    \\_\\ |_|  \\_\\    |_|   ");
  gotoxy (0,0);


}
int get_list_menu_width (int menu_size, char **menu){
    int max_weidth ;
    int i ;
    for (i = 0; i < menu_size; i++)
    {
      if (i == 0)
	max_weidth = strlen (menu[i]);
      if (strlen (menu[i]) > max_weidth)
	max_weidth = strlen (menu[i]);


    }
    return max_weidth ;
}
int get_startX(int w)
{
    int startX ;

    startX = (get_screen_columns_count() - w)/2 ;

    return startX ;

}
int get_startY(int h)
{
    int startY ;

    startY = (get_screen_rows_count() - h)/2 ;

    return startY ;

}
void ShowConsoleCursor (int flag)
{
  HANDLE out = GetStdHandle (STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo (out, &cursorInfo);
  cursorInfo.bVisible = flag;
  SetConsoleCursorInfo (out, &cursorInfo);
}
int get_screen_columns_count (){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns ;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    return columns ;
}

int get_screen_rows_count (){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows ;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return rows ;
}

