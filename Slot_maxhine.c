#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void){

    const char *symbols[] = {"🍒","🍋", "🔔", "💎", "7️⃣", "👑"};
    const int symbol_count = sizeof(symbols) / sizeof(symbols[0]);
    char choice;

    srand( (unsigned int) time(NULL));

    printf("🎰 Welcome to the Slot Machine! 🎰\n");

    do{
        int i1 = rand() % symbol_count;
        int i2 = rand() % symbol_count;
        int i3 = rand() % symbol_count;

        printf("\nSpinning...\n");
        printf("| %s | %s | %s |\n", symbols[i1], symbols[i2], symbols[i3]);

        if (i1 == i2 && i2 ==i3){
            printf("🎉 JACKPOT! Allthree match!\n");
        }
        else if(i1 == i2 || i2 == i3 || i1 == i3){
            printf("✨ Nice! You got two matching symbols!\n");
        }
        else{
            printf("😊 No match. Try again!");
        }
        printf("\nPlay again? (y/n): ");
        scanf(" %c", &choice);
    }
    while (choice == 'y' || choice == 'Y');

    printf("\nThanks for playing! 🎲\n");

}