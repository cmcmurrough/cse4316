# send HTTP GET request to endpoint "/"
curl -X GET http://127.0.0.1:3000/ -i

# send HTTP PUT request to endpoint "/"
curl -d "param1=value1&param2=value2" -X PUT http://localhost:3000/ -i

# send HTTP POST request to endpoint "/"
curl -d "param1=value1&param2=value2" -X POST http://localhost:3000/ -i

# POST JSON data to the /json endpoint
curl -d '"key1":"value1", "key2":"value2' -H "Content-Type: application/json}" -X POST http://localhost:3000/json -i

# GET JSON data from the /json endpoint
curl -X GET http://localhost:3000/json -i
