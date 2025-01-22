#ifndef EX6_H
#define EX6_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// boolean handling ************************************************************

#define bool    int
#define true    1
#define false   0
#define neg     (-1)

// other definitions ***********************************************************

#define DEFAULT_CAPACITY    128
#define POKEDEX_LENGTH      151
#define BULBASAUR           0
#define CHARMANDER          3
#define SQUIRTLE            6

// type definitions ************************************************************

typedef enum PokemonType
{
    GRASS,
    FIRE,
    WATER,
    BUG,
    NORMAL,
    POISON,
    ELECTRIC,
    GROUND,
    FAIRY,
    FIGHTING,
    PSYCHIC,
    ROCK,
    GHOST,
    DRAGON,
    ICE
} PokemonType;

typedef enum EvolutionStatus
{
    CANNOT_EVOLVE,
    CAN_EVOLVE
} EvolutionStatus;

typedef struct PokemonData
{
    int id;
    char* name;
    PokemonType TYPE;
    int hp;
    int attack;
    EvolutionStatus CAN_EVOLVE;
} PokemonData;

// Binary Tree Node (for Pokedex)
typedef struct PokemonNode
{
    const PokemonData* data;
    struct PokemonNode* left;
    struct PokemonNode* right;
} PokemonNode;

// Linked List Node (for Owners)
typedef struct OwnerNode
{
    char* ownerName; // Owner's name
    PokemonNode* pokedexRoot; // Pointer to the root of the owner's Pokedex
    struct OwnerNode* next; // Next owner in the linked list
    struct OwnerNode* prev; // Previous owner in the linked list
} OwnerNode;

typedef struct NodeArray
{
    PokemonNode** nodes;
    int size;
    int capacity;
} NodeArray;

typedef void (*PokemonNodeConsumer)(const PokemonNode*);

// methods *********************************************************************

/**
 * @brief Remove leading/trailing whitespace (including '\r').
 * @param str modifiable string
 * @return the trimmed string
 * Why we made it: We must handle CR/LF or random spaces in user input.
 */
char* trim(char* str);

/**
 * @brief Read an integer safely, re-prompt if invalid.
 * @param prompt text to display
 * @return valid integer from user
 * Why we made it: We want robust menu/ID input handling.
 */
int readIntSafe(const char* prompt);

/**
 * @brief Read a line from stdin, store in the allocated buffer, trim whitespace.
 * @return pointer to the newly allocated string (caller frees)
 * Why we made it: We need flexible name input that handles CR/LF etc.
 */
char* getDynamicInput(void);

/**
 * @brief Return a string for a given PokemonType enum.
 * @param type the PokemonType ptr
 * @return string like "GRASS", "FIRE", etc.
 * Why we made it: So we can print readable type names.
 */
const char* getTypeName(PokemonType type);

/**
 * @brief Create a BST node with a copy of the given PokemonData.
 * @param data pointer to PokemonData (like from the global Pokedex)
 * @return newly allocated PokemonNode*
 * Why we made it: We need a standard way to allocate BST nodes.
 */
PokemonNode* createPokemonNode(const PokemonData* data);

/**
 * @brief Create an OwnerNode for the circular owners list.
 * @param name the dynamically allocated name
 * @param starter BST root for the starter Pokemon
 * @return newly allocated OwnerNode*
 * Why we made it: Each user is represented as an OwnerNode.
 */
OwnerNode* createOwner(char* name, PokemonNode* starter);

/**
 * @brief adds the OwnerNode to the circular owners list.
 * @param owner the pointer to the OwnerNode
 */
void addOwner(OwnerNode* owner);

/**
 * @brief Free one PokemonNode (including name).
 * @param node pointer to node
 * Why we made it: Avoid memory leaks for single nodes.
 */
void freePokemonNode(PokemonNode* node);

