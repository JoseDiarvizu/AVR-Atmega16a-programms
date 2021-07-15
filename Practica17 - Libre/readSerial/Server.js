var SerialPort = require("serialport");
var Readline = SerialPort.parsers.Readline;
var parser = new Readline("\n");

var puerto = "COM3";
var conection = new SerialPort(puerto, 
    { 
        baudRate: 4800,
        parser: parser
    });

conection.pipe(parser);

var respuesta="";

conection.on("open", function(){ 
    console.log(`\nPuerto serial conectado en el puerto ${puerto}`);
});


    const readline = require('readline');
    var log = console.log;
    
    var rl = readline.createInterface({
      input: process.stdin,
      output: process.stdout
    });
    
  
    conection.on("data", function(data){
        var data = data.toString('hex');
        
        var value = parseInt("0x"+data);
        if(value == 1)
          console.log("Simon dice...\n De pie");
        if(value == 2)
          console.log("Simon dice...\n Sentado");
        if(value == 3)
          console.log("Simon dice...\n Agachado");

        rl.question('1. De pie\n2. Sentado\n3. Agachado\nSigue a simon: (1, 2 o 3): ', input => {
          if( input == 'exit' )
            return rl.close();
          conection.write(input, function(err){
              if(err) {
                  console.log("\n[ERROR] No se ha podido enviar el mensaje.");
              } else {
                  console.log("\n[SUCCESS] Mensaje enviado correctamente.");
              }
          }); 
        });

    });