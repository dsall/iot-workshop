var express = require('express')
var app = express()
const hexRgb = require('hex-rgb');

var MagicHomeControl = require('magic-home').Control;
var light = new MagicHomeControl("192.168.0.17");

var MagicHomeDiscovery = require('magic-home').Discovery;

// Discovery should allow us to find the LED strip's IP address and pass it to the  controller instance
// I need t o work on that to get it working 

var discovery = new MagicHomeDiscovery();

var bodyParser = require('body-parser')

app.use(bodyParser.json());

// Turn On LED Strip
LEDOn = () => {
    light.turnOn(function (err, success)  {
    });
}

// Turn Off LED Strip 
LEDOff = () => {
    light.turnOff(function (err, success)  {
    });
}

//Function to set Color on LED Strip
LEDSetRGB = (r,g,b) => {
    light.setColor(r, g, b, function(err, success){

    })
}

app.get('/', function (req, res) {
  res.send('Working on port 3000')
})

// By going to localhost:300/LED/1 LED is turned on 
// By going to localhost:300/LED/0 LED is turned off
app.get('/led/:id', function (req, res) {
   if(req.params.id == 1){
       LEDOn();
   }

   if(req.params.id == 0){
       LEDOff();
   }
   res.send('success');
})

//When you go to localhost:3000/hex/"hexvalue of color" the LED sets the color to the hex value

app.get('/hex/:id', function (req, res) {
    var rgb = hexRgb(req.params.id);
    console.log(rgb);
    LEDSetRGB(rgb.red, rgb.green, rgb.blue);
    res.send('ok');
    
 })

 //You can submit the RGB values withing a JSON object as well 
 app.post('/rgb', function (req, res) {
    LEDSetRGB(req.body.r, req.body.g, req.body.b);
    res.send('ok');
 })


 
app.listen(3000);
console.log('Listening on port 3000');