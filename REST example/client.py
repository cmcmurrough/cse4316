import requests

# test HTTP GET method
response = requests.get('http://localhost:3000/')
print("response status code: " + str(response.status_code))
print("response body: " + response.text)

# test HTTP PUT method
response = requests.put('http://localhost:3000/', data={"param": "value"})
print("response status code: " + str(response.status_code))
print("response body: " + response.text)

# test HTTP POST method
response = requests.post('http://localhost:3000/', data={"param": "value"})
print("response status code: " + str(response.status_code))
print("response body: " + response.text)

# test HTTP GET method with JSON endpoint
response = requests.get('http://localhost:3000/json')
json_response = response.json()
print("response status code: " + str(response.status_code))
print("response body: " + str(json_response))

# test HTTP POST method with JSON endpoint
postJSON={'key':'value'}
response = requests.post('http://localhost:3000/json', json=postJSON)
print("response status code: " + str(response.status_code))
print("response body: " + response.text)

# test HTTP POST method with INVALID JSON data
# this should return an error message, as the JSON data is improperly formatted
response = requests.post('http://localhost:3000/json', json="invalid JSON data")
print("response status code: " + str(response.status_code))
print("response body: " + response.text)
