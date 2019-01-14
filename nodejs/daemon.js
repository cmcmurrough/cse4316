/*******************************************************************************************************************//**
* HTTP Client for posting messages
***********************************************************************************************************************/

// import http module for requests
var http = require('http');

// create the request header and endpoint configuration
var httpPostOptions = {
	host: "127.0.0.1",
	port: 8080,
	path: "/",
	method: "POST",
	headers: {"content-type": "text/plain"},
};

// define a function to handle outgoing HTTP POST messages
function httpPost(string)
{
	// set up the request
	var request = http.request(httpPostOptions);

	// fill the message body and send the request
	request.write(string);
	request.end();

	// handle a transmission failure
	request.on('error', function (res, socket, head) {
		console.log("WARNING: HTTP POST failed!");
  });
}

/*******************************************************************************************************************//**
* Child process spawning and event handling
***********************************************************************************************************************/

// define a function to handle child process stdout events
function childHandleStdout(data)
{
	// handle individual lines or full text blocks
	var separateLines = false;
	if(separateLines)
	{
		// separate full lines of text
		var strings = data.toString().split("\n");

		// iteratively handle each received line of text
		for(var i = 0; i < strings.length; i++)
		{
			// post the data
			httpPost(strings[i]);
		}
	}
	else
	{
		// post the data
		httpPost(data);
	}
}

// define a function to handle child process stderr events
function childHandleStderr(data)
{
	console.log('Child process STDERR: %s \n', data.toString());
}

// define a function to handle child process close events
function childHandleClose(code)
{
	console.log('Child process CLOSE: %s \n', code.toString());
}

// import the child_process module for syscalls
var spawn = require('child_process').spawn;

// spawn the child process
var child_command = "ipconfig";
var child = spawn(child_command);
console.log("Child process launched with process ID %d \n", child.pid);

// link output events to the proper handlers
child.stdout.on('data', childHandleStdout);
child.stderr.on('data', childHandleStderr);
child.on('close', childHandleClose);
