import { initializeApp } from 'https://www.gstatic.com/firebasejs/10.1.0/firebase-app.js';
// import { getFunctions, httpsCallable, connectFunctionsEmulator } from 'https://www.gstatic.com/firebasejs/10.1.0/firebase-functions.js';
import { getDatabase, onValue, update, set, ref, connectDatabaseEmulator } from 'https://www.gstatic.com/firebasejs/10.1.0/firebase-database.js';

const firebaseConfig = {
    authDomain: "arduino-iot-demo-785d8.firebaseapp.com",
    databaseURL: "https://arduino-iot-demo-785d8-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "arduino-iot-demo-785d8",
    storageBucket: "arduino-iot-demo-785d8.appspot.com",
    messagingSenderId: "1045703384375",
    appId: "1:1045703384375:web:f5390ead7a685664bd0eb2"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);
if (location.hostname === "localhost") {
    // Point to the RTDB emulator running on localhost.
    connectDatabaseEmulator(db, "127.0.0.1", 9000);
    // Reset database.
    set(ref(db), {
        "pins": [
            null,
            null,
            true,
            false,
            false,
            false,
            true,
            true,
            true,
            true,
            false,
            true,
            true
        ],
        "sensors": {
            "altitude": -3.662937,
            "gas_resistance": 50397,
            "humidity": 43.838787,
            "lpg_content": 434,
            "potentiometer": 357,
            "pressure": 101370,
            "temperature": 25.191204
        }
    });
}

onValue(ref(db, "sensors"), (snapshot) => {
    let sensors = snapshot.val();
    sensors.temperature = sensors.temperature?.toFixed(2);
    sensors.humidity = sensors.humidity?.toFixed(2);
    sensors.pressure = (sensors.pressure / 100).toFixed(2);
    sensors.gas_resistance = (sensors.gas_resistance / 1000).toFixed(2);

    const idSensorMap = {
        "temp-value": "temperature",
        "humidity-value": "humidity",
        "pressure-value": "pressure",
        "gasresistance-value": "gas_resistance",
        "lpgcontent-value": "lpg_content"
    }
    for (const id in idSensorMap) {
        if (Object.hasOwnProperty.call(idSensorMap, id)) {
            const element = idSensorMap[id];
            document.getElementById(id).innerHTML = sensors[element];
        }
    }
});


let ledState = {
    led1: false,
    led2: false,
    led3: {
        state: false,
        color: "red"
    },
}

const btnLed1 = document.getElementById("btn-led1");
const btnLed2 = document.getElementById("btn-led2");
const btnLed3 = document.getElementById("btn-led3-toggle");
const btnLed3Red = document.getElementById("btn-led3-red");
const btnLed3Green = document.getElementById("btn-led3-green");
const btnLed3Blue = document.getElementById("btn-led3-blue");

const pinMap = {
    led1: 6,
    led2: 5,
    led3Gnd: 10,
    led3Red: 11,
    led3Green: 9,
    led3Blue: 12,
}

function activateButton(btn) {
    if (!btn.classList.contains("active")) {
        btn.classList.add("active");
    }
}

function deactivateButton(btn) {
    if (btn.classList.contains("active")) {
        btn.classList.remove("active");
    }
}

function setButtons(state) {
    if (state.led1) {
        activateButton(btnLed1);
    } else {
        deactivateButton(btnLed1);
    }

    if (state.led2) {
        activateButton(btnLed2);
    } else {
        deactivateButton(btnLed2);
    }

    if (state.led3.state) {
        activateButton(btnLed3);
    } else {
        deactivateButton(btnLed3);
    }

    if (state.led3.color === "red") {
        activateButton(btnLed3Red);
    } else {
        deactivateButton(btnLed3Red);
    }

    if (state.led3.color === "green") {
        activateButton(btnLed3Green);
    } else {
        deactivateButton(btnLed3Green);
    }

    if (state.led3.color === "blue") {
        activateButton(btnLed3Blue);
    } else {
        deactivateButton(btnLed3Blue);
    }
}

onValue(ref(db, "pins"), (snapshot) => {
    let pins = snapshot.val();
    ledState.led1 = pins[pinMap.led1];
    ledState.led2 = pins[pinMap.led2];
    ledState.led3 = {
        state: !pins[pinMap.led3Gnd],
        color: pins[pinMap.led3Red] ? "red" : pins[pinMap.led3Green] ? "green" : "blue"
    };

    setButtons(ledState);
});

btnLed1.addEventListener("click", () => {
    set(ref(db, "pins/" + pinMap.led1), !ledState.led1);
});

btnLed2.addEventListener("click", () => {
    set(ref(db, "pins/" + pinMap.led2), !ledState.led2);
});

btnLed3.addEventListener("click", () => {
    set(ref(db, "pins/" + pinMap.led3Gnd), ledState.led3.state);
});

btnLed3Red.addEventListener("click", () => {
    update(ref(db, "pins"), {
        [pinMap.led3Red]: true,
        [pinMap.led3Green]: false,
        [pinMap.led3Blue]: false,
    });
});

btnLed3Blue.addEventListener("click", () => {
    update(ref(db, "pins"), {
        [pinMap.led3Red]: false,
        [pinMap.led3Green]: false,
        [pinMap.led3Blue]: true,
    });
});

btnLed3Green.addEventListener("click", () => {
    update(ref(db, "pins/"), {
        [pinMap.led3Red]: false,
        [pinMap.led3Green]: true,
        [pinMap.led3Blue]: true,
    });
});