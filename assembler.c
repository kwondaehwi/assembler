#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*******************************************************
 * Function Declaration
 *
 *******************************************************/
char *change_file_ext(char *str);

/*******************************************************
 * Function: main
 *
 * Parameters:
 *  int
 *      argc: the number of argument
 *  char*
 *      argv[]: array of a sting argument
 *
 * Return:
 *  return success exit value
 *
 * Info:
 *  The typical main function in C language.
 *  It reads system arguments from terminal (or commands)
 *  and parse an assembly file(*.s).
 *  Then, it converts a certain instruction into
 *  object code which is basically binary code.
 *
 *******************************************************/

char* data = {"0001000000000000"};

typedef struct hex
{
    char* name;
    char* Bnum;
}hexnum;

hexnum Hex[];

hexnum Hex[] =
{
    {"0", "0000"}, {"1", "0001"}, {"2", "0010"}, {"3", "0011"}, {"4", "0100"}, {"5", "0101"},
    {"6", "0110"}, {"7", "0111"}, {"8", "1000"}, {"9", "1001"}, {"a", "1010"}, {"b", "1011"},
    {"c", "1100"}, {"d", "1101"}, {"e", "1110"}, {"f", "1111"}
};

typedef struct regi
{
    char* name;
    char* Bnum;
}regis;

regis Reg[];

regis Reg[] = 
{
    { "$0", "00000"}, { "$1", "00001"}, { "$2", "00010"}, { "$3", "00011"}, { "$4", "00100"},
    { "$5", "00101"}, { "$6", "00110"}, { "$7", "00111"}, { "$8", "01000"}, { "$9", "01001"},
    { "$10", "01010"}, { "$11", "01011"}, { "$12", "01100"}, { "$13", "01101"}, { "$14", "01110"},
    { "$15", "01111"}, { "$16", "10000"}, { "$17", "10001"}, { "$18", "10010"}, { "$19", "10011"},
    { "$20", "10100"}, { "$21", "10101"}, { "$22", "10110"}, { "$23", "10111"}, { "$24", "11000"},
    { "$25", "11001"}, { "$26", "11010"}, { "$27", "11011"}, { "$28", "11100"}, { "$29", "11101"},
    { "$30", "11110"}, { "$31", "11111"}
};

typedef struct command{
    char* name;
    char* type;
    int token;
    char* op;
    char* func;
    char* shamt;
}commands;

commands Com[];

commands Com[] =
{
    {
        "addu", "r", 3, "000000", "100001", "00000" //
    },
    {
        "and", "r", 3, "000000", "100100", "00000" //
    },
    {
        "or", "r", 3, "000000", "100101", "00000" //
    },
    {
        "sll", "r", 3, "000000", "000000", "00000" ///
    },
    {
        "srl", "r", 3, "000000", "000010", "00000" ///
    },
    {
        "nor", "r", 3, "000000", "100111", "00000" //
    },
    {
        "subu", "r", 3, "000000", "100011", "00000" //
    },
    {
        "jr", "r", 1, "000000", "001000", "00000" ///
    },
    {
        "sltu", "r", 3, "000000", "101011", "00000" //
    },
    {
        "addiu", "i", 3, "001001", "000000", "00000" ////
    },
    {
        "bne", "i", 3, "000101", "000000", "00000" /////
    },
    {
        "beq", "i", 3, "000100", "000000", "00000" /////
    },
    {
        "ori", "i", 3, "001101", "000000", "00000" ////
    },
    {
        "andi", "i", 3, "001100", "000000", "00000" ////
    },
    {
        "lui", "i", 3, "001111", "000000", "00000" //////
    },
    {
        "lw", "i", 3, "100011", "000000", "00000" ///////
    },
    {
        "sw", "i", 3, "101011", "000000", "00000" ///////
    },
    {
        "j", "j", 3, "000010", "000000", "00000" ///
    },
    {
        "jal", "j", 3, "000011", "000000", "00000" 
    },
    {
        "sltiu", "i", 3, "001011", "000000", "00000" ////
    },
    {
        "la", "a", 3, "000000", "000000", "00000" //
    }
};

