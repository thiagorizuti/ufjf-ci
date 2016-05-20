#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double *cost, *times;
int machines, jobs, threshold;

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

void greedy_schedule(){
  int i,j,p, priority, best_machine, sort_index[machines], schedule[jobs];
  double completition_time[machines],total_time, total_cost, cost_impact,
    time_impact, impact[machines];

  //INITALIZING VALUES
  total_time = 0;
  total_cost = 0;
  priority = 0;
  for(i=0; i < jobs; i ++){
    schedule[i] = -1;
  }
  for(j=0; j < machines; j ++){
    completition_time[j] = 0;
    sort_index[j] = j;
  }

  for (i = 0; i < jobs; i++){
    //CHECK WHETHER THE TIME OR THE COST IS THE PRIORITY
    if((total_cost/threshold) > ((float)i/jobs)){
      priority=1;
    }
    else{
      priority=0;
    }

    for(j = 0; j < machines; j++){

      for(p=0; p < machines; p ++){
        sort_index[p] = p;
      }
      //IN THE FIRST ITERATION ONLY TAKES THE MACHINE COST IN ACCOUNT
      if(i==0){
        impact[j] = cost[j];
      }
      //IN OTHER ITERATIONS CONSIDERATES COST AND TIME IMPACTS
      else{
        cost_impact = ((total_cost + cost[j]-total_cost)/total_cost)*100;
        if(completition_time[j] + times[i] > total_time){
          time_impact = ((completition_time[j] + times[i]-total_time)/total_time)*100;
        }else{
          time_impact = 0.0;
        }

        //CHECK THE PRIORITY TO CALCULATE IMPACT
        if(priority == 0){
          impact[j] = time_impact;
        }else{
          impact[j] = cost_impact;
        }
      }

    }
    //SELECTING THE BEST CANDIDATE MACHINE
    quick_sort(impact,sort_index,0,machines-1);
    best_machine = sort_index[machines-1];

    //SCHEDULING AND UPDATING THE TOTAL COST AND TOTAL TIME
    schedule[i] = best_machine;
    total_cost = total_cost + times[i]*cost[best_machine];
    completition_time[best_machine] = completition_time[best_machine]  + times[i];
    if(completition_time[best_machine] > total_time){
      total_time = completition_time[best_machine];
    }

  }
  //PRINT THE RESULT
  for (p = 0; p < jobs; p++){
    printf("j%d na m%d\n",p,schedule[p]);
  }
  printf("total time: %.02f\n",total_time);
  printf("total cost: %.02f\n",total_cost);

  printf("\n\n");

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
          if(j > 1 && j <= 1 + machines){
            cost[p] = atof(buff);
            p++;
          }
          if(j > 1 + machines && j <= 1 + machines + jobs){
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
}

int main(int argc, char *argv[]){
  if(argc != 2){
		fprintf(stderr,"Filename expected as an argument.\n");
		return 1;
	}
  int i;
  read_data(argv[1]);
  printf("%d, %d, %d\n", machines, jobs, threshold);
  printf("MACHINES:\n");
  for(i=0;i<machines;i++){
    printf("%.2f\n",cost[i]);
  }
  printf("JOBS:\n");
  for(i=0;i<jobs;i++){
    printf("%.2f\n",times[i]);
  }

  greedy_schedule();
}
