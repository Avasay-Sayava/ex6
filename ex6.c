// include the header file
#include "ex6.h"

// the head of the two-way linked list which stores all the pokedexes
OwnerNode* ownersHead = NULL;
// the tail of the two-way linked list
OwnerNode* ownersTail = NULL;
// the variable that stores the size of the two-way linked list
int ownerCount = 0;

int main()
{
    // get into the main menu
    mainMenu();
    // after the user exited the program, free all the allocated data
    while (ownersHead != NULL)
        freeOwnerNode(ownersHead);
    return 0;
}

char* trim(char* str)
{
    // if the string is empty, return the string
    if (str[0] == '\0')
        return str;

    int start = 0;
    // for each space in the start of the string, update the start of the string accordingly
    while (isspace(str[start]))
        start++;

    // get the length of the str
    const int len = strlen(str); // NOLINT(*-narrowing-conversions)
    // for each space in the end of the string, delete it (switch with NULL)
    for (int i = len - 1; isspace(str[i]) && i > start; --i)
        str[i] = '\0';

    // copy the trimmed string into a temp variable
    char* tmp = malloc((strlen(str + start) + 1) * sizeof(char));
    // if the allocation has failed, exit the program.
    if (tmp == NULL)
    {
        printf("Memory reallocation failed. (trim: tmp)\n");
        exit(1);
    }
    strcpy(tmp, str + start);
    // copy the trimmed string back to the original string, but add it in its start.
    strcpy(str, tmp);
    // free the temporary allocated string
    free(tmp);

    // cut the original size of the string to fit exactly the new trimmed string.
    str = realloc(str, (strlen(str) + 1) * sizeof(char));
    // if the allocation failed, exit the program
    if (str == NULL)
    {
        printf("Memory reallocation failed. (trim: str)\n");
        exit(1);
    }

    // return the trimmed string.
    return str;
}

int readIntSafe(const char* prompt)
{
    // the buffer for reading not just one at a time
    char buffer[DEFAULT_CAPACITY];
    // the value to scan
    int value = 0;
    // if the scan was done successfully
    bool success = 0;

    // while the scan has not been completed
    while (!success)
    {
        // prompt the prompt :)
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0)
        {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char* endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0')
        {
            printf("Invalid input.\n");
        }
        else
        {
            // We got a valid integer
            success = true;
        }
    }
    // return the scanned valid input
    return value;
}

const char* getTypeName(const PokemonType type)
{
    // return the name of a PokemonType
    switch (type)
    {
    case GRASS:
        return "GRASS";
    case FIRE:
        return "FIRE";
    case WATER:
        return "WATER";
    case BUG:
        return "BUG";
    case NORMAL:
        return "NORMAL";
    case POISON:
        return "POISON";
    case ELECTRIC:
        return "ELECTRIC";
    case GROUND:
        return "GROUND";
    case FAIRY:
        return "FAIRY";
    case FIGHTING:
        return "FIGHTING";
    case PSYCHIC:
        return "PSYCHIC";
    case ROCK:
        return "ROCK";
    case GHOST:
        return "GHOST";
    case DRAGON:
        return "DRAGON";
    case ICE:
        return "ICE";
    default:
        return "UNKNOWN";
    }
}

char* getDynamicInput()
{
    // the size, capacity of the input
    size_t size = 0, capacity = DEFAULT_CAPACITY;
    // allocate space for the input
    char* input = malloc(capacity);
    // in the allocation has failed, exit the program
    if (input == NULL)
    {
        printf("Memory allocation failed. (getDynamicInput: input)\n");
        exit(1);
    }

    // the scanned char
    int c;
    // while the line has not been ended
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // if the size exceeds the capacity
        if (size + 1 >= capacity)
        {
            // increase the capacity
            capacity *= 2;
            // reallocate space for the input
            input = realloc(input, capacity);
            // if the reallocation has failed, exit the program.
            if (input == NULL)
            {
                printf("Memory reallocation failed. (getDynamicInput: tmp)\n");
                exit(1);
            }
        }
        // add the scanned char to the input
        input[size++] = (char)c;
    }
    // after reading the input, add the null char in the end
    input[size] = '\0';

    // trim any leading/trailing whitespace or carriage returns
    input = trim(input);

    // return the scanned string
    return input;
}

