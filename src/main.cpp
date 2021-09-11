#include <Arduino.h>
extern "C"
{
#include "esp_core_dump.h"
}
#define LED_BUILTIN 2

void readCoreDump();
esp_err_t esp_core_dump_image_erase();

void setup()
{
  Serial.begin(115200);
  Serial.println("SERIAL BEGING");
  readCoreDump();
  // put your setup code here, to run once:
}

void loop()
{
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.println("Hello!");
    delay(1000);
  }
  Serial.println("RESTART ::");
  int a = 0;
  int b = 4;
  Serial.printf("%d\n", b / a);
}

void readCoreDump()
{
  size_t size = 0;
  size_t address = 0;
  if (esp_core_dump_image_get(&address, &size) == ESP_OK)
  {
    const esp_partition_t *pt = NULL;
    pt = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_COREDUMP, "coredump");

    if (pt != NULL)
    {
      uint8_t bf[256];
      char str_dst[640];
      int16_t toRead;

      for (int16_t i = 0; i < (size / 256) + 1; i++)
      {
        strcpy(str_dst, "");
        toRead = (size - i * 256) > 256 ? 256 : (size - i * 256);

        esp_err_t er = esp_partition_read(pt, i * 256, bf, toRead);
        if (er != ESP_OK)
        {
          Serial.printf("FAIL [%x]\n",er);
          //ESP_LOGE("ESP32", "FAIL [%x]", er);
          break;
        }

        for (int16_t j = 0; j < 256; j++)
        {
          char str_tmp[3];

          sprintf(str_tmp, "%02x", bf[j]);
          strcat(str_dst, str_tmp);
        }

        printf("%s", str_dst);
      }
    }
    else
    {
      Serial.println("Partition NULL");
      //ESP_LOGE("ESP32", "Partition NULL");
    }
    esp_core_dump_image_erase();
  }
  else
  {
    Serial.println("esp_core_dump_image_get() FAIL");
    //ESP_LOGI("ESP32", "esp_core_dump_image_get() FAIL");
  }
}

esp_err_t esp_core_dump_image_erase()
{
    /* Find the partition that could potentially contain a (previous) core dump. */
    const esp_partition_t *core_part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA,
                                                                ESP_PARTITION_SUBTYPE_DATA_COREDUMP,
                                                                "coredump");
    if (!core_part) {
        Serial.println("No core dump partition found!");
        return ESP_ERR_NOT_FOUND;
    }
    if (core_part->size < sizeof(uint32_t)) {
        Serial.println("Too small core dump partition!");
        return ESP_ERR_INVALID_SIZE;
    }

    esp_err_t err = ESP_OK;
    err = esp_partition_erase_range(core_part, 0, core_part->size);
    if (err != ESP_OK) {
        Serial.printf("Failed to erase core dump partition (%d)!\n", err);
        return err;
    }

    // on encrypted flash esp_partition_erase_range will leave encrypted
    // garbage instead of 0xFFFFFFFF so overwriting again to safely signalize
    // deleted coredumps
    const uint32_t invalid_size = 0xFFFFFFFF;
    err = esp_partition_write(core_part, 0, &invalid_size, sizeof(invalid_size));
    if (err != ESP_OK) {
        Serial.printf("Failed to write core dump partition size (%d)!\n", err);
    }

    return err;
}