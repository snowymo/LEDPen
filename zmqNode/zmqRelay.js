var zmq = require('zmq')

var serverAddress = '192.168.1.19';	
var serverPort = 1611;
//var _ = require('underscore');

var holojam = require('holojam-node')(['emitter'],'192.168.1.19');

// TCP
var sub = zmq.socket('sub')
sub.identity = 'Hello' // set identity to enable durability
sub.connect('tcp://192.168.1.44:5565')
sub.subscribe('');

function readFloat(msg){
	var index = 0;
	return {x:msg.readFloatLE(index),y:msg.readFloatLE(index+8),z:msg.readFloatLE(index+16)};
}

sub.on('message', function (data) {
   var message = data.toString();
   console.log(message);
   if(data.length != 24){
	   return;
   }
   var pos = readFloat(data);
   var flakes = [{
	   label:'led', 
	   vector3s: [{
		   x: pos.x,
		   y: pos.y,
		   z: pos.z}],
	   }]
	console.log(flakes);
	holojam.Send(holojam.BuildUpdate('vrite', flakes));
})