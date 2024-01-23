#include <pthread.h>
#include <stdio.h>
#include <string.h>

//this called first
//this is the function that generates bulk file to be split into for equal smaller files
void stringGen()
{
    const char* theStrings = "test";//generates the string to be printed to the file

    //create input file
    const char* input_file = "All_strings.txt";

    //open the input file
    FILE* inputFILE = fopen("All_strings.txt", "w");

    int SIZE = strlen(theStrings);//getting string size

    //here we are creating a file with size of 200 MB and determining the amount of times the print to file should be run
    int fileSIZE = 200 * 1024 * 1024;
    int timesRun = fileSIZE / (SIZE + 1);
    
    //load the strings into the file
    for(size_t i = 0; i < timesRun; i++)
    {//here print the string along with a space
        fputs(theStrings, inputFILE);
        fputc(' ', inputFILE);
    }
    fclose(inputFILE);//closing the file
}

//this is called by stringSplit function
//we use this function to obtain the string count of the bulk file so we can divide the strings to the respective other files based on the amout
//of strings in the bulk file
int getStringCount(const char *fileName) {
    FILE* inputFILE = fopen(fileName, "r");//opens the file with a specific name

    char buffer[100];//characther buffer
    int word_count = 0;//variable to keep track of the word count in the file

    while (fscanf(inputFILE, "%s", buffer) != EOF) {//while loop to continue to read strings till the end of the file
        word_count++;//after each read iterate the word count once
    }

    fclose(inputFILE);//close file
    return word_count;//return the word count
}

//this is the third function run
//this is the function that splits the bulk file into 4 equal smaller files
void stringSplit()
{
    //setting variable for the bulk file to be passed into the getStringCount function
    const char* inputFileName = "All_strings.txt";
    int totalWordCount = getStringCount(inputFileName);//obtaining the string count for bulk file to be divided by 4

    FILE* inputFILE = fopen("All_strings.txt", "r");//opening bulk file

    FILE* outputFILES[4];//creating array of 4 output files
    const char* NEWfilenames[4] = {"output1.txt", "output2.txt", "output3.txt", "output4.txt"};//creating respective file names

    for (int j = 0; j < 4; j++) { //for loop to open all 4 file for input
        outputFILES[j] = fopen(NEWfilenames[j], "w");
    }

    char buff[2056]; //character buffer for reading in 
    int currentFile = 0;//keeping track of each file 
    int wordsPerFile = totalWordCount / 4; // Calculate the target number of words per file using integer division
    int wordsWritten = 0;//keeping track of amount of words that has currently been written into each respective file 

    while (fscanf(inputFILE, "%s", buff) != EOF) {//while we have not reached the end of the bulk file
        fprintf(outputFILES[currentFile], "%s ", buff);//print string to output file
        wordsWritten++;//increment once
        
        if (wordsWritten >= wordsPerFile) {//checks to see if the amount of words currently written to the file has meet its max which is wordsPerFile
            currentFile = (currentFile + 1) % 4;//if so increment current file to next
            wordsWritten = 0;//reset wordsWritten variable for next file
        }
    }

    fclose(inputFILE);//close bulk file

    for (int i = 0; i < 4; i++) { //close output files
        fclose(outputFILES[i]);
    }
}

void *runner(void *param);      /* threads call this function */
 
//making the struct that will hold the variables needed to be passed in
struct threadValues{
    const char *NameOfFile;// to deal with the names of each output file
};

int main()
{
    stringGen();//calling function to create bulk file
    stringSplit();//calling function to seperate bulk file into 4 equal distributions

    const char* inputFileName = "All_strings.txt";//assigning var for name of bulk file "All_strings.txt"
    int totalWordCount = getStringCount(inputFileName);//this is the total sum of the words contained in the All_strings.txt file

    pthread_t thread1;  //thread identifier
    pthread_t thread2;  //thread identifier
    pthread_t thread3;  //thread ident 3
    pthread_t thread4;  //threat ident 4
    pthread_attr_t attr;   //set of thread attributes

    threadValues threadData[4];//setting the variables for each thread to each file
    threadData[0].NameOfFile = "output1.txt";//each var has its respective file
    threadData[1].NameOfFile = "output2.txt";
    threadData[2].NameOfFile = "output3.txt";
    threadData[3].NameOfFile = "output4.txt";

    //setting the thread identifiers to each thread info
    pthread_attr_init(&attr);  //set the default attributes of the thread 
    pthread_create(&thread1, &attr, runner, &threadData[0]); //thread1 = output1.txt   //create the thread we and assign the file name to argument, in our case the ouptut files 
    pthread_create(&thread2, &attr, runner, &threadData[1]); //thread2 = output2.txt
    pthread_create(&thread3, &attr, runner, &threadData[2]); //threat3 = output3.txt
    pthread_create(&thread4, &attr, runner, &threadData[3]); //threat4 = output4.txt

    pthread_join(thread1, NULL);  //wait for thread to exit 
    pthread_join(thread2, NULL);  //wait for thread to exit 
    pthread_join(thread3, NULL); 
    pthread_join(thread4, NULL);

    printf("Total word count from main file All_strings.txt: %d\n", totalWordCount);//printing the total word count of the All_strings file
    //all output files sum to be the total of the All_strings.txt file

    //printing the thread IDs
    printf("thread: %ld\n", thread1);
    printf("thread: %ld\n", thread2);
    printf("thread: %ld\n", thread3);
    printf("thread: %ld\n", thread4); 
}


/* this is the function the threads execute */
void *runner(void *param)
{
    //setting the pointer the created struct
    struct threadValues *thread_data = (struct threadValues *)param;

    //creating a pointer specifically to the NameOfFile to have the ability to access all 
    const char *filename = thread_data->NameOfFile;

    //opening the files 
    FILE *file = fopen(filename, "r");//opening file based on var filename
    char buffer[2560];//characther buffer for reading in the bulk file by chunks
    char wordBuff[256];//word buffer array used to count each word as it is read in
    int word_count = 0;//keeping track of word count

    while (1) {//while loop that breaks when the end of file is reached
        int bytesRead = fread(buffer, 1, 2560, file);//var for counting amount of chars/bytes read in
        if (bytesRead == 0) { //if at end of file where bytes read = 0 break out
            break;  // End of file
        }

        int bufferPos = 0;//used to keep track of buffer position

        while (bufferPos < bytesRead) {//while bufferPos is less than the amout of bytes read into the buffer continue
            int wordPos = 0;//keeps track of word pos in wordBuff array
            while (bufferPos < bytesRead && wordPos < 2056 - 1) { //keep going until we have reached the end of the buffer
                char c = buffer[bufferPos++];//increments bufferPos after reading a char
                if (c == ' ' || c == '\t' || c == '\n') { //if the character being looked at is either a space, tab, or newline
                    if (wordPos > 0) { //also if we are at the end of the word
                        wordBuff[wordPos] = '\0';//insert a null terminator to indicate the end of a word
                        word_count++;//increment word_count
                        wordPos = 0;//reset wordPos
                    }
                } else {//else if it is a char that is attached a string we indicate that here
                    wordBuff[wordPos++] = c;
                }
            }
        }
    } 

    printf("Word count for %s: %d\n", thread_data->NameOfFile, word_count); //outputs the word count of the respective file
    fclose(file);//close each respective file

    pthread_exit(0);//exit
}

