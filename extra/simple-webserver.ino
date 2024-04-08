#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server(80);

const char* htmlContent = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Webversion von Simon Says</title>
    <style>
        body {
            background: linear-gradient(90deg, #ff8a00, #e52e71, #8914b3, #0077ff, #00ff00);
            background-size: 200% 200%;
            animation: gradient 10s ease infinite;
            color: #ffffff; /* Weiß */
            font-family: Arial, sans-serif;
            padding: 20px;
            text-align: center;
        }
        @keyframes gradient {
            0% {
                background-position: 0% 50%;
            }
            50% {
                background-position: 100% 50%;
            }
            100% {
                background-position: 0% 50%;
            }
        }
        h1 {
            color: #ffeb3b; /* Gelb */
        }
        p {
            font-size: 18px;
        }
        .button {
            background-color: #00bcd4; /* Türkis */
            color: #ffffff; /* Weiß */
            border: none;
            padding: 10px 20px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 5px;
            transition: background-color 0.3s ease;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1), 0 1px 3px rgba(0, 0, 0, 0.08);
        }
        .button:hover {
            background-color: #0097a7; /* Dunkles Türkis */
            transform: translateY(-2px);
            box-shadow: 0 6px 8px rgba(0, 0, 0, 0.15), 0 2px 4px rgba(0, 0, 0, 0.1);
        }
    </style>
</head>
<body>
    <h1>Wähle eine Farbe zum Starten</h1>
    <p>Klicke auf eine Farbe, um das Spiel zu starten.</p>
    <div id="startButtons">
        <button class="color-button" onclick="startGame('red')">Rot</button>
        <button class="color-button" onclick="startGame('green')">Grün</button>
        <button class="color-button" onclick="startGame('yellow')">Gelb</button>
        <button class="color-button" onclick="startGame('blue')">Blau</button>
    </div>



<script>
// Funktion zum Starten des Spiels mit einer ausgewählten Farbe
function startGame(color) {
    // Sende einen Befehl an den Server, um das Spiel zu starten
    fetch('/start_simon_says')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
        })
        .catch(error => {
            console.error('There was a problem with the fetch operation:', error);
        });
}

// Variablen zur Speicherung des aktuellen Musters und der Benutzerantwort
let sequence = [];
let userSequence = [];
let level = 0;
let index = 0;

// Funktion zum Starten des Simon Says-Spiels
function startSimonSays() {
    sequence = [];
    userSequence = [];
    level = 0;
    index = 0;
    nextSequence();
}

// Funktion zum Generieren der nächsten Sequenz
function nextSequence() {
    level++;
    const nextColor = Math.floor(Math.random() * 4); // Zufällige Farbe generieren (0 bis 3)
    sequence.push(nextColor); // Farbe zur Sequenz hinzufügen
    console.log("Simon's sequence:", sequence);
    displaySequence(); // Sequenz anzeigen
}

// Funktion zum Anzeigen der Sequenz
function displaySequence() {
    // Simulieren Sie hier die Anzeige der Farbsequenz (z. B. durch Blinken der Farben)
    // Nach einer kurzen Verzögerung kann die Benutzerinteraktion zugelassen werden
    setTimeout(() => {
        playSequence(0);
    }, 1000);
}

// Funktion zum Abspielen der Farbsequenz
function playSequence(i) {
    if (i < sequence.length) {
        const color = sequence[i];
        highlightButton(color);
        setTimeout(() => {
            unhighlightButton(color);
            playSequence(i + 1);
        }, 1000);
    } else {
        allowUserInput();
    }
}

// Funktion zum Hervorheben des entsprechenden Buttons
function highlightButton(color) {
    // Implementieren Sie hier die Logik, um den entsprechenden Button hervorzuheben
    console.log("Highlighting button:", color);
}

// Funktion zum Deaktivieren der Hervorhebung des entsprechenden Buttons
function unhighlightButton(color) {
    // Implementieren Sie hier die Logik, um die Hervorhebung des Buttons zu deaktivieren
    console.log("Unhighlighting button:", color);
}

// Funktion zum Erlauben der Benutzerinteraktion
function allowUserInput() {
    // Implementieren Sie hier die Logik, um die Benutzerinteraktion zu ermöglichen
    console.log("Waiting for user input...");
}

// Funktion zur Überprüfung der Benutzerantwort
function checkUserInput(color) {
    if (color === sequence[index]) {
        index++;
        if (index === sequence.length) {
            // Benutzer hat die Sequenz richtig wiederholt
            console.log("Correct sequence!");
            nextSequence();
        }
    } else {
        // Benutzer hat die Sequenz falsch wiederholt
        console.log("Incorrect sequence!");
        // Hier können Sie die Logik für den Umgang mit einer falschen Antwort implementieren
    }
}

</script>
</body>
</html>
)=====";

enum Color { RED, GREEN, BLUE, YELLOW };

Color sequence[10]; // Maximum sequence length
int sequenceLength = 4; // Initial sequence length
int sequenceIndex = 0;
bool playerTurn = false;

void generateSequence() {
    for (int i = 0; i < sequenceLength; ++i) {
        sequence[i] = static_cast<Color>(random(4)); // Generate random color
    }
}

void handleRoot() {
    server.send(200, "text/html", htmlContent);
}

void handleStartSimonSays() {
    generateSequence();
    sequenceIndex = 0;
    playerTurn = false;
    server.send(200, "text/plain", "Simon Says gestartet!");
}

void handleColorButton() {
    if (!playerTurn) {
        return; // Ignore button clicks during Simon's turn
    }
    
    String color = server.arg("color");
    Color selectedColor;

    if (color == "red") {
        selectedColor = RED;
    } else if (color == "green") {
        selectedColor = GREEN;
    } else if (color == "blue") {
        selectedColor = BLUE;
    } else if (color == "yellow") {
        selectedColor = YELLOW;
    } else {
        return; // Invalid color
    }

    if (selectedColor == sequence[sequenceIndex]) {
        sequenceIndex++;
        if (sequenceIndex >= sequenceLength) {
            sequenceLength++; // Increase sequence length for next round
            generateSequence();
            sequenceIndex = 0;
            playerTurn = false;
        }
    } else {
        server.send(400, "text/plain", "Falsche Farbe! Versuche es erneut.");
        sequenceLength = 4; // Reset sequence length for next game
        generateSequence();
        sequenceIndex = 0;
        playerTurn = false;
    }
    
    server.send(200, "text/plain", "OK");
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Verbindung zum WLAN...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Verbindung zum WLAN fehlgeschlagen! Erneuter Versuch...");
    }

    Serial.println("Verbunden mit dem WLAN");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, handleRoot);
    server.on("/start_simon_says", HTTP_GET, handleStartSimonSays);
    server.on("/color", HTTP_GET, handleColorButton);

    server.begin();
    Serial.println("Webserver gestartet");
}

void loop() {
    server.handleClient();
}
