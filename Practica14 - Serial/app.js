var SerialPort = require("serialport");
var Readline = SerialPort.parsers.Readline;
var parser = new Readline("\n");

var puerto = "COM1";
var conection = new SerialPort(puerto, 
    { 
        baudRate: 4800,
        parser: parser
    });

conection.pipe(parser);


conection.on("open", function(){ 
    console.log(`Puerto serial conectado en el puerto ${puerto}`);
});

conection.on("data", function(data){
    var data = data.toString('hex');
    
    var value = parseInt("0x"+data);
    var value = (value*5)/255;
    
    console.log("Voltaje " + value);
});


