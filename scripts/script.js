//This is a modification of original codes of Conneted Devices and Networked Interaction course

// variables for the DOM elements:
let portSelector;
let powerButton;
let brightness;
// variable for the serialport object:
let serial;
// previous state of the serial input from the button:
let lastButtonState = 0;

// this function is called when the page is loaded. 
// element event listeners are  added here:
function setup(event) {
  // add listeners for the power button and the brightness:
  powerButton = document.getElementById('power');
  powerButton.addEventListener('click', setPowerState);

  brightnessSlider = document.getElementById('brightness');
  brightnessSlider.addEventListener('change', setBrightness);

  // initialize the serialport object:
  serial = new p5.SerialPort(); // new instance of the serialport library
  serial.on('list', printList); // callback function for serialport list event
  serial.on('data', serialEvent); // callback function for serialport data event
  serial.list(); // list the serial ports

  // set initial states of the inputs:
  setPowerState();
  setBrightness();
  setColor();
  setNumber();
}



function setPowerState() {
  // change its value, depending on its current value:
  if (powerButton.value == 'on') {
    powerButton.value = 'off';
  } else {
    powerButton.value = 'on';
  }
  // get the span associated with it and change its text:
  let thisSpan = document.getElementById(powerButton.id + 'Val');
  thisSpan.innerHTML = "Power is " + powerButton.value;
}

function setBrightness(e) {
 // assume e is a number:
var currentValue = e;
// but if it's an object instead, it's because
// the slider change event called this function. 
// Extract the number from it:
  if (typeof e == 'object') {
    currentValue = e.target.value;
  } 
  //get the span associated with it and change its text:
  let thisSpan = document.getElementById(brightnessSlider.id + 'Val');
  thisSpan.innerHTML = currentValue;
}

function setColor(c){
    var colorValue = c;
    if (typeof a == 'object'){
        colorValue = a.target.value;
    }
    let thisSpan = document.getElementById(colorBox.id + 'Val');
    thisSpan.innterHTML = colorValue;
}

function setNumber(d){
    var numberValue = d;
    if (typeof a == 'object'){
        numberValue = a.target.value;
    }
    let thisSpan = document.getElementById(knobNumber.id + 'Val');
    thisSpan.innterHTML = numberValue;
}

// make a serial port selector object:
function printList(portList) {
  // create a select object:
  portSelector = document.getElementById('portSelector');
  // portList is an array of serial port names
  for (var i = 0; i < portList.length; i++) {
    // add this port name to the select object:
    var option = document.createElement("option");
    option.text = portList[i];
    portSelector.add(option);
  }
  // set an event listener for when the port is changed:
  portSelector.addEventListener('change', openPort);
}

function openPort() {
  let item = portSelector.value;
  // if there's a port open, close it:
  if (serial.serialport != null) {
    serial.close();
  }
  // open the new port:
  serial.open(item);
}


function serialEvent() {
  // read a line of incoming data:
  var inData = serial.readLine();
  // if the line is not empty, parse it to JSON:
  if (inData) {
    var sensors = JSON.parse(inData);
    // button value:
    // if the button's changed and it's pressed, take action:
    if (sensors.button !== lastButtonState) {
      if (sensors.button === 0) {
        setPowerState(sensors.button);
      }
      // save button value for next time:
      lastButtonState = sensors.button;
    }
    //brightness slider value:
    setBrightness(sensors.knob);
    setColor(sensors.knob);
  }
}

// add a listener for the page to load:
window.addEventListener('DOMContentLoaded', setup);