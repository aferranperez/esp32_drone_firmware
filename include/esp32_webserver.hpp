#include <Update.h>
#include <AsyncEventSource.h>

bool cors = true;

// Create an instance of the server
AsyncWebServer server(80);

// Allocate the JSON document
JsonDocument jsonResponse;

// HTML con un botón almacenado en PROGMEM
const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Llamada a API</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #121212;
            color: #e0e0e0;
        }
        .container {
            text-align: center;
            padding: 30px;
            background-color: #1e1e1e;
            border-radius: 12px;
            box-shadow: 0 4px 20px rgba(0,0,0,0.3);
            max-width: 600px;
            width: 90%;
        }
        h1 {
            font-weight: 300;
            margin-bottom: 30px;
            color: #ffffff;
        }
        button {
            background-color: #5865F2;
            color: white;
            border: none;
            padding: 14px 28px;
            font-size: 16px;
            cursor: pointer;
            border-radius: 6px;
            transition: all 0.2s ease;
            font-weight: 500;
            letter-spacing: 0.5px;
        }
        button:hover {
            background-color: #4752c4;
            transform: translateY(-2px);
            box-shadow: 0 4px 8px rgba(88, 101, 242, 0.3);
        }
        button:active {
            transform: translateY(0);
        }
        #resultado {
            margin-top: 30px;
            padding: 20px;
            border: 1px solid #333;
            border-radius: 8px;
            min-height: 120px;
            text-align: left;
            background-color: #252525;
            overflow: auto;
        }
        pre {
            color: #9ecbff;
            white-space: pre-wrap;
            word-wrap: break-word;
            font-family: 'Consolas', monospace;
            font-size: 14px;
            margin: 0;
        }
        p {
            margin: 0;
            line-height: 1.6;
        }
        .loader {
            border: 3px solid #333;
            border-top: 3px solid #5865F2;
            border-radius: 50%;
            width: 24px;
            height: 24px;
            animation: spin 1s linear infinite;
            margin: 20px auto;
            display: none;
        }
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>API Request</h1>
        <button id="btnLlamarAPI">GET /api</button>
        <div class="loader" id="loader"></div>
        <div id="resultado">
            <p>Los resultados aparecerán aquí...</p>
        </div>
    </div>

    <script>
        document.getElementById('btnLlamarAPI').addEventListener('click', function() {
            // Mostrar loader y mensaje de carga
            document.getElementById('loader').style.display = 'block';
            document.getElementById('resultado').innerHTML = '<p>Cargando datos...</p>';
            
            // Realizar la llamada fetch a la API
            fetch('/api')
                .then(response => {
                    // Verificar si la respuesta es exitosa
                    if (!response.ok) {
                        throw new Error('Error en la respuesta: ' + response.status);
                    }
                    return response.json(); // Parsear la respuesta como JSON
                })
                .then(data => {
                    // Ocultar loader
                    document.getElementById('loader').style.display = 'none';
                    
                    // Mostrar los datos en el elemento resultado
                    const resultadoElement = document.getElementById('resultado');
                    resultadoElement.innerHTML = '<p style="color:#8e9eff;margin-bottom:10px;">Respuesta:</p>';
                    
                    // Formatear y mostrar los datos recibidos
                    const pre = document.createElement('pre');
                    pre.textContent = JSON.stringify(data, null, 2);
                    resultadoElement.appendChild(pre);
                })
                .catch(error => {
                    // Ocultar loader
                    document.getElementById('loader').style.display = 'none';
                    
                    // Manejar errores en la llamada
                    document.getElementById('resultado').innerHTML = 
                        '<p style="color: #ff6b6b;">Error: ' + error.message + '</p>';
                });
        });
    </script>
</body>
</html>
    )rawliteral";

void webServerSetup () {
    //-----------------------------------------------------------------------------
    // API REST Zone
    //-----------------------------------------------------------------------------
    server.on("/api", HTTP_GET, [](AsyncWebServerRequest *request) {
        int currentState = digitalRead(LED);
        digitalWrite(LED, !currentState); // Toggle LED state

        jsonResponse["sensor"] = "gps";
        jsonResponse["time"] = 1351824120;
    
        // Add an array.
        JsonArray data = jsonResponse["data"].to<JsonArray>();
        data.add(48.756080);
        data.add(2.302038);
  
        // Generate the minified JSON and send it to the Serial port.
        String jsonString;
        serializeJson(jsonResponse, jsonString);


        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonString ); //Sends 404 File Not Found
        response->addHeader("Access-Control-Allow-Origin","*");
        response->addHeader("Access-Control-Allow-Headers","*");
        request->send(response);
    });

    server.on("/index", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
        response->addHeader("Access-Control-Allow-Origin","*");
        response->addHeader("Access-Control-Allow-Headers","*");
        request->send(response);
    });


    server.begin();
    log("[ Info ] Web server started on port 80");
  
}