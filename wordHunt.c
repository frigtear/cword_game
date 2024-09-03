#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_CHARACTERS 10000
#define RAND_CAP 30000
#define ALPHA_SIZE 26
#define LOOPS 10
#define SEED 100
#define LETTERCOUNT 8

static int findWord(FILE* stream, const char* word) {
    char* buffer = (char*)malloc(MAX_CHARACTERS);
    char* result = NULL;
    long pos = 0;
    int count = 0;

    while (fgets(buffer, MAX_CHARACTERS, stream) != NULL) {
        int bufferWordLength = 0;
        while(buffer[bufferWordLength] != '\n'){
            bufferWordLength ++;
        }
        count ++;
        int longer = (bufferWordLength > strlen(word)) ? bufferWordLength : strlen(word);
        if (strncasecmp(buffer, word, longer) == 0) {
            pos = ftell(stream); 
            result = (char*)malloc(strlen(buffer) + 1);
            if (result == NULL) {
                perror("result malloc failure");
                exit(EXIT_FAILURE);
            }
            strcpy(result, buffer);
            
            break;
        }
    }
    printf("Scanned %d words, found %s at position %ld\n",count,word,pos);
    free(buffer);
    rewind(stream);
    if (result != NULL) {
        return 0;
    } else {
        return 1;
    }
}


static char* generate_characters(const int len,long seed){
    char* alphabet = "abcdefghijklmnopqrstuvwxyz";
    char* letters = (char*) malloc( (len+1) * (sizeof(char) ));
    srand(seed);
    for(int i = 0;i<len;i++){
        int choice = rand()%ALPHA_SIZE;
        letters[i] = alphabet[choice];
    }
    letters[len] = '\0';
    puts(letters);
    return letters;
}


static int getScore(char* guess){
    int score = 0;

    int values[ALPHA_SIZE] = {
        /* A */ 1, /* B */ 3, /* C */ 3, /* D */ 2, /* E */ 1,
    /* F */ 4, /* G */ 2, /* H */ 4, /* I */ 1, /* J */ 8,
    /* K */ 5, /* L */ 1, /* M */ 3, /* N */ 1, /* O */ 1,
    /* P */ 3, /* Q */ 10, /* R */ 1, /* S */ 1, /* T */ 1,
    /* U */ 1, /* V */ 4, /* W */ 4, /* X */ 8, /* Y */ 4,
    /* Z */ 10
    };

    for(int i = 0;i<strlen(guess);i++){
        char let = tolower(guess[i]);
        if (let >= 'a' && let <= 'z'){
            int index = (let - 'a');
            score += values[index];
        }
    }

    return score;
}


static int checkAllowed(char* guess, char* allowed){
    if (strlen(guess) < 4){
        return 1;
    }

    int isAllowed = 1;
    for(int i = 0;i<strlen(guess);i++){
        for(int j = 0;j<strlen(allowed);j++){
            if (guess[i] == allowed[j]){
                isAllowed = 0;
                break;
            }
        }

        if (isAllowed != 0){
            return 1;
        }
        isAllowed = 1;
    }
    return 0;
}

int main() {
    FILE* file = fopen("words.txt", "r");
    if (file == NULL) {
        perror("Cant open file");
        return -1;
    }

    char* letters = generate_characters(LETTERCOUNT,SEED);
    int loop_count = 0;
    int total_score = 0;

    while(loop_count < LOOPS){
        printf("Given letters\n");
        puts(letters);

        char in[MAX_CHARACTERS];
        fscanf(stdin,"%s", in);
        puts(in);
        int inLen = 0;
        while(in[inLen] != '\0'){
            inLen ++;
        }

        char guess[inLen];
        for(int i = 0;i<inLen;i++){
            guess[i] = in[i];
        }


        if (checkAllowed(guess,letters) == 0){
            if (findWord(file,guess) == 0){
                int addedScore = getScore(guess);
                total_score += addedScore;
                printf("%s exists!!! Adding %d to you score. Your total score is %d",guess,addedScore,total_score);
            }
            else{
                printf("Cant find the word\n");
            }
        }else{
            printf("That word is not allowed\n");
        }
        

        loop_count ++;
    }

    printf("Game over!, your final score was %d",total_score);
    fclose(file);
    return 0;
}
