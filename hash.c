#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 256

struct hash
{
    char *entry;
    struct hash *next;
};

int create_hash(char *str, int n)
{
    int hash_value = 0;
    for (int i = 0; i < n; i++)
    {
        hash_value += str[i];
    }

    return hash_value;
}

void insert_hash(struct hash **hash_table_entry, char *str)
{
    if (*hash_table_entry == NULL)
    {
        *hash_table_entry = (struct hash *)malloc(sizeof(struct hash));
        (*hash_table_entry)->entry = str;
        (*hash_table_entry)->next = NULL;
        return;
    }

    struct hash *head = *hash_table_entry;
    struct hash *new_hashed_entry = (struct hash *)malloc(sizeof(struct hash));
    new_hashed_entry->entry = str;
    new_hashed_entry->next = NULL;

    while (head->next)
    {
        head = head->next;
    }

    head->next = new_hashed_entry;
}

void print_hash_table(struct hash *hash_table[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", i);
        if (hash_table[i] == NULL)
        {
            printf("EMPTY \n");
            continue;
        }
        struct hash *head = hash_table[i];

        while (head)
        {
            printf("%s ", head->entry);
            head = head->next;
        }
        printf("\n");
    }
}

int main()
{
    int n;
    printf("Enter the number of keywords to enter\n");
    scanf("%d", &n);

    struct hash *hash_table[CAPACITY];
    memset(hash_table, 0, sizeof(hash_table)); // initialize all pointers to NULL

    for (int i = 0; i < n; i++)
    {
        int size;
        printf("Enter the keyword size \n");
        scanf("%d", &size);
        printf("Enter the keyword \n");
        char *str = (char *)malloc(sizeof(char) * (size + 1));
        getchar(); // Consume the newline character left in the input buffer
        fgets(str, size + 1, stdin);
        int len = strlen(str);
        if (len > 0 && str[len - 1] == '\n')
        {
            str[len - 1] = '\0'; // remove the newline character
        }

        int hash_value = create_hash(str, size);

        insert_hash(&hash_table[hash_value], str);
    }

    print_hash_table(hash_table, CAPACITY);

    return 0;
}
