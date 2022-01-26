// import HTTP module
var http = require('http');

// define the listening port
const PORT=8080;

// define a function to handle requests and send a response
function handleRequest(request, response)
{
	// handle all of the chunks of incoming data
	console.log('Receiving message...')
	var body = [];
	request.on('data', function(chunk) {
  	    body.push(chunk);
	}).on('end', function() {
		// assemble the entire message from all of the received chunks
  	    body = Buffer.concat(body).toString();
		console.log(body)

		// send message response
		response.end('Responding to request at path: ' + request.url);
		console.log('Response sent');
	});
}

// define a function to handle server launch
function serverStart()
{
	// callback triggered when server is successfully listening
	console.log("Server listening on: http://localhost:%s", PORT);
}

// create a server
var server = http.createServer(handleRequest);

// start the server
server.listen(PORT, serverStart);
