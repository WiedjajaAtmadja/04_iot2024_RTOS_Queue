#include <Arduino.h>
#define LED_RED   21
#define LED_GREEN 22
#define LED_BLUE  23
QueueHandle_t xQueue;

void taskProduce(void *pvParameters) {
  int i = 1;
  while (1) {
    xQueueSend(xQueue, &i, portMAX_DELAY);
    Serial.printf("==> Produce: %d \r\n", i); fflush(stdout);
    i++;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void taskConsume(void *pvParameters) {
  int i;
  while (1) {
    xQueueReceive(xQueue, &i, portMAX_DELAY);
    Serial.printf("<== Consume: %d \r\n", i); fflush(stdout);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);

  xQueue = xQueueCreate(3, sizeof(int));
  xTaskCreatePinnedToCore(taskProduce, "taskProduce", configMINIMAL_STACK_SIZE+1024, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskConsume, "taskConsume", configMINIMAL_STACK_SIZE+1024, NULL, 1, NULL, 1);
  Serial.println("System ready");
}

void loop() {
  vTaskDelay(0);
}