/**
 * @brief Recursively free a BST of PokemonNodes.
 * @param root BST root
 * Why we made it: Clearing a user’s entire Pokedex means freeing a tree.
 */
void freePokemonTree(PokemonNode* root);

/**
 * @bried adds the second Pokedex to the first Pokedex
 * @param first the first PokemonNode
 * @param second the second PokemonNode
 * @return the merged Pokedex
 */
PokemonNode* mergeRoots(PokemonNode* first, PokemonNode* second);

/**
 * @brief Free an OwnerNode (including name and entire Pokedex BST).
 * @param owner pointer to the owner
 * Why we made it: Deleting an owner also frees their Pokedex & name.
 */
void freeOwnerNode(OwnerNode* owner);

/**
 * @brief Insert a PokemonNode into BST by ID; duplicates freed.
 * @param root pointer to BST root
 * @param node node to insert
 * @param handleDupe if true, handle duplicates
 * @return updated BST root
 * Why we made it: Standard BST insertion ignoring duplicates.
 */
PokemonNode* insertPokemonNode(PokemonNode* root, PokemonNode* node, bool* handleDupe);

/**
 * @brief BFS search for a Pokemon by ID in the BST.
 * @param root BST root
 * @param id ID to find
 * @return pointer to found node or NULL
 * Why we made it: BFS ensures we find nodes even in an unbalanced tree.
 */
PokemonNode* searchPokemonBFS(PokemonNode* root, int id);

/**
 * @brief Remove node from BST by ID if found (BST removal logic).
 * @param root BST root
 * @param id ID to remove
 * @return updated BST root
 * Why we made it: We handle special cases of a BST remove (0,1,2 children)
 */
PokemonNode* removeNodeBST(PokemonNode* root, int id);

/**
 * @brief Combine BFS search + BST removal to remove Pokemon by ID.
 * @param root BST root
 * @param id the ID to remove
 * @return updated BST root
 * Why we made it: BFS confirms existence, then removeNodeBST does the removal.
 */
PokemonNode* removePokemonByID(PokemonNode* root, int id);

/**
 * @brief applying consumer on the given level of the BT
 * @param root BT root
 * @param level the level to print
 * @param consumer the consumer to apply
 * @return if the level is not empty
 */
bool BTLevel(const PokemonNode* root, int level, PokemonNodeConsumer consumer);

/**
 * @brief Generic BFS traversal: call consumer() on each node (level-order).
 * @param root BST root
 * @param consumer function pointer for what to do with each node
 * Why we made it: BFS plus function pointers => flexible traversal.
 */
void BFSGeneric(const PokemonNode* root, PokemonNodeConsumer consumer);

/**
 * @brief A generic pre-order traversal (Root-Left-Right).
 * @param root BST root
 * @param consumer function pointer
 * Why we made it: Another demonstration of function-pointer-based traversal.
 */
void preOrderGeneric(const PokemonNode* root, PokemonNodeConsumer consumer);

/**
 * @brief A generic in-order traversal (Left-Root-Right).
 * @param root BST root
 * @param consumer function pointer
 * Why we made it: Great for seeing sorted order if BST is sorted by ID.
 */
void inOrderGeneric(const PokemonNode* root, PokemonNodeConsumer consumer);

/**
 * @brief A generic post-order traversal (Left-Right-Root).
 * @param root BST root
 * @param consumer function pointer
 * Why we made it: Another standard traversal pattern.
 */
void postOrderGeneric(const PokemonNode* root, PokemonNodeConsumer consumer);

/**
 * @brief Print one PokemonNode’s data: ID, Name, Type, HP, Attack, Evolve?
 * @param node pointer to the node
 * Why we made it: We can pass this to BFSGeneric or others to quickly print.
 */
void printPokemonNode(const PokemonNode* node);

/**
 * @brief Initialize a NodeArray with given capacity.
 * @param na pointer to NodeArray
 * @param cap initial capacity
 * Why we made it: We store pointers to PokemonNodes for alphabetical sorting.
 */
