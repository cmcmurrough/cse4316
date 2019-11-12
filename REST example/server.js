const express = require('express')
const app = express()
const port = 3000

app.use(express.json()) // for parsing application/json
app.use(express.urlencoded({ extended: true })) // for parsing application/x-www-form-urlencoded

// define the default error handler
app.use (function (error, request, response, next){
    console.error("WARNING: error detected...")
    console.error(error.stack)
    response.status(500).send("Error handling request: " + error.message)
});

// handler for a receved HTTP GET request
app.get('/', (request, response) => {
    console.log('request body:', request.body);
    return response.send('Received a GET HTTP request');
});

// handler for a receved HTTP POST request
app.post('/', (request, response) => {
    console.log('request body:', request.body);
    return response.send('Received a POST HTTP request');
});

// handler for a receved HTTP POST request at endpoint /json
app.post('/json', (request, response) => {
    console.log('request body:', request.body);
    // echo back the received JSON data
    response.json(request.body)
});

// handler for a receved HTTP GET request at endpoint /json
app.get('/json', (request, response) => {
    console.log('request body:', request.body);
    // create a JSON message to send
    data = {timestamp: Date.now(), message: "Hello world!" }
    response.json(data)
});

// handler for a receved HTTP PUT request
app.put('/', (request, response) => {
    console.log('request body:', request.body)
    return response.send('Received a PUT HTTP request');
});

// handler for a receved HTTP DELETE request
app.delete('/', (request, response) => {
    console.log('request body:', request.body)
    return response.send('Received a DELETE HTTP request');
});

// handler for application start
app.listen(port, () =>
    console.log("Example app listening on port " + port)
);

