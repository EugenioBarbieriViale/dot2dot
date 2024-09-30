import requests

file = open("openai-key")
key = file.readline(164)
file.close()

url = "https://api.openai.com/v1/chat/completions"

headers = {
    "Content-Type": "application/json",
    "Authorization": "Bearer " + key
}

data = {
    "model": "gpt-3.5-turbo",
    "messages": [{"role": "user", "content": "Say this is a test!"}],
    "temperature": 0
}

response = requests.post(url, headers=headers, json=data)
print(response.json())
