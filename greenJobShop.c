#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double *cost, *times, total_time, total_cost;
long double processing_time;
int machines, jobs, threshold, **scheduling;

void quick_sort(double value[], int index[],int left, int right){
    int i, j, aux_index;
    double pivot, aux_value;

    i = left;
    j = right;
    pivot = value[(left + right) / 2];

    while(i <= j){
        while(value[i] > pivot && i < right){
            i++;
        }
        while(value[j] < pivot && j > left){
            j--;
        }
        if(i <= j){
            aux_value = value[i];
            aux_index = index[i];
            value[i] = value[j];
            index[i] = index[j];
            value[j] = aux_value;
            index[j] = aux_index;
            i++;
            j--;
        }
    }
    if(j > left)
        quick_sort(value,index,left,j);
    if(i < right)
        quick_sort(value,index,i,right);

}

void schedule(int job, int machine){
  int i;
  for(i = 0; i<jobs; i++){
    if(scheduling[machine][i] == -1){
        scheduling[machine][i] = job;
        break;
    }
  }
}

double completition_time(int machine){
  int i; int count = 0;
  for(i = 0; i<jobs; i++){
    if(scheduling[machine][i] != -1){
        count = count + times[i];
    }
  }
  return count;
}


void greedy_schedule(){
  int i,j,p, priority, best_machine, machine_index[machines], job_index[jobs];
  double  completition_time[machines], impact[machines];

  //INITALIZING VALUES
  priority = 0;

  for(i=0; i < machines; i ++){
    completition_time[i] = 0;
    machine_index[i] = i;
  }
  for(i=0; i < jobs; i ++){
    job_index[i] = i;
  }
  quick_sort(times,job_index,0,jobs-1);
  for (i = jobs-1; i >= 0; i--){
    printf("PARA J%d\n", job_index[i]);
    //CHECK WHETHER THE TIME OR THE COST IS THE PRIORITY
    printf("CUSTO TOTAL = %.2f\n", total_cost);
    printf("TEMPO TOTAL = %.2f\n", total_time);
    printf("CUSTO TOTAL/LIMITE = %.2f\n", total_cost/threshold);
    printf("JOBS/TOTAL JOBS = %.2f\n", (float)(jobs-1-i)/jobs);
    if((total_cost/threshold) > ((float)(jobs-1-i)/jobs)){
      printf("PRIORIDADE: custo\n" );
      priority=1;
    }
    else{
      printf("PRIORIDADE: tempo\n" );
      priority=0;
    }

    for(j = 0; j < machines; j++){

      for(p=0; p < machines; p ++){
        machine_index[p] = p;
      }
      //IN THE FIRST ITERATION ONLY TAKES THE MACHINE COST IN ACCOUNT
      if(i==jobs-1){
        impact[j] = cost[j];
      }
      //IN OTHER ITERATIONS CONSIDERATES COST OR TIME IMPACTS
      else{

        //CHECK THE PRIORITY TO CALCULATE IMPACT
        if(priority == 0){
          impact[j] = ((completition_time[j] + times[i])/total_time)*100;
          printf("IMPACTO M%d: %.2f\n",j,impact[j] );
        }else{
          impact[j] = ((cost[j]*times[i])/total_cost)*100;
          printf("IMPACTO M%d: %.2f\n",j,impact[j] );
        }
        if(total_cost + cost[j]*times[i] > threshold){
          impact[j] = 9999;
          printf("IMPACTO M%d: %.2f\n",j,impact[j] );
        }
      }
    }

    //SELECTING THE BEST CANDIDATE MACHINE
    quick_sort(impact,machine_index,0,machines-1);
    best_machine = machine_index[machines-1];
    printf("ESCOLHA: M%d\n", machine_index[machines-1]);

    //SCHEDULING AND UPDATING THE TOTAL COST AND TOTAL TIME
    schedule(job_index[i],best_machine);
    total_cost = total_cost + times[i]*cost[best_machine];
    completition_time[best_machine] = completition_time[best_machine] + times[i];
    total_time = total_time + completition_time[best_machine] + times[i];
    printf("\n");
  }

}

void read_data(char *file_name){
    FILE *fp;
    fp = fopen(file_name, "rb");
    char c;
    char buff[256];
    int  i = 0, j = 0, p = 0, q = 0;
    int line_break = 0;
    char value[10];

    while((c = fgetc(fp)) != EOF){
      if(c == 10 || c == 13 || c == 32){
        if(line_break == 0){
          line_break = 1;
          buff[i] = '\0';
          i = 0;
          if(j == 0){
            machines = atoi(buff);
            cost = (double*) malloc (machines * sizeof(double));
            j++;
          }
          else if(j == 1){
            jobs = atoi(buff);
            times = (double*) malloc (jobs * sizeof(double));
            j++;
          }
          else if(j > 1 && j <= 1 + machines){
            cost[p] = atof(buff);
            p++;
          }
          else if(j > 1 + machines && j <= 1 + machines + jobs){
            cost[q] = atof(buff);
            q++;
          }
        }
      }else {
        line_break = 0;
        buff[i] = c;
        i++;
        if(i >= 256){
          fprintf(stderr,"Buffer overflow.\n");
          break;
        }
      }

    }
    threshold = atoi(buff);
    fclose(fp);
}



void initialize(){
  int i,j;
  total_time = 0;
  total_cost = 0;
  scheduling = (int**)malloc(machines * sizeof(int*));
  for (i = 0; i < machines; i++){
       scheduling[i] = (int*) malloc(jobs * sizeof(int));
  }
  for (i = 0; i < machines; i++){
       for (j = 0; j < jobs; j++){
            scheduling[i][j] = -1;
       }
  }
}

void print_solution(char name[]){
  int i, j;
  printf("%s\n", name);
  if(total_cost > threshold){
    printf("Solução inviável!\n");
      printf("total cost: %.02f\n",total_cost);
  }else{
    for(i = 0; i < machines; i++){
      printf("m%d:",i);
      for (j = 0; j < jobs; j++){
        if(scheduling[i][j]!= -1){
            printf(" j%d,",scheduling[i][j]);
        }
      }
      printf("\n");
    }
    printf("total time: %.02f\n",total_time);
    printf("total cost: %.02f\n",total_cost);
    printf("processing time: %Lf s\n",processing_time);
  }
}

int main(int argc, char *argv[]){
  if(argc != 2){
		fprintf(stderr,"Filename expected as an argument.\n");
		return 1;
	}
  int i,j;
  clock_t t;
  read_data(argv[1]);
  initialize();
  t = clock();
  greedy_schedule();
  t = clock() - t;
  processing_time = ((double)t)/CLOCKS_PER_SEC;
  print_solution(argv[1]);
  free(times);
  free(cost);
  free(scheduling);
}
