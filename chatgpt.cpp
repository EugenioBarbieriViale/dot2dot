#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>

std::string get_api_key(std::string name_of_file) {
    std::string line;
    std::ifstream file (name_of_file);
    
    if (file.is_open()) {
        getline(file, line);
        file.close();
    }
    else
        std::cout << "Cannot open the file" << '\n';

    return line;
}

int main() {
    CURL *curl;
    curl = curl_easy_init();

    std::string key = get_api_key("openai-key");
    
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Authorization: Bearer " + key);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/engines/davinci/completions");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"prompt\": \"Your prompt here\", \"max_tokens\": 100}");
    }
    else {
        std::cerr << "Unable to send http request" << '\n';
        return -1;
    }

    return 0;
}
