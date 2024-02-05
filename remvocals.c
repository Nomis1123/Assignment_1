#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) 
{

  if (argc <= 2)
  {
    printf("ERROR not enough args provided\r\n");
    return -1;
  }

  char* sourcewav = argv[1];
  char* destwav = argv[2];
  
  FILE *sourcefp = fopen(sourcewav, "r");
  if (sourcefp == NULL)
  {
    printf("ERROR input file does not exist\r\n");
    return -1;
  }
  
  FILE *destfp = fopen(destwav, "w+");

  

  char readbuff[44];
  size_t sizeread = fread(readbuff, 44, 1, sourcefp);

  if (sizeread != 1)
  {
    printf("ERROR file read error\r\n");
    fclose(sourcefp);
    fclose(destfp);
    return -1;
  }

   size_t sizewrite = fwrite(readbuff, 44, 1, destfp);
  
  if (sizewrite != 1)
  {
    printf("ERROR file write error\r\n");
    fclose(sourcefp);
    fclose(destfp);
    return -1;
    
  }


  while(1)
    {
      short first;
      short second;

      size_t firstread = fread(&first, sizeof(short), 1, sourcefp);
      if (firstread != 1)
      {
        break;
      }
      
      size_t secondread = fread(&second, sizeof(short), 1, sourcefp);
      if (secondread != 1)
      {
        break;
      }

      short total = (first - second) / 2;

      firstread = fwrite(&total, sizeof(short), 1, destfp);
      secondread = fwrite(&total, sizeof(short), 1, destfp);

      
    }
  
  fclose(sourcefp);
  fclose(destfp);


  return 0;
}
