#include "concurrency.h"
//#include "avr/wiring.c"
//lock_t m;
#define INIT_SIZE (10)
//process_t *current_process =  NULL;
//process_t *head_process =  NULL;
//process_t *queue =  NULL; 
PriorityQueue * prio_queue = NULL;
lock_t m;

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
      digitalWrite (2, HIGH);
      delay (400);
      digitalWrite (2, LOW);
      delay (400);
    Serial.println("blink2");
    if (serialEventRun) serialEventRun();

   }
}


void blink3 (void)
{
   while (1) {
      digitalWrite (3, HIGH);
      delay (300);
      digitalWrite (3, LOW);
      delay (300);
    Serial.println("blink3");
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
//    noInterrupts();
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
  process_create (blink1, 32);
  process_create (blink2, 32);
//  Serial.println(process_create (blink3, 32));
  process_create (blink4, 32);
}


void loop() {
  Serial.println ("start...");
  process_start ();
  Serial.println ("done!");
}