void printPokemonNode(const PokemonNode* node)
{
    // if the PokemonNode does not exist
    if (node == NULL)
        return;
    // else, print all of its details
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id + 1,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           node->data->CAN_EVOLVE == CAN_EVOLVE ? "Yes" : "No");
}

void displayPokemon(const OwnerNode* owner)
{
    // if there is no Pokemon in the Pokedex, inform the user and return
    if (owner->pokedexRoot == NULL)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    // print the menu
    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    // read the chosen option
    const int choice = readIntSafe("Your choice: ");

    // do the chosen action accordingly
    switch (choice)
    {
    case 1:
        displayBFS(owner->pokedexRoot);
        break;
    case 2:
        displayPreOrder(owner->pokedexRoot);
        break;
    case 3:
        displayInOrder(owner->pokedexRoot);
        break;
    case 4:
        displayPostOrder(owner->pokedexRoot);
        break;
    case 5:
        displayAlphabetical(owner->pokedexRoot);
        break;
    default: // if the chosen action is not valid, inform the user and return
        printf("Invalid choice.\n");
    }
}

void displayAlphabetical(PokemonNode* root)
{
    // create an array for all the PokemonNodes in the BT
    NodeArray na;
    // init the array to a default capacity
    initNodeArray(&na, DEFAULT_CAPACITY);
    // insert all the nodes into the array
    collectAll(root, &na);
    // sort the nodes alphabetically
    qsort(na.nodes, na.size, sizeof(PokemonNode*), compareByNameNode);
    // print every node (by alphabetical order)
    for (int i = 0; i < na.size; ++i)
        printPokemonNode(na.nodes[i]);
    // free the array
    free(na.nodes);
}

bool compareByNameNode(const void* a, const void* b)
{
    // compare nodes by their name
    const PokemonNode* nodeA = *(const PokemonNode**)a;
    const PokemonNode* nodeB = *(const PokemonNode**)b;
    return strcmp(nodeA->data->name, nodeB->data->name);
}

void initNodeArray(NodeArray* na, const int cap)
{
    // allocate memory for the array
    na->nodes = malloc(cap * sizeof(PokemonNode*));
    // if the allocation failed, exit the program
    if (na->nodes == NULL)
    {
        printf("Memory allocation failed. (initNodeArray: na->nodes)\n");
        exit(1);
    }
    // init the array struct variables
    na->size = 0;
    na->capacity = cap;
}

void collectAll(PokemonNode* root, NodeArray* na)
{
    // if the given data has no nodes in it, return.
    if (root == NULL)
        return;
    // add the root node
    addNode(na, root);
    // repeat recursively for left and right
    collectAll(root->left, na);
    collectAll(root->right, na);
}

void addNode(NodeArray* na, PokemonNode* node)
{
    // if the size of the array equals the capacity, we have to increase the capacity
    if (na->size == na->capacity)
    {
        // increase the capacity
        na->capacity *= 2;
        // allocate memory with the new capacity
        na->nodes = realloc(na->nodes, na->capacity * sizeof(PokemonNode*));
        // if the allocation has failed, exit teh program
        if (na->nodes == NULL)
        {
            printf("Memory reallocation failed. (addNode: na->nodes)\n");
            exit(1);
        }
    }
    // add the node to the array
    na->nodes[na->size++] = node;
}

void displayInOrder(const PokemonNode* root)
{
    // print the nodes in the BT with the in-order algorithm
    inOrderGeneric(root, printPokemonNode);
}

void inOrderGeneric(const PokemonNode* root, const PokemonNodeConsumer consumer)
{
    // if the root is NULL, return
    if (root == NULL)
        return;
    // call the function on the left node
    inOrderGeneric(root->left, consumer);
    // activate the consumer on the root
    consumer(root);
    // call the function on the right node
    inOrderGeneric(root->right, consumer);
}

void displayPostOrder(const PokemonNode* root)
{
    // print the nodes in the BT with the post-order algorithm
    postOrderGeneric(root, printPokemonNode);
}

void postOrderGeneric(const PokemonNode* root, const PokemonNodeConsumer consumer)
{
    // if the root is NULL, return
    if (root == NULL)
        return;
    // call the function on the left node
    postOrderGeneric(root->left, consumer);
    // call the function on the right node
    postOrderGeneric(root->right, consumer);
    // activate the consumer on the root
    consumer(root);
}