void initNodeArray(NodeArray *na, int cap);

/**
 * @brief Add a PokemonNode pointer to NodeArray, reallocate if needed.
 * @param na pointer to NodeArray
 * @param node pointer to the node
 * Why we made it: We want a dynamic list of BST nodes for sorting.
 */
void addNode(NodeArray *na, PokemonNode *node);

/**
 * @brief Recursively collect all nodes from the BST into a NodeArray.
 * @param root BST root
 * @param na pointer to NodeArray
 * Why we made it: We gather everything for qsort.
 */
void collectAll(PokemonNode *root, NodeArray *na);

/**
 * @brief Compare function for qsort (alphabetical by node->data->name).
 * @param a pointer to a pointer to PokemonNode
 * @param b pointer to a pointer to PokemonNode
 * @return -1, 0, or +1
 * Why we made it: Sorting by name for alphabetical display.
 */
bool compareByNameNode(const void *a, const void *b);

/**
 * @brief BFS is nice, but alphabetical means we gather all nodes, sort by name, then print.
 * @param root BST root
 * Why we made it: Provide user the option to see Pokemon sorted by name.
 */
void displayAlphabetical(PokemonNode *root);

/**
 * @brief BFS user-friendly display (level-order).
 * @param root BST root
 * Why we made it: Quick listing in BFS order.
 */
void displayBFS(const PokemonNode *root);

/**
 * @brief Pre-order user-friendly display (Root->Left->Right).
 * @param root BST root
 * Why we made it: Another standard traversal for demonstration.
 */
void displayPreOrder(const PokemonNode *root);

/**
 * @brief In-order user-friendly display (Left->Root->Right).
 * @param root BST root
 * Why we made it: Good for sorted output by ID if the tree is a BST.
 */
void displayInOrder(const PokemonNode *root);

/**
 * @brief Post-order user-friendly display (Left->Right->Root).
 * @param root BST root
 * Why we made it: Another standard traversal pattern.
 */
void displayPostOrder(const PokemonNode *root);

/**
 * @brief Let user pick two Pokemon by ID in the same Pokedex to fight.
 * @param owner pointer to the Owner
 * Why we made it: Fun demonstration of BFS and custom formula for battles.
 */
void pokemonFight(const OwnerNode *owner);

/**
 * @brief Evolve a Pokemon (ID -> ID+1) if allowed.
 * @param owner pointer to the Owner
 * Why we made it: Demonstrates removing an old ID, inserting the next ID.
 */
void evolvePokemon(OwnerNode *owner);

/**
 * @brief Prompt for an ID, BFS-check duplicates, then insert into BST.
 * @param owner pointer to the Owner
 * Why we made it: Primary user function for adding new Pokemon to an owner’s Pokedex.
 */
void addPokemon(OwnerNode *owner);

/**
 * @brief Prompt for ID, remove that Pokemon from BST by ID.
 * @param owner pointer to the Owner
 * Why we made it: Another user function for releasing a Pokemon.
 */
void freePokemon(OwnerNode *owner);

/**
 * @brief Show sub-menu to let user pick BFS, Pre, In, Post, or alphabetical.
 * @param owner pointer to Owner
 * Why we made it: We want a simple menu that picks from various traversals.
 */
void displayPokemon(const OwnerNode* owner);

/**
 * @brief Sort the circular owners list by name.
 * Why we made it: Another demonstration of pointer manipulation + sorting logic.
 */
void sortOwners(void);

/**
 * @brief Helper to swap name & pokedexRoot in two OwnerNode.
 * @param a pointer to first owner
 * @param b pointer to second owner
 * Why we made it: Used internally by bubble sort to swap data.
 */
void swapOwnerData(OwnerNode* a, OwnerNode* b);

