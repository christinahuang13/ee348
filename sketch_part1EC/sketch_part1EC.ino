#include "concurrency.h"
//#include "avr/wiring.c"
//lock_t m;
#define INIT_SIZE (10)
//process_t *current_process =  NULL;
//process_t *head_process =  NULL;
//process_t *queue =  NULL; 
PriorityQueue * prio_queue = NULL;
lock_t m;

void printFromC(const char *s){
  Serial.print(s);
  if (serialEventRun) serialEventRun();
  
}

unsigned long getCurrMillis(){
  return millis();
}

void printJobTerm(process_t * currp){
  long diff = millis() - currp->finish_time;
  if (diff <= 0){
    Serial.print("Difference in Termination Time and WCET for Process with stackpointer ");
    Serial.print(currp->sp);
    Serial.print(": ");
    Serial.print(abs(diff/1000));
    Serial.println("ms");
  } else{
    Serial.print("ERROR: Missed deadline for Process with stackpointer ");
    Serial.print(currp->sp);
    Serial.print(". WCET: ");
    Serial.print(currp->wcet);
    Serial.print(". Extra Time Taken: ");
    Serial.print(diff);
    Serial.println("ms");
  }
}

void printFromC_ln(const char *s){
  Serial.println(s);
  if (serialEventRun) serialEventRun();
}

void blink1 (void)
{
  while (1) {
    noInterrupts();
    lock_acquire (&m);
    interrupts();
    digitalWrite (13, HIGH);
    delay (500);
    digitalWrite (13, LOW);
    delay (500);
    noInterrupts();
    lock_release (&m);
    Serial.println("blink1");
    if (serialEventRun) serialEventRun();
    interrupts();
  }
}

void blink2 (void)
{
   while (1) {
     Serial.println("blink2 start");
    if (serialEventRun) serialEventRun();
      digitalWrite (2, HIGH);
      delay (400);
      digitalWrite (2, LOW);
      delay (400);
    Serial.println("blink2 end");
    if (serialEventRun) serialEventRun();

   }
}


void blink3 (void)
{
   while (1) {
     Serial.println("blink3 start");
    if (serialEventRun) serialEventRun();
      digitalWrite (3, HIGH);
      delay (300);
      digitalWrite (3, LOW);
      delay (300);
    Serial.println("blink3 end");
    if (serialEventRun) serialEventRun();
   }
}

void blink4 (void)
{
  while (1) {
    noInterrupts();
      lock_acquire (&m);
      interrupts();
      digitalWrite (13, HIGH);
      delay (100);
      digitalWrite (13, LOW);
      delay (100);
      digitalWrite (13, HIGH);
      delay (100);
      digitalWrite (13, LOW);
      delay (100);
      noInterrupts();
      lock_release (&m);
      interrupts();
      Serial.println("blink4");
    if (serialEventRun) serialEventRun();
  }
}

void blink5 (void){
  //terminating test
  int count = 10;
  while (count > 0){
      lock_acquire (&m);
        Serial.println("Blink 5 lock acquired");
  if (serialEventRun) serialEventRun();
      digitalWrite (2, HIGH);
      delay (300);
      digitalWrite (2, LOW);
      delay (300);
      digitalWrite (2, HIGH);
      delay (300);
      digitalWrite (2, LOW);
      delay (300);
      lock_release (&m);
        Serial.println("Blink 5 lock released");
  if (serialEventRun) serialEventRun();
    count --;
  }
    Serial.println("Blink 5 terminated");
  if (serialEventRun) serialEventRun();
}

void blink6 (void){
  //terminating test
  int count = 10;
  while (count > 0){
      lock_acquire (&m);
        Serial.println("Blink 6 lock acquired");
  if (serialEventRun) serialEventRun();
      digitalWrite (3, HIGH);
      delay (400);
      digitalWrite (3, LOW);
      delay (400);
      digitalWrite (3, HIGH);
      delay (400);
      digitalWrite (3, LOW);
      delay (400);
      lock_release (&m);
        Serial.println("Blink 6 lock released");
  if (serialEventRun) serialEventRun();
    count --;
  }
  Serial.println("Blink 6 terminated");
  if (serialEventRun) serialEventRun();
}

void priority_test(){
  // Blink 6 has highest priority and should run exclusively until it terminates
  // Then Blink 5 will run exclusively until it terminates
  // Then Blink 1 and Blink 2 will run concurrently for an infintely long time
  process_create_prio (blink1, 64, 100);
  process_create_prio (blink2, 64, 100);
  process_create_prio (blink5, 64, 122);
  process_create_prio (blink6, 64, 200); 
}

void timed_test(){
  if (process_create_rtjob (blink5, 128, 1, 2)== -1){
    Serial.println("Failed blink5");
  }
    delay(300);
  if (process_create_rtjob (blink6, 128, 30000, 40000) == -1){
    Serial.println("Failed blink6");
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode (13, OUTPUT);
  pinMode (11, OUTPUT);
  pinMode (2, OUTPUT);
  pinMode (3, OUTPUT);
  digitalWrite (11, HIGH);
  Serial.begin (9600);
  lock_init (&m);
  prio_queue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  PriorityQueueInit(prio_queue);

//  priority_test();
  timed_test();
}

void loop() {
  Serial.println ("start...");
  process_start ();
  Serial.println ("done!");
}
