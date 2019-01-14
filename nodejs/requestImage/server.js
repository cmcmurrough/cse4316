var express = require('express')
var fs = require('fs');
var app = express()

app.get('/', function (req, res) {
  console.log('received a request!')
  res.send('Hello World!')
})

app.listen(8080, function () {
  console.log('Example app listening on port 8080!')
})


// based on example: http://expressjs.com/en/api.html#res.sendFile
app.get('/user/:uid/album/:file', function(request, response){
  var uid = request.params.uid;
  var file = request.params.file;
  var path = __dirname + '/user/' + uid + '/album/' + file;
  
  var isAllowed = true;	// replace with a function that checks to see if the user is authenticated and has permission to access the file
  var fileExists = fs.existsSync(path);
  
  if (!isAllowed) {
    console.log('forbidden')
    response.status(403).send("Forbidden");
  } else if (!fileExists) {
    console.log('file not found')
    response.status(404).send("File not found");
  } else {
    console.log('send file')
    response.sendFile(__dirname + '/user/' + uid + '/album/' + file);
  }
});