/**
 * @brief Insert a new owner into the circular list. If none exist, it's alone.
 * @param newOwner pointer to newly created OwnerNode
 * Why we made it: We need a standard approach to keep the list circular.
 */
void linkOwnerInCircularList(OwnerNode* newOwner);

/**
 * @brief Remove a specific OwnerNode from the circular list, possibly updating head.
 * @param target pointer to the OwnerNode
 * Why we made it: Deleting or merging owners requires removing them from the ring.
 */
void removeOwnerFromCircularList(OwnerNode* target);

/**
 * @brief Find an owner by name in the circular list.
 * @param name string to match
 * @return pointer to the matching OwnerNode or NULL
 * Why we made it: We often need to locate an owner quickly.
 */
OwnerNode* findOwnerByName(const char* name);

/**
 * @brief Let user pick an existing Pokedex (owner) by number, then sub-menu.
 * Why we made it: This is the main interface for adding/fighting/evolving, etc.
 */
void pokedexMenu();

/**
 * @brief Creates a new Pokedex (prompt for name, check uniqueness, choose starter).
 * Why we made it: The main entry for building a brand-new Pokedex.
 */
void newPokedex(void);

/**
 * @brief Delete an entire Pokedex (owner) from the list.
 * Why we made it: Let user pick which Pokedex to remove and free everything.
 */
void deletePokedex(void);

/**
 * @brief Merge the second owner's Pokedex into the first, then remove the second owner.
 * Why we made it: BFS copy demonstration plus removing an owner.
 */
void mergePokedexMenu(void);

/**
 * @brief Print owners left or right from head, repeating as many times as user wants.
 * Why we made it: Demonstrates stepping through a circular list in a chosen direction.
 */
void printOwnersCircular(void);

/**
 * @brief Frees every remaining owner in the circular list, setting ownerHead = NULL.
 * Why we made it: Ensures a squeaky-clean exit with no leftover memory.
 */
void freePokdexes(OwnerNode** pPockdexes);

/**
 * @brief The main driver loop for the program (new Pokedex, merge, fight, etc.).
 * Why we made it: Our top-level UI that keeps the user engaged until they exit :)
 */
void mainMenu(void);

// data ************************************************************************

