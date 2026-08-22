#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct TimePeriod {
    int year;
    char name[50];
    char description[150];
    struct TimePeriod* next;
    struct TimePeriod* prev;
} TimePeriod;


TimePeriod* createPeriod(int year, const char* name, const char* desc) {
    TimePeriod* newPeriod = (TimePeriod*)malloc(sizeof(TimePeriod));
    if (!newPeriod) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newPeriod->year = year;
    strcpy(newPeriod->name, name);
    strcpy(newPeriod->description, desc);
    newPeriod->next = newPeriod;
    newPeriod->prev = newPeriod;
    return newPeriod;
}

void addPeriod(TimePeriod** head, int year, const char* name, const char* desc) {
    TimePeriod* newPeriod = createPeriod(year, name, desc);
    if (*head == NULL) {
        *head = newPeriod;
        return;
    }

    TimePeriod* tail = (*head)->prev;

    tail->next = newPeriod;
    newPeriod->prev = tail;
    newPeriod->next = *head;
    (*head)->prev = newPeriod;
}


typedef struct StackNode {
    TimePeriod* period;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} TimeStack;

void initStack(TimeStack* stack) {
    stack->top = NULL;
}

void push(TimeStack* stack, TimePeriod* period) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) return;
    newNode->period = period;
    newNode->next = stack->top;
    stack->top = newNode;
}

TimePeriod* pop(TimeStack* stack) {
    if (stack->top == NULL) return NULL;
    StackNode* temp = stack->top;
    TimePeriod* period = temp->period;
    stack->top = stack->top->next;
    free(temp);
    return period;
}

void printHistory(TimeStack* stack) {
    if (stack->top == NULL) {
        printf("\n[History] No time jumps recorded yet!\n");
        return;
    }
    printf("\n--- TIME TRAVEL JUMP HISTORY (STACK) ---\n");
    StackNode* current = stack->top;
    int step = 1;
    while (current != NULL) {
        printf("%d. Year %d: %s\n", step++, current->period->year, current->period->name);
        current = current->next;
    }
    printf("---------------------------------------\n");
}


void freeAll(TimePeriod* head, TimeStack* stack) {

    while (stack->top != NULL) {
        pop(stack);
    }


    if (head != NULL) {
        TimePeriod* current = head;
        TimePeriod* temp;
        head->prev->next = NULL;
        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
    }
}


int main() {
    TimePeriod* timeline = NULL;
    TimeStack history;
    initStack(&history);


    addPeriod(&timeline, -2560, "Ancient Egypt", "The Great Pyramids are under construction!");
    addPeriod(&timeline, 800, "Viking Era", "Norse explorers set sail across uncharted seas.");
    addPeriod(&timeline, 1889, "Industrial Revolution", "Steam engines roar and factory smoke fills the sky.");
    addPeriod(&timeline, 2026, "Present Day", "AI agents dominate tech, and humans dream of Mars.");
    addPeriod(&timeline, 3050, "Cybernetic Future", "Neon skyscrapers touch the clouds, flying vehicles everywhere.");

    TimePeriod* current = timeline;
    int choice;

    printf("=============================================\n");
    printf("   WELCOME TO THE TIME WARP ADVENTURE ENGINE \n");
    printf("=============================================\n");

    do {
        printf("\n---------------------------------------------\n");
        printf("CURRENT ERA: [%d AD/BC] %s\n", current->year, current->name);
        printf("DETAILS: %s\n", current->description);
        printf("---------------------------------------------\n");
        printf("1. Travel Forward in Time  (Next Node)\n");
        printf("2. Travel Backward in Time (Prev Node)\n");
        printf("3. Perform Time Warp Jump  (Push to Stack History)\n");
        printf("4. Rewind Last Jump        (Pop from Stack History)\n");
        printf("5. View Jump History\n");
        printf("6. Exit Game\n");
        printf("Enter action: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                current = current->next;
                printf("\n>> Step forward into the future...\n");
                break;
            case 2:
                current = current->prev;
                printf("\n>> Step back into the past...\n");
                break;
            case 3:
                push(&history, current);
                printf("\n>> TIME WARP JUMP RECORDED! Location stored in Stack.\n");
                break;
            case 4: {
                TimePeriod* lastJump = pop(&history);
                if (lastJump != NULL) {
                    current = lastJump;
                    printf("\n>> REWIND SUCCESSFUL! Returned to [%d] %s.\n", current->year, current->name);
                } else {
                    printf("\n>> REWIND FAILED! No jumps recorded in the stack.\n");
                }
                break;
            }
            case 5:
                printHistory(&history);
                break;
            case 6:
                printf("\nExiting Time Warp Machine... Goodbye traveler!\n");
                break;
            default:
                printf("\nInvalid choice! Space-time continuum intact.\n");
        }
    } while (choice != 6);

    freeAll(timeline, &history);
    return 0;
}

