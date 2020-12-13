var ws;
var ESP_ip;
let connected = false;

function wsInit() {
    ws = new WebSocket("ws://" + ESP_ip + ":81");

    ws.onopen = function (event) {
        connected = true;
        resetOutput();
        online();
    }
    
    ws.onmessage = function (event) {
        if (typeof(event.data) == "string") {
            receiveTxt(event.data);
        } else { //anything else is considered as a pong
            receivePong();
        }
    }
    
    ws.onclose = function (event) {
        connected = false;
        offline();
        setTimeout(wsInit, 1000);
    }
}

let waitingForPong = 0;

function receivePong() {
    waitingForPong = 0;
    online();
}

function handlePingPong() {
    if (connected) {
        if (waitingForPong++ > 2) {
            offline();
        }
        ws.send("");
    }
    setTimeout(handlePingPong, 1000);
}

let isOnline = false;

function online() {
    if (!isOnline) {
        inputButtonGreen();
        isOnline = true;
    }
} 

function offline() {
    if (isOnline) {
        inputButtonRed();
        isOnline = false;
    }
} 

function inputButtonGreen() {
    document.getElementById("inputImgRed").style.display = "none";
    document.getElementById("inputImgGreen").style.display = "inherit";
}

function inputButtonRed() {
    document.getElementById("inputImgRed").style.display = "inherit";
    document.getElementById("inputImgGreen").style.display = "none";
}

function inputButtonSetup() {
    document.getElementById("txtIn").addEventListener("keydown", function(event) {
      if (event.keyCode === 13) {
        event.preventDefault();
        document.getElementById("sendButton").click();
      }
    });
}

function getESPlocation() {
    var loc = location.hostname;
    if (loc == "") loc = prompt("Enter ESP ip address","192.168.1.80");
    return loc;
}

function resetOutput() {
    document.getElementById("txtOut").innerHTML = "";
}

function receiveTxt(txt) {
    var textarea = document.getElementById("txtOut");
    var scrolledBottom = (textarea.scrollTop + textarea.clientHeight + 20 >= textarea.scrollHeight);
    textarea.innerHTML += txt;
    if (scrolledBottom) textarea.scrollTop = textarea.scrollHeight;
}

function send() {
    if (isOnline) {
        var txt = document.getElementById("txtIn").value;
        if (txt.length > 0) {
            ws.send(txt);
            document.getElementById("txtIn").value = "";
        }
    }
}

function resize() {
    var terminalHeight=document.getElementById("terminal").clientHeight;
    var headerHeight=document.getElementById("header").clientHeight;
    var inputLineHeight=document.getElementById("inputLine").clientHeight;
    var inputButtonHeight=document.getElementById("inputButton").clientHeight;
    var inputHeight = inputLineHeight > inputButtonHeight ? inputLineHeight : inputButtonHeight;
    document.getElementById("input").style.height = inputHeight + "px";
    var outputHight = terminalHeight - headerHeight - inputHeight - 20;
    document.getElementById("output").style.height = outputHight + "px";
    var textarea = document.getElementById("txtOut");
    textarea.scrollTop = textarea.scrollHeight;
}
window.onresize = resize;

function setup() {
    ESP_ip = getESPlocation();
    inputButtonSetup();
    wsInit();
    resize();
    handlePingPong();
}

window.addEventListener("DOMContentLoaded", (event) => {setup();});