static const PokemonData pokedex[] = {
    {0, "Bulbasaur", GRASS, 45, 49, CAN_EVOLVE},
    {1, "Ivysaur", GRASS, 60, 62, CAN_EVOLVE},
    {2, "Venusaur", GRASS, 80, 82, CANNOT_EVOLVE},
    {3, "Charmander", FIRE, 39, 52, CAN_EVOLVE},
    {4, "Charmeleon", FIRE, 58, 64, CAN_EVOLVE},
    {5, "Charizard", FIRE, 78, 84, CANNOT_EVOLVE},
    {6, "Squirtle", WATER, 44, 48, CAN_EVOLVE},
    {7, "Wartortle", WATER, 59, 63, CAN_EVOLVE},
    {8, "Blastoise", WATER, 79, 83, CANNOT_EVOLVE},
    {9, "Caterpie", BUG, 45, 30, CAN_EVOLVE},
    {10, "Metapod", BUG, 50, 20, CAN_EVOLVE},
    {11, "Butterfree", BUG, 60, 45, CANNOT_EVOLVE},
    {12, "Weedle", BUG, 40, 35, CAN_EVOLVE},
    {13, "Kakuna", BUG, 45, 25, CAN_EVOLVE},
    {14, "Beedrill", BUG, 65, 90, CANNOT_EVOLVE},
    {15, "Pidgey", NORMAL, 40, 45, CAN_EVOLVE},
    {16, "Pidgeotto", NORMAL, 63, 60, CAN_EVOLVE},
    {17, "Pidgeot", NORMAL, 83, 80, CANNOT_EVOLVE},
    {18, "Rattata", NORMAL, 30, 56, CAN_EVOLVE},
    {19, "Raticate", NORMAL, 55, 81, CANNOT_EVOLVE},
    {20, "Spearow", NORMAL, 40, 60, CAN_EVOLVE},
    {21, "Fearow", NORMAL, 65, 90, CANNOT_EVOLVE},
    {22, "Ekans", POISON, 35, 60, CAN_EVOLVE},
    {23, "Arbok", POISON, 60, 85, CANNOT_EVOLVE},
    {24, "Pikachu", ELECTRIC, 35, 55, CAN_EVOLVE},
    {25, "Raichu", ELECTRIC, 60, 90, CANNOT_EVOLVE},
    {26, "Sandshrew", GROUND, 50, 75, CAN_EVOLVE},
    {27, "Sandslash", GROUND, 75, 100, CANNOT_EVOLVE},
    {28, "NidoranF", POISON, 55, 47, CAN_EVOLVE},
    {29, "Nidorina", POISON, 70, 62, CAN_EVOLVE},
    {30, "Nidoqueen", POISON, 90, 92, CANNOT_EVOLVE},
    {31, "NidoranM", POISON, 46, 57, CAN_EVOLVE},
    {32, "Nidorino", POISON, 61, 72, CAN_EVOLVE},
    {33, "Nidoking", POISON, 81, 102, CANNOT_EVOLVE},
    {34, "Clefairy", FAIRY, 70, 45, CAN_EVOLVE},
    {35, "Clefable", FAIRY, 95, 70, CANNOT_EVOLVE},
    {36, "Vulpix", FIRE, 38, 41, CAN_EVOLVE},
    {37, "Ninetales", FIRE, 73, 76, CANNOT_EVOLVE},
    {38, "Jigglypuff", NORMAL, 115, 45, CAN_EVOLVE},
    {39, "Wigglytuff", NORMAL, 140, 70, CANNOT_EVOLVE},
    {40, "Zubat", POISON, 40, 45, CAN_EVOLVE},
    {41, "Golbat", POISON, 75, 80, CAN_EVOLVE},
    {42, "Oddish", GRASS, 45, 50, CAN_EVOLVE},
    {43, "Gloom", GRASS, 60, 65, CAN_EVOLVE},
    {44, "Vileplume", GRASS, 75, 80, CANNOT_EVOLVE},
    {45, "Paras", BUG, 35, 70, CAN_EVOLVE},
    {46, "Parasect", BUG, 60, 95, CANNOT_EVOLVE},
    {47, "Venonat", BUG, 60, 55, CAN_EVOLVE},
    {48, "Venomoth", BUG, 70, 65, CANNOT_EVOLVE},
    {49, "Diglett", GROUND, 10, 55, CAN_EVOLVE},
    {50, "Dugtrio", GROUND, 35, 80, CANNOT_EVOLVE},
    {51, "Meowth", NORMAL, 40, 45, CAN_EVOLVE},
    {52, "Persian", NORMAL, 65, 70, CANNOT_EVOLVE},
    {53, "Psyduck", WATER, 50, 52, CAN_EVOLVE},
    {54, "Golduck", WATER, 80, 82, CANNOT_EVOLVE},
    {55, "Mankey", FIGHTING, 40, 80, CAN_EVOLVE},
    {56, "Primeape", FIGHTING, 65, 105, CANNOT_EVOLVE},
    {57, "Growlithe", FIRE, 55, 70, CAN_EVOLVE},
    {58, "Arcanine", FIRE, 90, 110, CANNOT_EVOLVE},
    {59, "Poliwag", WATER, 40, 50, CAN_EVOLVE},
    {60, "Poliwhirl", WATER, 65, 65, CAN_EVOLVE},
    {61, "Poliwrath", WATER, 90, 95, CANNOT_EVOLVE},
    {62, "Abra", PSYCHIC, 25, 20, CAN_EVOLVE},
    {63, "Kadabra", PSYCHIC, 40, 35, CAN_EVOLVE},
    {64, "Alakazam", PSYCHIC, 55, 50, CANNOT_EVOLVE},
    {65, "Machop", FIGHTING, 70, 80, CAN_EVOLVE},
    {66, "Machoke", FIGHTING, 80, 100, CAN_EVOLVE},
    {67, "Machamp", FIGHTING, 90, 130, CANNOT_EVOLVE},
    {68, "Bellsprout", GRASS, 50, 75, CAN_EVOLVE},
    {69, "Weepinbell", GRASS, 65, 90, CAN_EVOLVE},
    {70, "Victreebel", GRASS, 80, 105, CANNOT_EVOLVE},
    {71, "Tentacool", WATER, 40, 40, CAN_EVOLVE},
    {72, "Tentacruel", WATER, 80, 70, CANNOT_EVOLVE},
    {73, "Geodude", ROCK, 40, 80, CAN_EVOLVE},
    {74, "Graveler", ROCK, 55, 95, CAN_EVOLVE},
    {75, "Golem", ROCK, 80, 120, CANNOT_EVOLVE},
    {76, "Ponyta", FIRE, 50, 85, CAN_EVOLVE},
    {77, "Rapidash", FIRE, 65, 100, CANNOT_EVOLVE},
    {78, "Slowpoke", WATER, 90, 65, CAN_EVOLVE},
    {79, "Slowbro", WATER, 95, 75, CANNOT_EVOLVE},
    {80, "Magnemite", ELECTRIC, 25, 35, CAN_EVOLVE},
    {81, "Magneton", ELECTRIC, 50, 60, CANNOT_EVOLVE},
    {82, "Farfetch'd", NORMAL, 52, 65, CANNOT_EVOLVE},
    {83, "Doduo", NORMAL, 35, 85, CAN_EVOLVE},
    {84, "Dodrio", NORMAL, 60, 110, CANNOT_EVOLVE},
    {85, "Seel", WATER, 65, 45, CAN_EVOLVE},
    {86, "Dewgong", WATER, 90, 70, CANNOT_EVOLVE},
    {87, "Grimer", POISON, 80, 80, CAN_EVOLVE},
    {88, "Muk", POISON, 105, 105, CANNOT_EVOLVE},
    {89, "Shellder", WATER, 30, 65, CAN_EVOLVE},
    {90, "Cloyster", WATER, 50, 95, CANNOT_EVOLVE},
    {91, "Gastly", GHOST, 30, 35, CAN_EVOLVE},
    {92, "Haunter", GHOST, 45, 50, CAN_EVOLVE},
    {93, "Gengar", GHOST, 60, 65, CANNOT_EVOLVE},
    {94, "Onix", ROCK, 35, 45, CANNOT_EVOLVE},
    {95, "Drowzee", PSYCHIC, 60, 48, CAN_EVOLVE},
    {96, "Hypno", PSYCHIC, 85, 73, CANNOT_EVOLVE},
    {97, "Krabby", WATER, 30, 105, CAN_EVOLVE},
    {98, "Kingler", WATER, 55, 130, CANNOT_EVOLVE},
    {99, "Voltorb", ELECTRIC, 40, 30, CAN_EVOLVE},
    {100, "Electrode", ELECTRIC, 60, 50, CANNOT_EVOLVE},
    {101, "Exeggcute", GRASS, 60, 40, CAN_EVOLVE},
    {102, "Exeggutor", GRASS, 95, 95, CANNOT_EVOLVE},
    {103, "Cubone", GROUND, 50, 50, CAN_EVOLVE},
    {104, "Marowak", GROUND, 60, 80, CANNOT_EVOLVE},
    {105, "Hitmonlee", FIGHTING, 50, 120, CANNOT_EVOLVE},
    {106, "Hitmonchan", FIGHTING, 50, 105, CANNOT_EVOLVE},
    {107, "Lickitung", NORMAL, 90, 55, CANNOT_EVOLVE},
    {108, "Koffing", POISON, 40, 65, CAN_EVOLVE},
    {109, "Weezing", POISON, 65, 90, CANNOT_EVOLVE},
    {110, "Rhyhorn", GROUND, 80, 85, CAN_EVOLVE},
    {111, "Rhydon", GROUND, 105, 130, CANNOT_EVOLVE},
    {112, "Chansey", NORMAL, 250, 5, CANNOT_EVOLVE},
    {113, "Tangela", GRASS, 65, 55, CANNOT_EVOLVE},
    {114, "Kangaskhan", NORMAL, 105, 95, CANNOT_EVOLVE},
    {115, "Horsea", WATER, 30, 40, CAN_EVOLVE},
    {116, "Seadra", WATER, 55, 65, CANNOT_EVOLVE},
    {117, "Goldeen", WATER, 45, 67, CAN_EVOLVE},
    {118, "Seaking", WATER, 80, 92, CANNOT_EVOLVE},
    {119, "Staryu", WATER, 30, 45, CAN_EVOLVE},
    {120, "Starmie", WATER, 60, 75, CANNOT_EVOLVE},
    {121, "Mr. Mime", PSYCHIC, 40, 45, CANNOT_EVOLVE},
    {122, "Scyther", BUG, 70, 110, CANNOT_EVOLVE},
    {123, "Jynx", ICE, 65, 50, CANNOT_EVOLVE},
    {124, "Electabuzz", ELECTRIC, 65, 83, CANNOT_EVOLVE},
    {125, "Magmar", FIRE, 65, 95, CANNOT_EVOLVE},
    {126, "Pinsir", BUG, 65, 125, CANNOT_EVOLVE},
    {127, "Tauros", NORMAL, 75, 100, CANNOT_EVOLVE},
    {128, "Magikarp", WATER, 20, 10, CAN_EVOLVE},
    {129, "Gyarados", WATER, 95, 125, CANNOT_EVOLVE},
    {130, "Lapras", WATER, 130, 85, CANNOT_EVOLVE},
    {131, "Ditto", NORMAL, 48, 48, CANNOT_EVOLVE},
    {132, "Eevee", NORMAL, 55, 55, CAN_EVOLVE},
    {133, "Vaporeon", WATER, 130, 65, CANNOT_EVOLVE},
    {134, "Jolteon", ELECTRIC, 65, 65, CANNOT_EVOLVE},
    {135, "Flareon", FIRE, 65, 130, CANNOT_EVOLVE},
    {136, "Porygon", NORMAL, 65, 60, CANNOT_EVOLVE},
    {137, "Omanyte", ROCK, 35, 40, CAN_EVOLVE},
    {138, "Omastar", ROCK, 70, 60, CANNOT_EVOLVE},
    {139, "Kabuto", ROCK, 30, 80, CAN_EVOLVE},
    {140, "Kabutops", ROCK, 60, 115, CANNOT_EVOLVE},
    {141, "Aerodactyl", ROCK, 80, 105, CANNOT_EVOLVE},
    {142, "Snorlax", NORMAL, 160, 110, CANNOT_EVOLVE},
    {143, "Articuno", ICE, 90, 85, CANNOT_EVOLVE},
    {144, "Zapdos", ELECTRIC, 90, 90, CANNOT_EVOLVE},
    {145, "Moltres", FIRE, 90, 100, CANNOT_EVOLVE},
    {146, "Dratini", DRAGON, 41, 64, CAN_EVOLVE},
    {147, "Dragonair", DRAGON, 61, 84, CAN_EVOLVE},
    {148, "Dragonite", DRAGON, 91, 134, CANNOT_EVOLVE},
    {149, "Mewtwo", PSYCHIC, 106, 110, CANNOT_EVOLVE},
    {150, "Mew", PSYCHIC, 100, 100, CANNOT_EVOLVE}
};

#endif
