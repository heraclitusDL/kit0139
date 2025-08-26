#define ANALOG_PIN A1
#define RANGE 5000 // Depth measuring range 5000mm (for water)
#define CURRENT_INIT 0.004 // Current @ 0mm (unit: mA)
#define DENSITY_WATER 1  // Pure water density normalized to 1
#define DENSITY_GASOLINE 0.74  // Gasoline density
#define PRINT_INTERVAL 1000

// matt pallan, 08/21/25, init code with CSV output

int16_t dataVoltage;
float dataCurrent, depth; // unit: mA
unsigned long timepoint_measure;
bool headerPrinted = false;

void setup()
{
  Serial.begin(9600);
  pinMode(ANALOG_PIN, INPUT);
  timepoint_measure = millis();

  // Print CSV header row once
  Serial.println("Voltage,Current,Depth(mm)");
}

void loop()
{
  if (millis() - timepoint_measure > PRINT_INTERVAL) {
    timepoint_measure = millis();

    dataVoltage = analogRead(ANALOG_PIN);
    dataCurrent = ((dataVoltage / 1024.0) * 4.95) / 120.0; // Sense Resistor:120 ohm
    depth = (dataCurrent - CURRENT_INIT) * (RANGE / DENSITY_WATER / 16.0); // Calculate depth

    if (depth < 0) depth = 0.0;

    // Print in CSV format: Voltage,Current,Depth
    Serial.print(dataVoltage);
    Serial.print(",");
    Serial.print(dataCurrent, 6); // 6 decimal places for precision
    Serial.print(",");
    Serial.println(depth, 2); // 2 decimal places for depth
  }
}
