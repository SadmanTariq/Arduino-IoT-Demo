import { initializeApp } from 'https://www.gstatic.com/firebasejs/10.1.0/firebase-app.js';
import { getFunctions, httpsCallable, connectFunctionsEmulator } from 'https://www.gstatic.com/firebasejs/10.1.0/firebase-functions.js';

const DEBUG = false;

const firebaseConfig = {
    apiKey: "AIzaSyCnx3I0MGLDhLIqj-0P5xXDm6vbQgCtqy8",
    authDomain: "arduino-iot-demo-785d8.firebaseapp.com",
    databaseURL: "https://arduino-iot-demo-785d8-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "arduino-iot-demo-785d8",
    storageBucket: "arduino-iot-demo-785d8.appspot.com",
    messagingSenderId: "1045703384375",
    appId: "1:1045703384375:web:f5390ead7a685664bd0eb2"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const functions = getFunctions(app);

if (DEBUG)
    connectFunctionsEmulator(functions, "127.0.0.1", 5001);

const setPin = httpsCallable(functions, 'setPin');
const resetDb = httpsCallable(functions, 'resetDb');
const getPins = httpsCallable(functions, 'getPins');

async function togglePin(pin) {
    return setPin({ pin: pin, value: 1 })
}

for (let i = 2; i <= 13; i++) {
    document.getElementById('btn-pin-' + i).addEventListener('click', () => {
        togglePin(i)
            .then(updatePins());
    });
}

document.getElementById('btn-reset').addEventListener('click', () => {
    console.log("reset");
    resetDb()
        .then((result) => {
            console.log(result);
        })
        .catch((error) => {
            console.error(error);
        });
    updatePins();
});

async function updatePins() {
    let pins = await getPins();
    pins = pins.data;
    console.log(pins);
    for (let i = 2; i <= 13; i++) {
        document.getElementById('pin-' + i + '-status').innerHTML = pins[i].value ? "ON" : "OFF";
    }
}

setInterval(updatePins, 1000);