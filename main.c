
// liabaries needed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h" // checks memory leak 


// declare structs 
typedef struct customer{

  int ctime; //time of arrival
  char *name; // last name
  int lineNum;// line number where the customer is standing
  int serNum; //number of services

}customer;

typedef struct cnode{

  customer* cdata;
  struct cnode* next;

}cnode;

typedef struct queue{

  cnode *front;
  cnode *back;

}queue;

//function declarations 
customer* createCustomer(char *name, int numservice, int lineNum, int time);
void init(queue* qPtr);
void enQueue(queue *qPtr, customer *cusInfo); // a. enQueue 
cnode* deQueue(queue *qPtr);
customer* peek(queue *qPtr); // c. Peek: Return the front of the queue WITHOUT dequeuing. 
int isEmpty(queue *qPtr); // d. Empty (returns 1 if the queue is empty, 0 if it is not)
int getNextLine(queue *que, int ctr);
void freeMem(cnode* nextCust);



// runs all the program and functions 
int main(void) {
  atexit(report_mem_leak); // checks memory leak
  queue que[15];
  char name[16];
  //int ctr = 0; // initialize starting time
  int testcases = 0;
  customer* cust;
  int numcus = 0;
  int numSer, ctime, lineNum;
  
  scanf("%d", &testcases);
  int i = 0;
  // runs the appropiate test cases 
  while(i < testcases){
    int ctr = 0; // initalize time 

    for(int j = 0; j<15;j++){
      init(&que[j]);
    }
    // scans and takes in custmers and sends em to the line (enque)
    scanf("%d", &numcus);    
    for(int j = 0; j < numcus ; j++){
      //scans customer and creates them in the system
      scanf("%s %d %d %d", name, &numSer,&ctime, &lineNum );
      cust = createCustomer(name, numSer, lineNum, ctime);
      enQueue(&que[cust->lineNum-1], cust);
      
    }
  // takes customer off of the line and takes them off of the line (customer leavea)
    for(int j = 0; j < numcus; j++){
      int lineCust = getNextLine(que, ctr);
      cnode* nextCust = deQueue(&que[lineCust-1]);
    

      if(ctr < nextCust->cdata->ctime){
        ctr = nextCust->cdata->ctime;
      }

      int sec = 50 + (80 * nextCust->cdata->serNum) + ctr;
      ctr = sec;

      //A checks out at 8150 from line 1
      // displays customer info
      printf("%s checks out at %d from line %d.\n", nextCust->cdata->name, sec, lineCust);

      freeMem(nextCust);
      
    }
    i++; //incriemnts i to approate test cases 
    
  }
  
}


//(name, number of services, line  number,  time  entering  line).
customer* createCustomer(char *name, int numservice, int lineNum, int time){
  //This function returns a dynamically allocated monster filled with the provided parameters 
  customer *cus = (customer*)malloc(sizeof(customer));

  cus->name = (char*)malloc((strlen(name)+1)*sizeof(char));
  strcpy(cus->name,name);

  //(name, number of services, line  number,  time  entering  line).
  // adds customer info to customer 
  cus->serNum = numservice;
  cus->lineNum = lineNum;
  cus->ctime = time;
    
  return cus;
  
  }

// initializes the que
void init(queue *qPtr){
  
  qPtr->front = NULL;
  qPtr->back = NULL;
  
}

// adds the customer to the que 
 // a. enQueue \/maybe change for void
void enQueue(queue *qPtr, customer *cusInfo){

  // DMA memory to store customer info 
  //creates the linklist 
  cnode* temp = malloc(sizeof(cnode));
  temp->cdata = cusInfo;
  temp->next = NULL;
// checks if it is empty 
  if(isEmpty(qPtr)){

    qPtr->front = temp;
    qPtr->back = temp;
    
    }
  else{
    qPtr->back->next=temp;
    qPtr->back=temp;
  }
}

 


// b. deQueue, takes away the customer from the que, 
// once custiner leaves they are not in the que, so they are dequed 
cnode* deQueue(queue *qPtr){

  // checks que is empty 
  if(isEmpty(qPtr)){ //if(qPtr->front == NULL){
    return NULL;
  }

  cnode *saveCus = qPtr->front;
  //cnode *temp = qPtr->front;

  qPtr->front = qPtr->front->next;

  //free(temp);
  
  if(qPtr->front == NULL){
    qPtr->back = NULL;
  }
  
  return saveCus;
} 


 // d. Empty (returns 1 if the queue is empty, 0 if it is not)
// checks if que is empty, if it is empty returns a 1, if not it returns a 0
int isEmpty(queue *qPtr){
  if(!(qPtr->front == NULL && qPtr->back == NULL)){
    return 0;
  }
  else
    return 1;
}

 // c. Peek: Return the front of the queue WITHOUT dequeuing. 
// looks at who is next in the line (que) without removing them. 
customer* peek(queue *qPtr){
  if(qPtr->front != NULL){
    return qPtr->front->cdata;
  }
  else
    return NULL;

    
 }


// goes trough all the lines 
int getNextLine(queue *que, int ctr){

  //declares the variables 
  int totalNumSer = 101, placeLine = -1, totalTime = 1000000001;

  // goes trough the loop and puts customer into the appropiate ques 
  for(int i = 0; i < 15; i++){

    if(isEmpty(&que[i])){ // checks if que is empty 
      continue;
    }

    customer *nextCus = peek(&que[i]); // checks for the next custoimer 
    
    if(nextCus->ctime > ctr){
      continue;
    }

    // checks the number of services a customer has and adds stuff 
    if(nextCus->serNum < totalNumSer){
      placeLine = nextCus->lineNum;
      totalNumSer = nextCus->serNum;
      
    }
  }
  // checks if placeNum had changed 
  if(placeLine != -1){
    return placeLine;
  }

  // checks if line is empty;
  for(int i = 0; i < 15; i++){
    if(isEmpty(&que[i])){
      continue;
    }
  // checks on the next customer 
    customer* nextCus = peek(&que[i]);
    //checks customer tim with total time
    if(nextCus->ctime < totalTime){
      placeLine = nextCus->lineNum;
      totalTime= nextCus->ctime;
    }
    
  }
  return placeLine;
  
}

// free the memory 
void freeMem(cnode* nextCust){
    free(nextCust->cdata->name);
    free(nextCust->cdata);
    free(nextCust);
}