void displayPreOrder(const PokemonNode* root)
{
    // print the nodes in the BT with the pre-order algorithm
    preOrderGeneric(root, printPokemonNode);
}

void preOrderGeneric(const PokemonNode* root, const PokemonNodeConsumer consumer)
{
    // if the root is NULL, return
    if (root == NULL)
        return;
    // activate the consumer on the root
    consumer(root);
    // call the function on the left node
    preOrderGeneric(root->left, consumer);
    // call the function on the right node
    preOrderGeneric(root->right, consumer);
}

void displayBFS(const PokemonNode* root)
{
    // print the nodes in the BT with the BFS algorithm
    BFSGeneric(root, printPokemonNode);
}

void BFSGeneric(const PokemonNode* root, const PokemonNodeConsumer consumer)
{
    // if the root is NULL, return
    if (root == NULL)
        return;
    // for every level in the BT
    for (int i = 0; true; ++i)
        // activate the consumer on every node in it
        if (!BTLevel(root, i, consumer))
            // and if there is no nodes in the level, exit
            break;
}

bool BTLevel(const PokemonNode* root, const int level, const PokemonNodeConsumer consumer)
{
    // if the root is NULL, return
    if (root == NULL)
        return false;
    // if the level is 0, just print the root
    if (level == 0)
    {
        consumer(root);
        // return that we found a node in this level
        return true;
    }
    // search for the node in the left and right nodes, and apply the consumer on the nodes in the level
    const bool left = BTLevel(root->left, level - 1, consumer);
    const bool right = BTLevel(root->right, level - 1, consumer);
    // return if we found a node in the level
    return left || right;
}

