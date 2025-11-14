
//Client

var name= prompt("What's your name?");
while(name==""){
    name=prompt("You have to choose a name. \n What's your name?")
}

// Query DOM
var message = document.getElementById('messaggio'),
   sender = document.getElementById('sender'),
   btn = document.getElementById('send'),
   output = document.getElementById('output'),
   feedback = document.getElementById('feedback');

sender.innerHTML=name;  
sender.value=name;

// Invio richiesta di connessione al server
var webSocket = io.connect();

// Listen for events
btn.addEventListener('click', function(){
   if (message.value!="" ){    
 	   webSocket.emit('messaggio', {
        message: message.value,
 	   sender: sender.value,
 	   });
 	   message.value = "";
   }
});

webSocket.on('aggiornamento', function(data){
   feedback.innerHTML = '';
   output.innerHTML += '<p><strong>' + data.sender + ': </strong>' + data.message + '</p>';
});

message.onkeydown = function(){
   webSocket.emit('typing', {
      sender: sender.value
   });
}

webSocket.on('typing', function(data){
   feedback.innerHTML = '<p><em>' + data.sender + ' sta scrivendo un messaggio...</em></p>';
});
