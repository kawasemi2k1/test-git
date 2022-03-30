#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <stdlib.h>
#define INITIAL_SIZE 100
#define INCREMENTAL_SIZE 10
#define MaxName 80

typedef struct
{
  long number;
  char name[MaxName];
} PhoneEntry;

typedef struct
{
  PhoneEntry *entries;
  int total;
  int size;
} PhoneBook;

PhoneBook createPhoneBook()
{
  PhoneBook book;
  //book=(PhoneBook)malloc(sizeof(PhoneBook));
  book.entries = (PhoneEntry *)malloc(INITIAL_SIZE * sizeof(PhoneEntry));
  book.size = INITIAL_SIZE;
  book.total = 0;
  return book;
}

void dropPhoneBook(PhoneBook *book)
{
  free(book->entries);
}

void addPhoneNumber(char *name, long number, PhoneBook *book)
{
  int i, done;
  done = 0;
  for (i = 0; i < book->total; i++)
  {
    if (strcmp(name, book->entries[i].name) == 0)
    {
      book->entries[i].number = number;
      done = 1;
    }
  }
  if (done == 0 && book->total < book->size)
  {
    book->entries[book->total].number = number;
    strcpy(book->entries[book->total].name, name);
    book->total++;
  }
  else if (book->total == book->size)
  {
    book->entries = (PhoneEntry *)realloc(book->entries, (book->size + INCREMENTAL_SIZE) * sizeof(PhoneEntry));

    book->size = book->size + INCREMENTAL_SIZE;
    book->entries[book->total].number = number;
    strcpy(book->entries[book->total].name, name);
    book->total++;
  }
}
char *getPhoneNumber(long number, PhoneBook *book)
{
  int i;
  for (i = 0; i < book->total; i++)
  {
    if (number == book->entries[i].number)
      return book->entries[i].name;
  }
  return NULL;
}

int main()
{
  int i;
  PhoneBook book;
  PhoneEntry *entry;
  book = createPhoneBook();
  //printf("Here\n");
  addPhoneNumber("Huy", 12347, &book);
  addPhoneNumber("Thuy", 9895125, &book);
  addPhoneNumber("Tuyen", 147852, &book);
  addPhoneNumber("Hai", 1234567, &book);
  addPhoneNumber("Ha", 369123, &book);
  for (i = 0; i < book.total; i++)
    printf("%s: %ld\n", book.entries[i].name, book.entries[i].number);
  if (getPhoneNumber(9895125, &book) == NULL)
    printf("Khong co\n");
  else
    printf("So 9895125 la cua: %s\n", getPhoneNumber(9895125, &book));
}
