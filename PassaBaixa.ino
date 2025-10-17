//Fs = 10Hz, Fc = 2Hz, Ordem N=18 (19 Coeficientes/Taps)
const int N_ORDER = 18;
const int NUM_COEFFICIENTS = N_ORDER + 1;
const long INTERVALO_MS = 100; // 1/10Hz = 0.1s = 100ms

// Coeficientes h[n] (Banda Passante de 0Hz a 2Hz)
float h[NUM_COEFFICIENTS] = {
    -0.00290, -0.00317,  0.00611,  0.01792,  0.00000, 
    -0.04941, -0.04937,  0.08448,  0.29519,  0.40000,
     0.29519,  0.08448, -0.04937, -0.04941,  0.00000, 
     0.01792,  0.00611, -0.00317, -0.00290 
};

// Array para armazenar as 19 amostras de entrada anteriores (x[n] a x[n-18])
float x_delay[NUM_COEFFICIENTS] = {0.0};

// Variáveis de tempo
unsigned long previousMillis = 0;

//Equação de diferenças
float apply_fir_filter(float new_sample) {
    // 1. Desloca o buffer: x[n-1] -> x[n-2], etc.
    for (int i = NUM_COEFFICIENTS - 1; i > 0; i--) {
        x_delay[i] = x_delay[i - 1];
    }
    
    // 2. Armazena a nova amostra em x[n] (x_delay[0])
    x_delay[0] = new_sample;

    // 3. Convolução: y[n] = SOMA ( h[k] * x[n-k] )
    float y_n = 0.0;
    for (int k = 0; k < NUM_COEFFICIENTS; k++) {
        y_n += h[k] * x_delay[k];
    }

    return y_n;
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    unsigned long currentMillis = millis();

    // Garante que a amostragem ocorra a 10Hz (a cada 100ms)
    if (currentMillis - previousMillis >= INTERVALO_MS) {
        previousMillis = currentMillis;

        // Leitura da entrada (A0) e normalização para float (0.0 a 1.0)
        int raw_adc = analogRead(A0); 
        float x_n = (float)raw_adc / 1023.0; 
        
        // Aplicação do filtro
        float y_n_filtrado = apply_fir_filter(x_n);
        
        // Saída serial para monitoramento
        Serial.print("Entrada (x[n]): ");
        Serial.print(x_n);
        Serial.print(" | Saída Filtrada (y[n]): ");
        Serial.println(y_n_filtrado);
    }
}
