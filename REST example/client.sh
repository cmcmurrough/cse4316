# send HTTP GET request to the /
curl -X GET http://127.0.0.1:3000/ -i

# send JSON data to the /json endpoint
curl -d '{"key1":"value1", "key2":"value2"}' -H "Content-Type: application/json" -X POST http://localhost:3000/json -i

