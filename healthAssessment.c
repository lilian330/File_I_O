#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_SIZE 36

typedef struct{
  unsigned char       id;
  unsigned char       age;
  char                education[3];
  unsigned char       numChildren;
  char                socialDrinker;
  char                socialSmoker;
  unsigned char       numPets;
  unsigned char       bodyMassIndex;//Body mass index;
  unsigned short int  totalHoursAbsent;
} PersonalData;


typedef struct{
  unsigned char       id;
  char                month;
  char                dayOfWeek;
  char                season;
  unsigned short int  hoursAbsent;
} AbsentEpisode;

void printInfo(PersonalData**, int*, int*, int*,int,int);
void swap(PersonalData **,PersonalData **);
void sort(PersonalData **);


int main(){
  FILE  *fd;
  AbsentEpisode *absent[ID_SIZE];
  PersonalData  *person[ID_SIZE];
  for(int i = 0;i<ID_SIZE;++i){
    person[i] = (PersonalData *)malloc(sizeof(PersonalData));
    absent[i] = malloc(sizeof(AbsentEpisode));
  }
  int weekHours[] = {0,0,0,0,0,0,0};
  int seasonHours[] = {0,0,0,0};
  int monthHours[] = {0,0,0,0,0,0,0,0,0,0,0,0};
  int totalHours = 0;
  //Open the binary file for reading
  fd = fopen("absenteeism.bin", "r");
  //check for errors in opening the file
  if (!fd) {
    printf("ERROR: Could not open the text file\n");
    exit(1);
  }

  unsigned short int size;
  fread(&size, sizeof(unsigned short int), 1, fd);
  fgetc(fd);//next line;

  //read the binary file and load the information.
  int count = 0;
  while (!feof(fd)) {
    unsigned char id,age,numChildren,numPets,bmi,hours;
    char month,dayOfWeek,season,education[3],drinker,smoker;
    //read the information one by one
    fread(&id, sizeof(unsigned char), 1, fd);
    fread(&month, sizeof(char), 1, fd);
    fread(&dayOfWeek, sizeof(char), 1, fd);
    fread(&season, sizeof(char), 1, fd);
    fread(&age, sizeof(unsigned char), 1, fd);
    fread(&education, sizeof(unsigned char), 2, fd);
    fread(&numChildren, sizeof(unsigned char), 1, fd);
    fread(&drinker, sizeof(char), 1, fd);
    fread(&smoker, sizeof(char), 1, fd);
    fread(&numPets, sizeof(unsigned char), 1, fd);
    fread(&bmi, sizeof(unsigned char), 1, fd);
    fread(&hours, sizeof(unsigned char), 1, fd);
    //check if contains current id
    int index = -1;
    for(int i = 0 ;i < ID_SIZE;++i){
      if(person[i]->id  == id){
        index = i;
        break;
      }
    }
    //if current id is new id,add id,count++
    if (index == -1)
    {
      index = count;
      count++;
    }
    //store Personal Data
    person[index]->id  = id;
    absent[index]->id = id;
    absent[index]->month = month;
    absent[index]->dayOfWeek = dayOfWeek;
    absent[index]->season = season;
    absent[index]->hoursAbsent = hours - '0';
    person[index]->age  = age;
    person[index]->education[0] = education[0];
    person[index]->education[1] = education[1];
    person[index]->education[2] = 0;
    person[index]->numChildren  = numChildren;
    person[index]->socialDrinker  = drinker;
    person[index]->socialSmoker  = smoker;
    person[index]->numPets  = numPets;
    person[index]->bodyMassIndex  = bmi;
    person[index]->totalHoursAbsent  += absent[index]->hoursAbsent;

    int hoursAb = absent[index]->hoursAbsent ;
    int s = -1;
    switch (season) {
      case 'W':
        s = 0;break;
      case 'P':
        s = 1;break;
      case 'S':
        s = 2;break;
      case 'F':
        s = 3;break;
      default:
        break;
    }
    seasonHours[s] += hoursAb;
    weekHours[dayOfWeek-'0'] += hoursAb;
    int m = month -'0';
    monthHours[m-1] += hoursAb;
    totalHours += hoursAb;
    fgetc(fd);//next line;
  }

  // sort the person array
  sort(person);

  //print the information
  printInfo(person,weekHours,monthHours,seasonHours,totalHours,size);
  for(int i = 0;i<ID_SIZE;++i){
    free(absent[i]);
    free(person[i]);
  }

  fclose(fd);
}


