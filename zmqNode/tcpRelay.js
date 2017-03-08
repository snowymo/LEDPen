var net = require('net');

var server = net.createServer();  
server.on('connection', handleConnection);

server.listen(12345,'127.0.0.1', function() {  
  console.log('server listening to %j', server.address());
});

function handleConnection(conn) {  
  var remoteAddress = conn.remoteAddress + ':' + conn.remotePort;
  console.log('new client connection from %s', remoteAddress);

  conn.setEncoding('utf8');

  conn.on('data', onConnData);
  conn.once('close', onConnClose);
  conn.on('error', onConnError);

  function onConnData(d) {
    console.log('connection data from %s: %j\n', remoteAddress, d);
	var data = d.toString('ascii',0,d.length);
	console.log("data:",data);
    //conn.write(d.toUpperCase());
  }
  function onConnClose() {

    console.log('connection from %s closed', remoteAddress);
  }

  function onConnError(err) {
    console.log('Connection %s error: %s', remoteAddress, err.message);
  }
}