void printHex(char k, FILE* output)
{
    switch(k)
    {
        case '0': fprintf(output, "%s", Hex[0].Bnum); break;
        case '1': fprintf(output, "%s", Hex[1].Bnum); break;
        case '2': fprintf(output, "%s", Hex[2].Bnum); break;
        case '3': fprintf(output, "%s", Hex[3].Bnum); break;
        case '4': fprintf(output, "%s", Hex[4].Bnum); break;
        case '5': fprintf(output, "%s", Hex[5].Bnum); break;
        case '6': fprintf(output, "%s", Hex[6].Bnum); break;
        case '7': fprintf(output, "%s", Hex[7].Bnum); break;
        case '8': fprintf(output, "%s", Hex[8].Bnum); break;
        case '9': fprintf(output, "%s", Hex[9].Bnum); break;
        case 'a': fprintf(output, "%s", Hex[10].Bnum); break;
        case 'b': fprintf(output, "%s", Hex[11].Bnum); break;
        case 'c': fprintf(output, "%s", Hex[12].Bnum); break;
        case 'd': fprintf(output, "%s", Hex[13].Bnum); break;
        case 'e': fprintf(output, "%s", Hex[14].Bnum); break;
        case 'f': fprintf(output, "%s", Hex[15].Bnum); break;
    }
}

