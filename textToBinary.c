#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARR 300
#define ARR_SIZE 8

typedef struct{
  int id;// a number from 1 to 36
  int reason;//reason for being absent, number from 1-28:
  int month;//Month of Absence – a number from 1 to 12, where Jan = 1 and Dec = 12
  int dayOfWeek;//2=Mon, 3=Tues, 4=Wed, 5=Thurs, 6 = Fri
  int season;// 1=Winter, 2=Spring, 3=Summer, 4=Fall
  int transExpense;//Transportation Expense – an integer representing the monthly cost of transportation to work
  int distance;// Distance from Residence to Work – an integer representing the km traveled to work
  int serviceTime;//number of years worked at the company
  int age; //the age of the employee
  float workDays ;//the normal minutes worked per day by the employee
  int hitTarget;//an estimate of their performance ... a percentage
  int disciplinary;//1 if has been disciplined, 0 otherwise
  int education;//1=high school, 2=graduate, 3=postgraduate, 4=masters or doctorate
  int numChildren;//Number of Children – number of children the employee has;
  int drinker;//Social Drinker – 1 if yes, 0 if no
  int smoker;//SocialSmoker – 1 if yes,0 if no
  int numPets;//number of pets the employee has
  int weight;//Weight – integer (Kg)
  int height;//Height – integer (cm)
  int bmi;//Body Mass Index
  int hours;//hours absent during this episode
}TextData;

typedef struct{
  unsigned char id;
  char          month;//Month of Absence – a number from 1 to 12, where Jan = 1 and Dec = 12
  char          dayOfWeek;//Mon, 3=Tues, 4=Wed, 5=Thurs, 6 = Fri
  char          season;// W=Winter, P=Spring, S=Summer, F=Fall
  unsigned char age; //the age of the employee
  char          education[ARR_SIZE];//HS’=high school, ‘GR’=graduate, ‘PG’=postgraduate,  ‘MD‘=masters or doctorate
  unsigned char numChildren;//Number of Children – number of children the employee has;
  char          drinker;//Social Drinker – Y if yes,N if no
  char          smoker;//SocialSmoker – Y if yes,N if no
  unsigned char numPets;//number of pets the employee has
  unsigned char bmi;//Body Mass Index
  unsigned char hours;//hours absent during this episode
}BinaryData;

int main() {
  FILE  *tfd, *bfd;//tfd for text file for reading ,bfd for binary file for writing.
  TextData *textData;
  BinaryData *binaryData;
  unsigned int count = 0,fileRow = 0;

  //Open the text file for reading
  tfd = fopen("absenteeism.txt", "r");
  //check for errors in opening the file
  if (!tfd) {
    printf("ERROR: Could not open the text file\n");
    exit(1);
  }

  //read the row of the file
  while(!feof(tfd))
  {
    if(fgetc(tfd) == '\n')
      count++;
  }
  fileRow = count + 1;
  // printf("row = %d\n", fileRow);//for debug

  textData = malloc(fileRow * sizeof(TextData));
  binaryData = malloc(fileRow * sizeof(BinaryData));

  // Position back to the start
  fseek(tfd, 0, SEEK_SET);
  count = 0;

  //skip first line in text file
  while(!feof(tfd))
  {
    if(fgetc(tfd) == '\n')
      break;
  }


  // Open a new file for writing only
  bfd = fopen("absenteeism.bin", "w");
  // rewind(*bfd);
  if (!bfd) {
    printf("ERROR: Could not open the binary file\n");
    exit(1);
  }

  char seasons[] = "WPSF";
  char edu[] = "HS,GR,PG,MD";
  char social[] = "NY";

  while (!feof(tfd)) {
    if(fscanf(tfd,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%f;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\n",
    &textData[count].id, &textData[count].reason, &textData[count].month,
    &textData[count].dayOfWeek,&textData[count].season, &textData[count].transExpense,
    &textData[count].distance, &textData[count].serviceTime,&textData[count].age,
    &textData[count].workDays, &textData[count].hitTarget,&textData[count].disciplinary,
    &textData[count].education,&textData[count].numChildren, &textData[count].drinker,
    &textData[count].smoker, &textData[count].numPets,&textData[count].weight,
    &textData[count].height, &textData[count].bmi, &textData[count].hours) == EOF){
      break;
    }

    //add data to binaryData
    binaryData[count].id = textData[count].id + '0';
    binaryData[count].month = textData[count].month + '0';
    binaryData[count].dayOfWeek = textData[count].dayOfWeek + '0';
    binaryData[count].season = seasons[textData[count].season-1];
    binaryData[count].age = textData[count].age +'0';//  unsigned char
    binaryData[count].education[0] = edu[3*(textData[count].education-1)];
    binaryData[count].education[1] = edu[3*(textData[count].education-1)+1];
    binaryData[count].education[2] = 0;
    binaryData[count].numChildren = textData[count].numChildren + '0';
    binaryData[count].drinker = social[textData[count].drinker];
    binaryData[count].smoker = social[textData[count].smoker];
    binaryData[count].numPets = textData[count].numPets + '0';
    binaryData[count].bmi = textData[count].bmi + '0';
    binaryData[count].hours = textData[count].hours + '0';
    count++;
  }

    unsigned short int size  = count;

    fwrite(&size, sizeof(unsigned short int),1, bfd);
    for(int i=0 ; i< count;++i){

      fwrite("\n", sizeof(unsigned char), 1, bfd);
      fwrite(&binaryData[i].id, sizeof(unsigned char), 1, bfd);
      fwrite(&binaryData[i].month, sizeof(char), 1, bfd);
      fwrite(&binaryData[i].dayOfWeek, sizeof(char), 1, bfd);
      fwrite(&binaryData[i].season, sizeof(char), 1, bfd);
      fwrite(&binaryData[i].age, sizeof(unsigned char), 1, bfd);
      fwrite(&binaryData[i].education, sizeof(char), 2, bfd);
      fwrite(&binaryData[i].numChildren, sizeof(unsigned char), 1, bfd);
      fwrite(&binaryData[i].drinker, sizeof(char), 1, bfd);
      fwrite(&binaryData[i].smoker, sizeof(char), 1, bfd);
      fwrite(&binaryData[i].numPets, sizeof(unsigned char), 1, bfd);
      fwrite(&binaryData[i].bmi, sizeof(unsigned char), 1, bfd);
      fwrite(&binaryData[i].hours, sizeof(unsigned char), 1, bfd);
    }

  free(textData);
  free(binaryData);

  fclose(tfd);
  fclose(bfd);
  printf("Text To Binary, Done!\n");
}
