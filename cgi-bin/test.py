import os
import cgi

def document_response():
    print("Content-Type: text/html\r\n\r\n", end="")
    print("<html><body><h1>Document Response</h1><p>This is a document response.</p></body></html>")

def local_redir_response():
    print("Location: /local/path/to/dummy\r\n\r\n", end="")

def client_redir_response():
    print("Status: 302 Found\r\nLocation: example\r\n\r\n", end="")

def client_redirdoc_response():
    print("Status: 302 Found\r\nLocation: example\r\n\r\n", end="")
    print("<html><body><h1>Redirecting</h1><p>You are being redirected. <a href='dummy_url'>Click here</a> if not redirected.</p></body></html>")

def main():

    form = cgi.FieldStorage()
    response_type = form.getvalue("type")
    request_method = os.environ.get("REQUEST_METHOD")


    if response_type == "document-response":
        document_response()
    elif response_type == "local-redir-response":
        local_redir_response()
    elif response_type == "client-redir-response":
        client_redir_response()
    elif response_type == "client-redirdoc-response":
        client_redirdoc_response()
    else:
        print("Content-Type: text/plain\r\n\r\n", end="")
        print("Unknown response type.")
    print(request_method);

if __name__ == "__main__":
    main()