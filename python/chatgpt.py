import requests

file = open("openai-key")
key = file.readline(164)
file.close()

url = "https://api.openai.com/v1/models/"

headers = {
    "Content-Type": "application/json",
    "Authorization": "Bearer " + key
}

data = {
    "model": "gpt-4o-mini",
    "messages": [{"role": "user", "content": "Say this is a test!"}],
    "temperature": 0.7
}

response = requests.post(url, headers=headers, data=data)
print(response.json())