void datasec(char*** argvt, int* count, FILE* output)
{
    for(int i = 0; i < (*count); i++)
    {
        if(strcmp((*argvt)[i], ".word") == 0)
        {
            if(strncmp((*argvt)[i + 1], "0x", 2) == 0) // hex immediate
            {
                if(strlen((*argvt)[i+1]) == 3)
                {
                    fprintf(output, "%s", "0000000000000000000000000000");
                    for(int a = 2; a < strlen((*argvt)[i + 1]); a++)
                    {
                        printHex((*argvt)[i+1][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+1]) == 4)
                {
                    fprintf(output, "%s", "000000000000000000000000");
                    for(int a = 2; a < strlen((*argvt)[i + 1]); a++)
                    {
                        printHex((*argvt)[i+1][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+1]) == 5)
                {
                    
                    fprintf(output, "%s", "00000000000000000000");
                    for(int a = 2; a < strlen((*argvt)[i + 1]); a++)
                    {
                        printHex((*argvt)[i+1][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+1]) == 6)
                {
                    fprintf(output, "%s", "0000000000000000");
                    for(int a = 2; a < strlen((*argvt)[i + 1]); a++)
                    {
                        printHex((*argvt)[i+1][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+1]) == 7)
                {
                    fprintf(output, "%s", "000000000000");
                    for(int a = 2; a < strlen((*argvt)[i + 1]); a++)
                    {
                        printHex((*argvt)[i+1][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+1]) == 8)
                {
                    fprintf(output, "%s", "00000000");
                    for(int a = 2; a < strlen((*argvt)[i + 1]); a++)
                    {
                        printHex((*argvt)[i+1][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+1]) == 9)
                {
                    fprintf(output, "%s", "0000");
                    for(int a = 2; a < strlen((*argvt)[i + 1]); a++)
                    {
                        printHex((*argvt)[i+1][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+1]) == 10)
                {
                    for(int a = 2; a < strlen((*argvt)[i + 1]); a++)
                    {
                        printHex((*argvt)[i+1][a], output);
                    }                    
                }
            }
            else // dec immediate
            {
                char dec[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                int temp = atoi((*argvt)[i + 1]);
                
                for(int n = 0; temp > 0; n++)
                {
                    dec[n] = temp%2;
                    temp = temp/2;
                }
                for(int l = 31;  l >= 0; l--)
                {
                    fprintf(output, "%d", dec[l]);
                }
            }
        }
    }

}

void fprint(char*** argvt, int* count, FILE* output)
{
    for(int i = 0; i < (*count); i++)
    {

        for(int j = 0; j < 20; j++) //opcode
        {
            if(strcmp((*argvt)[i], Com[j].name) == 0)
            {
                fprintf(output, "%s", Com[j].op);
            }
        }
        //addu, and, or, nor, subu, sltu

        if(strcmp((*argvt)[i], Com[0].name) == 0 || strcmp((*argvt)[i], Com[1].name) == 0
        || strcmp((*argvt)[i], Com[2].name) == 0 || strcmp((*argvt)[i], Com[5].name) == 0
        || strcmp((*argvt)[i], Com[6].name) == 0 || strcmp((*argvt)[i], Com[8].name) == 0)
        {
            for(int j = 0; j < 32; j++) //rs
            {
                if(strcmp((*argvt)[i + 2], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            for(int j = 0; j < 32; j++) //rt
            {
                if(strcmp((*argvt)[i + 3], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            for(int j = 0; j < 32; j++) //rd
            {
                if(strcmp((*argvt)[i + 1], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            for(int j = 0; j < 21; j++) //shamt
            {
                if(strcmp((*argvt)[i], Com[j].name) == 0)
                {
                    fprintf(output,"%s", Com[j].shamt);
                }
            }
            for(int j = 0; j < 21; j++) //func
            {
                if(strcmp((*argvt)[i], Com[j].name) == 0)
                {
                    fprintf(output,"%s", Com[j].func);
                }
            }
            
        }
        if(strcmp((*argvt)[i], "la") == 0) //la
        {
            
            for(int j = 0; j < i; j++) 
            {
                if(strcmp((*argvt)[i + 2], (*argvt)[j]) == 0)
                {
                    int w = 0;
                    for(int q = 0; q < j; q++)
                    {
                        if(strcmp((*argvt)[q], ".word") == 0)
                        {
                            w++;
                        }
                    }
                    w = w*4;
                    if(w == 0)
                    {
                        char* rs = {"00000"};
                        fprintf(output, "%s", Com[14].op); //opcode
                        fprintf(output, "%s", rs); //rs
                        for(int p = 0; p < 32; p++) //rt
                        {
                            if(strcmp((*argvt)[i + 1], Reg[p].name) == 0)
                            {
                                fprintf(output,"%s", Reg[p].Bnum);
                            }
                        }
                        fprintf(output, "%s", data); //immediate
                    }
                    else
                    {
        
                        char imm[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
                        for(int a = 0; w > 0; a++)
                        {
                            imm[a] = w % 2;
                            w = w/2;
                        }
                        char* rs = {"00000"}; //lui
                        fprintf(output, "%s", Com[14].op); //opcode
                        fprintf(output, "%s", rs); //rs
                        for(int p = 0; p < 32; p++) //rt
                        {
                            if(strcmp((*argvt)[i + 1], Reg[p].name) == 0)
                            {
                                fprintf(output,"%s", Reg[p].Bnum);
                            }
                        }
                        fprintf(output, "%s", data); //immediate

                        fprintf(output, "%s", Com[12].op); // ori opcode
                        for(int p = 0; p < 32; p++) //rs
                        {
                            if(strcmp((*argvt)[i + 1], Reg[p].name) == 0)
                            {
                                fprintf(output,"%s", Reg[p].Bnum);
                            }
                        }
                        for(int p = 0; p < 32; p++) //rt
                        {
                            if(strcmp((*argvt)[i + 1], Reg[p].name) == 0)
                            {
                                fprintf(output,"%s", Reg[p].Bnum);
                            }
                        }
                        for(int b = 15;  b >= 0; b--)
                        {
                            fprintf(output, "%d", imm[b]); //immediate
                        }
                    }
                }
            }
        }
        // sll, srl

        if(strcmp((*argvt)[i], Com[3].name) == 0 || strcmp((*argvt)[i], Com[4].name) == 0)
        {
            char* rs = {"00000"};
            fprintf(output, "%s", rs); //rs
            for(int j = 0; j < 32; j++) //rt
            {
                if(strcmp((*argvt)[i + 2], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            for(int j = 0; j < 32; j++) //rd
            {
                if(strcmp((*argvt)[i + 1], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            int temp = atoi((*argvt)[i+3]);
            char samt[5] = {0,0,0,0,0};
            for(int a = 0; temp > 0; a++)
            {
                samt[a] = temp%2;
                temp = temp/2;
            }
            for(int a = 4;  a >= 0; a--)
            {
                fprintf(output, "%d", samt[a]);  //shamt
            }
            for(int j = 0; j < 21; j++) //func
            {
                if(strcmp((*argvt)[i], Com[j].name) == 0)
                {
                    fprintf(output, "%s", Com[j].func);
                }
            }
        }
        //jr

        if(strcmp((*argvt)[i], Com[7].name) == 0)
        {
            for(int j = 0; j < 32; j++) //rs
            {
                if(strcmp((*argvt)[i + 1], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            fprintf(output, "00000"); //rt
            fprintf(output, "00000"); //rd
            fprintf(output, "00000"); //shamt
            fprintf(output, "%s", Com[7].func);  //func  
        }

        //addiu, ori, andi, sltiu
        if(strcmp((*argvt)[i], Com[9].name) == 0 || strcmp((*argvt)[i], Com[12].name) == 0
        || strcmp((*argvt)[i], Com[13].name) == 0 || strcmp((*argvt)[i], Com[19].name) == 0)
        {
            for(int j = 0; j < 32; j++) //rs
            {
                if(strcmp((*argvt)[i + 2], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            for(int j = 0; j < 32; j++) //rt
            {
                if(strcmp((*argvt)[i + 1], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }

            if(strncmp((*argvt)[i + 3], "0x", 2) == 0) // hex immediate
            {
                if(strlen((*argvt)[i+3]) == 3)
                {
                    fprintf(output, "%s", "000000000000");
                    for(int a = 2; a < strlen((*argvt)[i + 3]); a++)
                    {
                        printHex((*argvt)[i+3][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+3]) == 4)
                {
                    fprintf(output, "%s", "00000000");
                    for(int a = 2; a < strlen((*argvt)[i + 3]); a++)
                    {
                        printHex((*argvt)[i+3][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+3]) == 5)
                {
                    fprintf(output, "%s", "0000");
                    for(int a = 2; a < strlen((*argvt)[i + 3]); a++)
                    {
                        printHex((*argvt)[i+3][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+3]) == 6)
                {
                    for(int a = 2; a < strlen((*argvt)[i + 3]); a++)
                    {
                        printHex((*argvt)[i+3][a], output);
                    }                    
                }
            }
            else // dec immediate
            {
                char dec[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                int temp = atoi((*argvt)[i + 3]);
                
                for(int n = 0; temp > 0; n++)
                {
                    dec[n] = temp%2;
                    temp = temp/2;
                }
                for(int l = 15;  l >= 0; l--)
                {
                    fprintf(output, "%d", dec[l]);
                }
            }
        }

        //bne, beq
        if(strcmp((*argvt)[i], Com[10].name) == 0 || strcmp((*argvt)[i], Com[11].name) == 0)
        {
            for(int j = 0; j < 32; j++) //rs
            {
                if(strcmp((*argvt)[i + 1], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            for(int j = 0; j < 32; j++) //rt
            {
                if(strcmp((*argvt)[i + 2], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }

            for(int j = 0; j < i; j++)  // N immediate
            {
                
                if(strcmp((*argvt)[j], (*argvt)[i+3]) == 0)
                {
                    int ttt = 0;
                    for(int k = j + 1; k <= i; k++)
                    {
                        for(int m = 0; m < 21; m++) 
                        {
                            if(strcmp((*argvt)[k], Com[m].name) == 0)
                            {
                                ttt++;
                            }
                        }
                    }
                    ttt--;
                    char adr[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    for(int n = 0; ttt > 0; n++)
                    {
                        adr[n] = ttt%2;
                        ttt = ttt/2;
                    }
                    for(int n = 0;  n <= 15; n++)
                    {
                        if(adr[n] == 0)
                        {
                            adr[n] = 1;
                        }
                        else if(adr[n] == 1)
                        {
                            adr[n] = 0;
                        }
                    }
                    for(int l = 15;  l >= 0; l--)
                    {
                        fprintf(output, "%d", adr[l]);
                    }

                }
            }
            for(int j = i + 4; j < (*count); j++) // P immediate
            {
                
                if(strcmp((*argvt)[j], (*argvt)[i+3]) == 0)
                {
                    int ttt = 0;
                    for(int k = i + 4; k <= j; k++)
                    {
                        for(int m = 0; m < 21; m++) 
                        {
                            if(strcmp((*argvt)[k], Com[m].name) == 0)
                            {
                                ttt++;
                            }
                        }
                    }
                    char adr[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    for(int n = 0; ttt > 0; n++)
                    {
                        adr[n] = ttt%2;
                        ttt = ttt/2;
                    }
                    for(int l = 15;  l >= 0; l--)
                    {
                        fprintf(output, "%d", adr[l]);
                    }

                }
            }
        }
        if(strcmp((*argvt)[i], Com[14].name) == 0) // lui
        {
            fprintf(output, "00000"); //rs

            for(int j = 0; j < 32; j++) //rt
            {
                if(strcmp((*argvt)[i + 1], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }
            
            if(strncmp((*argvt)[i + 2], "0x", 2) == 0) // hex immediate
            {
                if(strlen((*argvt)[i+2]) == 3)
                {
                    fprintf(output, "%s", "000000000000");
                    for(int a = 2; a < strlen((*argvt)[i + 2]); a++)
                    {
                        printHex((*argvt)[i+2][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+2]) == 4)
                {
                    fprintf(output, "%s", "00000000");
                    for(int a = 2; a < strlen((*argvt)[i + 2]); a++)
                    {
                        printHex((*argvt)[i+2][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+2]) == 5)
                {
                    fprintf(output, "%s", "0000");
                    for(int a = 2; a < strlen((*argvt)[i + 2]); a++)
                    {
                        printHex((*argvt)[i+2][a], output);
                    }                    
                }
                else if(strlen((*argvt)[i+2]) == 6)
                {
                    for(int a = 2; a < strlen((*argvt)[i + 2]); a++)
                    {
                        printHex((*argvt)[i+2][a], output);
                    }                    
                }
            }
            else // dec immediate
            {
                char dec[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                int temp = atoi((*argvt)[i + 2]);
                
                for(int n = 0; temp > 0; n++)
                {
                    dec[n] = temp%2;
                    temp = temp/2;
                }
                for(int l = 15;  l >= 0; l--)
                {
                    fprintf(output, "%d", dec[l]);
                }
            }

        }
        // lw, sw
        if(strcmp((*argvt)[i], Com[15].name) == 0 || strcmp((*argvt)[i], Com[16].name) == 0)
        {
            int a = 0;
            char* temp = (char*)malloc(sizeof(char)*1000);
            strcpy(temp, (*argvt)[i + 2]);
            char** Narr = (char**)malloc(sizeof(char*)*100);
            Narr[a] = (char*)malloc(sizeof(char)*1000);
            Narr[a] = strtok(temp, "(");
            while(Narr[a]!=NULL)
            {
                
                a++;
                Narr[a] = (char*)malloc(sizeof(char)*1000);
                Narr[a] = strtok(NULL, "(");
            }
            char* New = strtok(Narr[1], ")");
            
            for(int j = 0; j < 32; j++) //rs
            {
                if(strcmp(New, Reg[j].name) == 0)
                {
                    fprintf(output, "%s", Reg[j].Bnum);
                }
            }
            for(int j = 0; j < 32; j++) //rt
            {
                if(strcmp((*argvt)[i + 1], Reg[j].name) == 0)
                {
                    fprintf(output,"%s", Reg[j].Bnum);
                }
            }

            int arnum = atoi(Narr[0]);
            if(arnum < 0)                       // N num
            {
                arnum = arnum * (-1);
                arnum--;
                char Nnum[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                for(int n = 0; arnum > 0; n++)
                {
                    Nnum[n] = arnum%2;
                    arnum = arnum/2;
                }
                for(int n = 0;  n <= 15; n++)
                {
                    if(Nnum[n] == 0)
                    {
                        Nnum[n] = 1;
                    }
                    else if(Nnum[n] == 1)
                    {
                        Nnum[n] = 0;
                    }
                }
                for(int l = 15;  l >= 0; l--)
                {
                    fprintf(output,"%d", Nnum[l]);
                }

            }
            else
            {
                char Pnum[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //P num
                for(int n = 0; arnum > 0; n++)
                {
                    Pnum[n] = arnum%2;
                    arnum = arnum/2;
                }
                for(int l = 15;  l >= 0; l--)
                {
                    fprintf(output, "%d", Pnum[l]);
                }
            }
            
        }
        if(strcmp((*argvt)[i], Com[17].name) == 0) //j
        {         
            for(int k = 0; k < i; k++)
            {
                if(strcmp((*argvt)[i + 1], (*argvt)[k]) == 0)
                {
                    
                    char jump[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
                    int bbb = 0;
                    int la = 0;
                    for(int p = 0; p < k + 1; p++)
                    {
                        if(strcmp((*argvt)[p], Com[20].name) == 0)
                        {
                            la++;
                        }
                        for(int q = 0; q < 21; q++)
                        {
                            if(strcmp((*argvt)[p], Com[q].name) == 0)
                            {
                                bbb++;
                            }
                        }
                    }
                    la = la - 1;
                    if(la > 0)
                    {
                        bbb = bbb + la;
                    }
                    for(int n = 0; bbb > 0; n++)
                    {
                        jump[n] = bbb%2;
                        bbb = bbb/2;
                    }
                    for(int l = 25;  l >= 0; l--)
                    {
                       fprintf(output, "%d", jump[l]);
                    }
                }
            }
            for(int k = i + 2; k < (*count); k++)
            {
                if(strcmp((*argvt)[i + 1], (*argvt)[k]) == 0)
                {
                    char jump[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
                    int bbb = 0;
                    int la = 0;
                    for(int p = 0; p < k + 1; p++)
                    {
                        if(strcmp((*argvt)[p], Com[20].name) == 0)
                        {
                            la++;
                        }
                        for(int q = 0; q < 21; q++)
                        {
                            if(strcmp((*argvt)[p], Com[q].name) == 0)
                            {
                                bbb++;
                            }   
                        }
                    }
                    la = la - 1;
                    if(la > 0)
                    {
                        bbb = bbb + la;
                    }
                    
                    for(int n = 0; bbb > 0; n++)
                    {
                        jump[n] = bbb%2;
                        bbb = bbb/2;
                    }
                    for(int l = 25;  l >= 0; l--)
                    {
                       fprintf(output, "%d", jump[l]);
                    }
                }
            }
        }

        if(strcmp((*argvt)[i], Com[18].name) == 0) //jal
        {
            for(int k = 0; k <= i; k++)
            {
                if(strcmp((*argvt)[i + 1], (*argvt)[k]) == 0)
                {
                    char jump[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
                    int bbb = 0;
                    for(int p = 0; p < k + 1; p++)
                    {
                        for(int q = 0; q < 21; q++)
                        {
                            if(strcmp((*argvt)[p], Com[q].name) == 0)
                            {
                                bbb++;
                            }   
                        }
                    }
                    for(int n = 0; bbb > 0; n++)
                    {
                        jump[n] = bbb%2;
                        bbb = bbb/2;
                    }
                    for(int l = 25;  l >= 0; l--)
                    {
                       fprintf(output, "%d", jump[l]);
                    }
                }
            }
            for(int k = i + 2; k < (*count); k++)
            {
                if(strcmp((*argvt)[i + 1], (*argvt)[k]) == 0)
                {
                    char jump[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
                    int bbb = 0;
                    for(int p = 0; p < k + 1; p++)
                    {
                        for(int q = 0; q < 21; q++)
                        {
                            if(strcmp((*argvt)[p], Com[q].name) == 0)
                            {
                                bbb++;
                            }   
                        }
                    }
                    for(int n = 0; bbb > 0; n++)
                    {
                        jump[n] = bbb%2;
                        bbb = bbb/2;
                    }
                    for(int l = 25;  l >= 0; l--)
                    {
                       fprintf(output, "%d", jump[l]);
                    }
                }
            }
        }
    }
    
}

void textB (char*** argvt, int* count, FILE* output)
{
    int num = 0;
    int la = 0;
    
    char textB[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < (*count); i++)
    {
        for(int j = 0; j < 21; j++)
        {
            if(strcmp((*argvt)[i],Com[j].name)==0)
            {
                num++;
            }
        }
        if(strcmp((*argvt)[i],Com[20].name)==0)
        {
            la++;
        }
    }
    la = la - 1;
    if(la > 0)
    {
        num = num + la;
    }
    
    num = num*4;
    for(int k = 0; num > 0; k++)
    {
        textB[k] = num%2;
        num = num/2;
    }
    for(int l = 31;  l >= 0; l--)
    {
        fprintf(output, "%d", textB[l]);
    }
}

int dataB (char*** argvt, int* count, FILE* output)
{
    int num = 0;
    char dataB[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < (*count); i++)
    {
        if(strcmp((*argvt)[i], ".word") == 0)
        {
            num++;
        }
    }
    num = num*4;
    
    for(int k = 0; num > 0; k++)
    {
        dataB[k] = num%2;
        num = num/2;
    }
    for(int j = 31;  j >=0; j--)
    {
        fprintf(output, "%d", dataB[j]);
    }
}

void parse_command(char** arr,char*** argvt, int* count)
{
    int i = 0;
    int s = 0;
    int e = 0;
    int com = 0;
    int col = 0;
    int dot = 0;
    int tab = 0;
    char** ptrE = (char**)malloc(sizeof(char*)*1000);
    char** ptrTab = (char**)malloc(sizeof(char*)*1000);
    char** ptrS = (char**)malloc(sizeof(char*)*1000);
    char** ptrCom = (char**)malloc(sizeof(char*)*1000);
    char** ptrCol = (char**)malloc(sizeof(char*)*1000);
    char** ptrD = (char**)malloc(sizeof(char*)*1000);
    
    for(i; arr[i]; i++)
    {
        char* temp = (char*)malloc(sizeof(char)*1000);
        strcpy(temp, arr[i]);
        ptrTab[tab] = strtok(temp, "\t");
        while(ptrTab[tab] != NULL)
        {
            tab++;
            ptrTab[tab] = strtok(NULL, "\t");
            
        }
    }
    for(int k =0; k < tab; k++)
    {
        ptrE[e] = strtok(ptrTab[k], "\n");
        while(ptrE[e] != NULL)
        {
            e++;
            ptrE[e] = strtok(NULL, "\n");
        }
    }
    for(int j =0; j < e; j++)
    {
        ptrS[s]=strtok(ptrE[j], " ");
        while(ptrS[s] != NULL)
        {
            s++;
            ptrS[s] = strtok(NULL, " ");
        }
    }
    for(int l =0; l < s; l++)
    {
        ptrCom[com] = strtok(ptrS[l],",");
        while(ptrCom[com] != NULL)
        {
            com++;
            ptrCom[com] = strtok(NULL, ",");
        }
    }
    for(int m = 0; m < com; m++)
    {
        ptrCol[col] = strtok(ptrCom[m], ":");
        while(ptrCol[col] != NULL)
        {
            col++;
            ptrCol[col] = strtok(NULL, ":");
        }
    }
    for(int n = 0; n < col; n++)
    {
        (*argvt)[n] = (char*)malloc(sizeof(char)*1000);
        strcpy((*argvt)[n],ptrCol[n]);
    }
    (*count) = col;
}

int
main(int argc, char *argv[])
{
    FILE *input, *output;
    char *filename;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <*.s>\n", argv[0]);
        fprintf(stderr, "Example: %s sample_input/example?.s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    filename = strdup(argv[1]); // strdup() is not a standard C library but fairy used a lot.
    if(change_file_ext(filename) == NULL) {
        fprintf(stderr, "'%s' file is not an assembly file.\n", filename);
        exit(EXIT_FAILURE);
    }
    
    output = fopen(filename, "w");
    if (output == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
    int SIZE = 1024;
    char str[SIZE];
    char** arr = (char**)malloc(sizeof(char*)*1000);
    char** argvt = (char**)malloc(sizeof(char*)*1000);
    int count = 0;
    int i = 0;
    str[i] = (char*)malloc(sizeof(char)*1000);
    while(fgets(str, SIZE, input))
    {
        arr[i] = (char*)malloc(sizeof(char)*1000);
        strcpy(arr[i], str);
        i++;
    }
    parse_command(arr, &argvt, &count);
    textB(&argvt, &count, output);
    dataB(&argvt, &count, output); 
    fprint(&argvt, &count, output);
    datasec(&argvt, &count, output);
    fprintf(output, "\n");
    
   

    fclose(input);
    fclose(output);
    exit(EXIT_SUCCESS);
}


/*******************************************************
 * Function: change_file_ext
 *
 * Parameters:
 *  char
 *      *str: a raw filename (without path)
 *
 * Return:
 *  return NULL if a file is not an assembly file.
 *  return empty string
 *
 * Info:
 *  This function reads filename and converst it into
 *  object extention name, *.o
 *
 *******************************************************/
char
*change_file_ext(char *str)
{
    char *dot = strrchr(str, '.');

    if (!dot || dot == str || (strcmp(dot, ".s") != 0)) {
        return NULL;
    }

    str[strlen(str) - 1] = 'o';
    return "";
}
