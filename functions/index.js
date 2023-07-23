const { logger } = require("firebase-functions");
const { onRequest, onCall } = require("firebase-functions/v2/https");
const admin = require("firebase-admin");
const { log } = require("firebase-functions/logger");

admin.initializeApp();

var db = admin.database();

exports.setPin = onCall((request) => {
    const { pin, value } = request.data;
    logger.info("Setting", pin, "to", value);

    const ref = db.ref("/pins/" + pin);
    ref.set({ value: value ? 1 : 0 });

    return { result: "done" };
});

exports.getPins = onCall(async (request) => {
    const ref = db.ref("/pins");
    const snapshot = await ref.once("value");
    let pins = {};
    snapshot.forEach((child) => {
        pins[child.key] = child.val();
    });
    logger.log(pins);
    return pins;
});

exports.resetDb = onCall(async (request) => {
    const ref = db.ref("/pins");

    await ref.set(null);

    for (let i = 2; i <= 13; i++) {
        await ref.child(i).set({ value: 0 });
    }

    logger.log("resetDb", "done");

    return { result: "done" };
});
// exports.getPinsFlag = onRequest(async (request, response) => {
//     let flag = 0; // 32-bit integer, each bit represents a pin with pin 0 being the least significant bit
//     const snapshot = await db.ref("/pins").once('value');
//     snapshot.forEach((pin) => {
//         // logger.log(pin.key, pin.val());
//         logger.info("pin.key =", pin.key, "pin.val() =", pin.val());
//         let bit = parseInt(pin.val()) * Math.pow(2, parseInt(pin.key));
//         logger.log("bit =", bit);
//         flag = flag | (pin.val() * Math.pow(2, parseInt(pin.key))); // 2^(pin.key)
//         logger.log("flag =", flag);
//     });

//     response.send(flag.toString());
// });

function setCharAt(str, index, chr) {
    if (index > str.length - 1) return str;
    return str.substring(0, index) + chr + str.substring(index + 1);
}

exports.getPinsFlag = onRequest(async (request, response) => {
    let flag = "0000000000000000"; // 32-bit integer, each bit represents a pin with pin 0 being the least significant bit
    // Im not touching bitwise operations in js with a 10-foot pole

    const snapshot = await db.ref("/pins").once("value");
    snapshot.forEach((pin) => {
        logger.log(pin.key, pin.val());
        if (pin.key < 0 || pin.key > 15) {
            logger.error("pin.key =", pin.key, "is out of range");
            response.status(500).send("pin.key out of range");
            return;
        }

        if (pin.val() > 0) {  // TODO: WHY THE FUCK IS THIS NOT WORKING???
            flag = setCharAt(flag, 16 - pin.key, "1");
        }

        logger.info(flag, parseInt(flag, 2));
    });
    response.status(200).send(parseInt(flag, 2).toString());
});