void swap(PersonalData **p1,PersonalData **p2) {
  PersonalData *temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

void sort(PersonalData **arr){
  for (int outloop = 0; outloop < ID_SIZE-1; outloop++)
  {
    for(int inloop = 0; inloop < ID_SIZE-1-outloop; inloop++)
    {
      if(arr[inloop]->totalHoursAbsent < arr[inloop+1]->totalHoursAbsent)
      {
        swap(&arr[inloop],&arr[inloop+1]);
      }
    }
  }
}


void printInfo(PersonalData **person,int * weekHours,int * monthHours,int * seasonHours,int totalHours,int size){
    printf("\nThe file contains %d records\n", size);
    printf("\nIndividual Employee Data:\n");
    printf("-------------------------\n");
    printf("\nID%*sABSENT%*sAGE%*sSMOKER%*sDRINKER%*s#CH%*s#PT%*sCATEGORY%*sEDU\n",3,"",3,"",3,"",3,"",3,"",3,"",3,"",4,"");
    printf("------------------------------------------------------------------\n");
    for(int i = 0;i<ID_SIZE;++i){
      int id = person[i]->id - '0';
      int age = person[i]->age - '0';
      int numChildren = person[i]->numChildren-'0';
      int numPets = person[i]->numPets -'0';
      int bmi = person[i]->bodyMassIndex-'0';
        printf("%2d%*s%3d%*s%2d%*s",id,3,"",person[i]->totalHoursAbsent,6,"",age,4,"");
        if(person[i]->socialSmoker=='Y'){
          printf("%-6c%*s",person[i]->socialSmoker,3,"");
        }else{
          printf("%*s",9,"");
        }
        if(person[i]->socialDrinker == 'Y'){
          printf("%-7c%*s",person[i]->socialDrinker,3,"");
        }else{
          printf("%*s",10,"");
        }
        printf("%-3d%*s%-3d%*s",numChildren,3,"",numPets,3,"");
        if( bmi< 18.5){
          printf("%-10s", "Underweight");
        }else if( bmi >25 &&  bmi<=30){
          printf("%-10s", "Overweight");
        }else if( bmi> 30){
          printf("%-10s", "Obese");
        }else{
          printf("%-10s","");
        }
        printf("%*s%-3s\n",2,"",person[i]->education);
    }

    printf("\nGrand Total Hours Absent: %d\n",totalHours);
    printf("\nHours absent based on weekday:\n");
    printf("------------------------------\n");
    printf("  Sat: %d\n",weekHours[0]);
    printf("  Sun: %d\n",weekHours[1]);
    printf("  Mon: %d\n",weekHours[2]);
    printf("  Tue: %d\n",weekHours[3]);
    printf("  Wed: %d\n",weekHours[4]);
    printf("  Thu: %d\n",weekHours[5]);
    printf("  Fri: %d\n",weekHours[6]);

    printf("\nHours absent based on season:\n");
    printf("-----------------------------\n");
    printf("  Winter: %d\n",seasonHours[0]);
    printf("  Spring: %d\n",seasonHours[1]);
    printf("  Summer: %d\n",seasonHours[2]);
    printf("  Fall  : %d\n",seasonHours[3]);

    printf("\nHours absent based on month:\n");
    printf("----------------------------\n");
    printf("  Jan: %d\n",monthHours[0]);
    printf("  Feb: %d\n",monthHours[1]);
    printf("  Mar: %d\n",monthHours[2]);
    printf("  Apr: %d\n",monthHours[3]);
    printf("  May: %d\n",monthHours[4]);
    printf("  Jun: %d\n",monthHours[5]);
    printf("  Jul: %d\n",monthHours[6]);
    printf("  Aug: %d\n",monthHours[7]);
    printf("  Sep: %d\n",monthHours[8]);
    printf("  Oct: %d\n",monthHours[9]);
    printf("  Nov: %d\n",monthHours[10]);
    printf("  Dec: %d\n",monthHours[11]);

}
