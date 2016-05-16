#include <stdio.h>
#include <stdlib.h>


#define MACHINES 2
#define JOBS 5
#define THRESHOLD 1600

double cost[MACHINES] = {15,8};
double times[JOBS] = {25,38,37,42,14};

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
  int i,j,p, priority, best_machine, sort_index[MACHINES], schedule[JOBS];
  double completition_time[MACHINES],total_time, total_cost, cost_impact,
    time_impact, impact[MACHINES];

  //INITALIZING VALUES
  total_time = 0;
  total_cost = 0;
  priority = 0;
  for(i=0; i < JOBS; i ++){
    schedule[i] = -1;
  }
  for(j=0; j < MACHINES; j ++){
    completition_time[j] = 0;
    sort_index[j] = j;
  }

  for (i = 0; i < JOBS; i++){
    //CHECK WHETHER THE TIME OR THE COST IS THE PRIORITY
    if((total_cost/THRESHOLD) > ((float)i/JOBS)){
      priority=1;
    }
    else{
      priority=0;
    }

    for(j = 0; j < MACHINES; j++){

      for(p=0; p < MACHINES; p ++){
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
    quick_sort(impact,sort_index,0,MACHINES-1);
    best_machine = sort_index[MACHINES-1];

    //SCHEDULING AND UPDATING THE TOTAL COST AND TOTAL TIME
    schedule[i] = best_machine;
    total_cost = total_cost + times[i]*cost[best_machine];
    completition_time[best_machine] = completition_time[best_machine]  + times[i];
    if(completition_time[best_machine] > total_time){
      total_time = completition_time[best_machine];
    }

  }
  //PRINT THE RESULT
  for (p = 0; p < JOBS; p++){
    printf("j%d na m%d\n",p,schedule[p]);
  }
  printf("total time: %.02f\n",total_time);
  printf("total cost: %.02f\n",total_cost);

  printf("\n\n");

}

int main(){
    greedy_schedule();
}