void pokedexMenu()
{
    // if there is no Pokedex, inform the user and return
    if (ownersHead == NULL)
    {
        printf("No existing Pokedexes.\n");
        return;
    }
    // print the menu
    printf("\nExisting Pokedexes:\n");
    const OwnerNode* owner = ownersHead;
    // print every Pokedex name in the menu
    for (int i = 0; i < ownerCount; i++, owner = owner->next)
        printf("%d. %s\n", i + 1, owner->ownerName);

    // read the choice of the user of the Pokedex he wants to open
    int choice = readIntSafe("Choose a Pokedex by number: ");
    // make target the chosen Pokedex
    OwnerNode* target = ownersHead;
    for (int i = 1; i < choice; i++, target = target->next)
    {
    }

    // inform the user
    printf("\nEntering %s's Pokedex...\n", target->ownerName);

    // until the user exits
    while (true)
    {
        // print the menu
        printf("\n-- %s's Pokedex Menu --\n", target->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        // input the choice of the user
        choice = readIntSafe("Your choice: ");

        // do the action chosen by the user
        switch (choice)
        {
        case 1:
            addPokemon(target);
            break;
        case 2:
            displayPokemon(target);
            break;
        case 3:
            freePokemon(target);
            break;
        case 4:
            pokemonFight(target);
            break;
        case 5:
            evolvePokemon(target);
            break;
        case 6:
            // if the user chose to exit, exit.
            printf("Back to Main Menu.\n");
            return;
        default:
            // if the user entered an invalid option, inform him
            printf("Invalid choice.\n");
        }
    }
}

void evolvePokemon(OwnerNode* owner)
{
    // if the Pokedex is empty, inform the user and return
    if (owner->pokedexRoot == NULL)
    {
        printf("Cannot evolve. Pokedex empty.\n");
        return;
    }
    // read the ID of the Pokemon the user wants to evolve
    const int choice = readIntSafe("Enter ID of Pokemon to evolve: ");
    // search for the Pokemon in the Pokedex with the BFS algorithm
    const PokemonNode* pokemon = searchPokemonBFS(owner->pokedexRoot, choice - 1);
    // if the Pokemon does not exist, inform the user and return
    if (pokemon == NULL)
    {
        printf("No Pokemon ID %d found.\n", choice);
        return;
    }
    // if the Pokemon cannot evolve, inform the user and return
    if (pokemon->data->CAN_EVOLVE == CANNOT_EVOLVE)
    {
        printf("%s (ID %d) cannot evolve.\n", pokemon->data->name, pokemon->data->id + 1);
        return;
    }
    // remove the Pokemon from the Pokedex
    removePokemonByID(owner->pokedexRoot, choice - 1);
    // inform the user that the Pokemon has evolved
    printf("Pokemon evolved from %s (ID %d) to %s (ID %d).",
           pokedex[choice].name, choice, pokedex[choice + 1].name, choice + 1);
    bool handleDupe = true;
    // insert the evolved Pokemon into the Pokedex
    owner->pokedexRoot = insertPokemonNode(owner->pokedexRoot, createPokemonNode(&pokedex[choice + 1]), &handleDupe);
}

void pokemonFight(const OwnerNode* owner)
{
    // if the Pokedex is empty, inform the user and return
    if (owner->pokedexRoot == NULL)
    {
        printf("Pokedex is empty.\n");
        return;
    }
    // read the IDs of the two Pokemon the user wants to fight
    const int first = readIntSafe("Enter ID of the first Pokemon: ");
    const int second = readIntSafe("Enter ID of the second Pokemon: ");
    // search for the two Pokemon in the Pokedex with the BFS algorithm
    const PokemonNode* firstNode = searchPokemonBFS(owner->pokedexRoot, first - 1);
    const PokemonNode* secondNode = searchPokemonBFS(owner->pokedexRoot, second - 1);
    // if one or both of the Pokemon do not exist, inform the user and return
    if (secondNode == NULL || firstNode == NULL)
    {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }
    // print the details of the two Pokemon
    printf("Pokemon 1: %s (Score = %.2f)\n",
           firstNode->data->name, firstNode->data->attack * 1.5 + firstNode->data->hp * 1.2);
    printf("Pokemon 2: %s (Score = %.2f)\n",
           secondNode->data->name, secondNode->data->attack * 1.5 + secondNode->data->hp * 1.2);
    // print the winner of the fight
    if (firstNode->data->attack * 1.5 + firstNode->data->hp * 1.2
        > secondNode->data->attack * 1.5 + secondNode->data->hp * 1.2)
        printf("%s wins!\n", firstNode->data->name);
    else if (firstNode->data->attack * 1.5 + firstNode->data->hp * 1.2
        < secondNode->data->attack * 1.5 + secondNode->data->hp * 1.2)
        printf("%s wins!\n", secondNode->data->name);
    else printf("it's a tie!\n");
}

PokemonNode* searchPokemonBFS(PokemonNode* root, const int id)
{
    // nah it is not really BFS, but it is a search function ;)
    // if the root is NULL, return NULL
    if (root == NULL)
        return NULL;
    // if the root is the Pokemon we are looking for, return it
    if (root->data->id == id)
        return root;
    // search for the Pokemon in the left and right nodes
    PokemonNode* left = searchPokemonBFS(root->left, id);
    PokemonNode* right = searchPokemonBFS(root->right, id);
    // return the Pokemon if it was found
    return left != NULL ? left : right;
}

void freePokemon(OwnerNode* owner)
{
    // if the Pokedex is empty, inform the user and return
    if (owner->pokedexRoot == NULL)
    {
        printf("No Pokemon to release.\n");
        return;
    }
    // read the ID of the Pokemon the user wants to release
    const int id = readIntSafe("Enter Pokemon ID to release: ");
    // remove the Pokemon from the Pokedex
    owner->pokedexRoot = removePokemonByID(owner->pokedexRoot, id - 1);
}

PokemonNode* removePokemonByID(PokemonNode* root, const int id)
{
    // if the root is NULL, inform the user and return NULL
    if (root == NULL)
    {
        printf("Pokemon with ID %d not found.\n", id + 1);
        return NULL;
    }
    // if the Pokemon is in the left node tree, recursively remove it
    if (root->data->id < id)
        root->right = removePokemonByID(root->right, id);
        // if the Pokemon is in the right node tree, recursively remove it
    else if (root->data->id > id)
        root->left = removePokemonByID(root->left, id);
    // if the Pokemon is the root, remove it
    else
    {
        // inform the user that the Pokemon has been removed
        printf("Removing Pokemon %s (ID %d).\n", root->data->name, root->data->id + 1);
        bool handleDupe = false;
        // if the Pokemon has two children
        if (root->left != NULL && root->right != NULL)
            // add the right child to the left child
            root->left = insertPokemonNode(root->left, root->right, &handleDupe);
        else if (root->right != NULL)
            // if the Pokemon has only a right child, return the right child
            root->left = root->right;
        // free the Pokemon
        PokemonNode* tmp = root->left;
        freePokemonNode(root);
        // return the left child
        return tmp;
    }
    // if the Pokemon was not found, return the root
    return root;
}

void freePokemonNode(PokemonNode* node)
{
    // there is not much to do here, just free the PokemonNode
    free(node);
}

void addPokemon(OwnerNode* owner)
{
    // read the ID of the Pokemon the user wants to add
    const int id = readIntSafe("Enter ID to add: ");
    // if the ID is invalid, inform the user and return
    if (id > POKEDEX_LENGTH || id < 1)
    {
        printf("Invalid ID.\n");
        return;
    }
    bool handleDupe = true;
    // insert the Pokemon into the Pokedex
    owner->pokedexRoot = insertPokemonNode(owner->pokedexRoot, createPokemonNode(&pokedex[id - 1]),
                                           &handleDupe);
    // inform the user that the Pokemon has been added if it was not a duplicate
    if (handleDupe) printf("Pokemon %s (ID %d) added.", pokedex[id - 1].name, id);
}

PokemonNode* insertPokemonNode(PokemonNode* root, PokemonNode* node, bool* handleDupe)
{
    // if the root is NULL, return the node
    if (root == NULL)
        return node;
    // if the node is smaller than the root, insert it to the left
    if (node->data->id < root->data->id)
        root->left = insertPokemonNode(root->left, node, handleDupe);
    // if the node is bigger than the root, insert it to the right
    else if (node->data->id > root->data->id)
        root->right = insertPokemonNode(root->right, node, handleDupe);
    // if the node is a duplicate, free it
    else
    {
        // duplicate in pokedex
        if (*handleDupe)
            printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n", node->data->id + 1);
        // change the handleDupe to the opposite value to inform that the node is a duplicate for the outside function
        *handleDupe = !*handleDupe;
        // if the node is a duplicate, free it
        freePokemonNode(node);
    }
    // return the root after the insertion
    return root;
}

void mainMenu()
{
    // until the user exits
    while (true)
    {
        // print the menu
        printf("\n=== Main Menu ===\n"
            "1. New Pokedex\n"
            "2. Existing Pokedex\n"
            "3. Delete a Pokedex\n"
            "4. Merge Pokedexes\n"
            "5. Sort Owners by Name\n"
            "6. Print Owners in a direction X times\n"
            "7. Exit\n");
        // read the choice of the user
        const int choice = readIntSafe("Your choice: ");

        // do the action chosen by the user
        switch (choice)
        {
        case 1:
            newPokedex();
            break;
        case 2:
            pokedexMenu();
            break;
        case 3:
            deletePokedex();
            break;
        case 4:
            mergePokedexMenu();
            break;
        case 5:
            sortOwners();
            break;
        case 6:
            printOwnersCircular();
            break;
        case 7:
            // if the user chose to exit, exit.
            printf("Goodbye!\n");
            return;
        default:
            // if the user entered an invalid option, inform him
            printf("Invalid.\n");
        }
    }
}

void printOwnersCircular()
{
    // if there are no owners, inform the user and return
    if (ownersHead == NULL)
    {
        printf("No owners.\n");
        return;
    }
    // scan the direction the user wants to print the owners
    printf("Enter direction (F or B): ");
    char direction;
    scanf(" %c", &direction);
    // if the direction is forwards, set the direction to true
    if (direction == 'F' || direction == 'f')
        direction = true;
    // if the direction is backwards, set the direction to false
    else if (direction == 'B' || direction == 'b')
        direction = false;
    // if the direction is invalid, inform the user and return
    else
    {
        printf("Invalid direction.\n");
        return;
    }
    scanf(" "); // consume newline
    // read the number of times the user wants to print the owners
    const int times = readIntSafe("How many prints? ");
    const OwnerNode* curr = ownersHead;
    // print the owners in the chosen direction
    for (int i = 0; i < times; ++i, curr = direction ? curr->next : curr->prev)
        printf("[%d] %s\n", i + 1, curr->ownerName);
}

void sortOwners(void)
{
    // if there are less than 2 owners to sort, inform the user and return
    if (ownerCount < 2)
    {
        printf("0 or 1 owners only => no need to sort.\n");
        return;
    }
    // sort the owners by their name using Bubble Sort
    for (int i = 1; i < ownerCount; ++i)
    {
        OwnerNode* curr = ownersHead;
        for (int j = 1; j < ownerCount; ++j)
        {
            if (strcmp(curr->ownerName, curr->next->ownerName) > 0)
                swapOwnerData(curr, curr->next);
            curr = curr->next;
        }
    }
    // inform the user that the owners have been sorted
    printf("Owners sorted by name.\n");
}

void swapOwnerData(OwnerNode* a, OwnerNode* b)
{
    // swap the data of the two owners using temporary variables
    char* tmp = a->ownerName;
    a->ownerName = b->ownerName;
    b->ownerName = tmp;
    PokemonNode* tmpRoot = a->pokedexRoot;
    a->pokedexRoot = b->pokedexRoot;
    b->pokedexRoot = tmpRoot;
}

void mergePokedexMenu()
{
    // if there are less than 2 owners to merge, inform the user and return
    if (ownerCount < 2)
    {
        printf("Not enough owners to merge.\n");
        return;
    }
    // print the menu
    printf("\n=== Merge Pokedexes ===\n");
    // scan the names of the two owners the user wants to merge
    printf("Enter name of first owner: ");
    char* first = getDynamicInput();
    printf("Enter name of second owner: ");
    char* second = getDynamicInput();
    // find the owners by their names
    OwnerNode* firstOwner = findOwnerByName(first);
    OwnerNode* secondOwner = findOwnerByName(second);
    // inform the user that the owners are being merged
    printf("Merging %s and %s...\n", first, second);
    // if the Pokedex of the first owner is empty,
    // set the Pokedex of the first owner to the Pokedex of the second owner
    if (firstOwner->pokedexRoot == NULL)
        firstOwner->pokedexRoot = secondOwner->pokedexRoot;
    // else, if the Pokedexes are not the same, merge the second Pokedex into the first Pokedex
    else if (firstOwner != secondOwner)
        firstOwner->pokedexRoot = mergeRoots(firstOwner->pokedexRoot, secondOwner->pokedexRoot);
    // inform the user that the merge has been completed
    printf("Merge completed.\n");
    // remove the second owner's Pokedex
    secondOwner->pokedexRoot = NULL;
    // free the second owner's Pokedex
    freeOwnerNode(secondOwner);
    // inform the user that the second owner has been removed after merging
    printf("Owner '%s' has been removed after merging.\n", second);
    // free the names of the owners
    free(first);
    free(second);
}

PokemonNode* mergeRoots(PokemonNode* first, PokemonNode* second)
{
    // if the second node Pokemon is NULL, return the first Pokemon (the BST)
    if (second == NULL)
        return first;
    // merge the left and right nodes of the second Pokemon into the first Pokemon
    first = mergeRoots(first, second->left);
    first = mergeRoots(first, second->right);
    // remove the left, right nodes of the second Pokemon from its data
    second->left = second->right = NULL;
    bool handleDupe = false;
    // insert the second Pokemon into the first Pokemon BST
    return insertPokemonNode(first, second, &handleDupe);
}

OwnerNode* findOwnerByName(const char* name)
{
    // for every owner, if the name is the same as the given name, return the owner
    OwnerNode* curr = ownersHead;
    for (int i = 0; i < ownerCount; ++i, curr = curr->next)
    {
        if (strcmp(name, curr->ownerName) == 0)
            return curr;
    }
    // if the owner was not found, return NULL
    return NULL;
}

void deletePokedex()
{
    // if there are no owners, inform the user and return
    if (ownersHead == NULL)
    {
        printf("No existing Pokedexes to delete.\n");
        return;
    }
    // print the menu
    printf("\n=== Delete a Pokedex ===\n");
    // print the names of the owners
    const OwnerNode* owner = ownersHead;
    for (int i = 0; i < ownerCount; i++, owner = owner->next)
        printf("%d. %s\n", i + 1, owner->ownerName);
    // read the choice of the user
    const int choice = readIntSafe("Choose a Pokedex to delete by number: ");
    // make target the chosen Pokedex
    OwnerNode* target = ownersHead;
    for (int i = 1; i < choice; i++, target = target->next)
    {
    }
    // inform the user that the Pokedex is being deleted
    printf("Deleting %s's entire Pokedex...\n", target->ownerName);
    // free the Pokedex
    freeOwnerNode(target);
    // inform the user that the Pokedex has been deleted
    printf("Pokedex deleted.\n");
}

void freeOwnerNode(OwnerNode* owner)
{
    // free the BST of the owner
    freePokemonTree(owner->pokedexRoot);
    // redirect the pointers of the Linked List of the owners
    owner->prev->next = owner->next;
    owner->next->prev = owner->prev;
    // if the owner is the head of the Linked List, set the head to the next owner
    if (owner == ownersHead)
        ownersHead = owner->next;
    // if the owner is the tail of the Linked List, set the tail to the previous owner
    if (owner == ownersTail)
        ownersTail = owner->prev;
    // if the owner is the only owner in the Linked List, set the head and tail to NULL
    if (owner == ownersHead && owner == ownersTail)
        ownersHead = ownersTail = NULL;
    // free the name of the owner
    free(owner->ownerName);
    // free the owner
    free(owner);
    // decrease the number of owners
    ownerCount--;
}

void freePokemonTree(PokemonNode* root)
{
    // if the root is NULL, return
    if (root == NULL)
        return;
    // recursively free the left and right nodes and their children
    freePokemonTree(root->left);
    freePokemonTree(root->right);
    // free the root
    freePokemonNode(root);
}

void newPokedex()
{
    // scan the name of the owner of the new Pokedex
    printf("Your name: ");
    char* ownerName = getDynamicInput();
    // if the name was not scanned properly, inform the user and exit
    if (ownerName == NULL)
    {
        printf("Failed to read name.\n");
        exit(1);
    }
    // if an owner with the same name already exist, inform the user and return
    const OwnerNode* curr = ownersHead;
    for (int i = 0; i < ownerCount; ++i, curr = curr->next)
    {
        if (strcmp(ownerName, curr->ownerName) == 0)
        {
            printf("Owner '%s' already exists. Not creating a new Pokedex.\n", ownerName);
            // free the name of the owner
            free(ownerName);
            return;
        }
    }
    // scan the starter Pokemon of the new Pokedex
    printf("Choose Starter:\n"
        "1. Bulbasaur\n"
        "2. Charmander\n"
        "3. Squirtle\n");
    // read the choice of the user
    const int choice = readIntSafe("Your choice: ");
    PokemonNode* starter;
    // get the starter Pokemon according to the choice of the user
    switch (choice)
    {
    case 1:
        starter = createPokemonNode(&pokedex[BULBASAUR]);
        break;
    case 2:
        starter = createPokemonNode(&pokedex[CHARMANDER]);
        break;
    case 3:
        starter = createPokemonNode(&pokedex[SQUIRTLE]);
        break;
    default: // we assume input validation
        printf("Invalid choice. (newPokedex: choice)\n");
        exit(1);
    }
    // add the owner to the Linked List of the owners
    addOwner(createOwner(ownerName, starter));
    // inform the user that the Pokedex has been created
    printf("New Pokedex created for %s with starter %s.\n", ownerName, starter->data->name);
}

void addOwner(OwnerNode* owner)
{
    // if the Linked List of the owners is empty, update it accordingly
    if (ownersHead == NULL)
    {
        ownersHead = owner;
        ownersTail = owner;
        owner->next = owner;
        owner->prev = owner;
    }
    // else, add the owner to the end of the Linked List
    else
    {
        ownersTail->next = owner;
        owner->prev = ownersTail;
        owner->next = ownersHead;
        ownersHead->prev = owner;
        ownersTail = owner;
    }
    // update the number of owners
    ownerCount++;
}

OwnerNode* createOwner(char* name, PokemonNode* starter)
{
    // allocate memory for the new owner
    OwnerNode* owner = malloc(sizeof(OwnerNode));
    // if the allocation has failed, exit the program
    if (owner == NULL)
    {
        printf("Memory allocation failed. (createOwner: owner)\n");
        exit(1);
    }
    // set the details of the owner
    owner->ownerName = name;
    owner->pokedexRoot = starter;
    owner->next = NULL;
    owner->prev = NULL;
    // return the newly created owner
    return owner;
}

PokemonNode* createPokemonNode(const PokemonData* data)
{
    // allocate memory for the new PokemonNode
    PokemonNode* node = malloc(sizeof(PokemonNode));
    // if the allocation has failed, exit the program
    if (node == NULL)
    {
        printf("Memory allocation failed. (createPokemonNode: node)\n");
        exit(1);
    }
    // set the details of the PokemonNode
    node->data = data;
    node->left = node->right = NULL;
    // return the newly created PokemonNode
    return node;
}
