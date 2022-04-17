import http.server
import socketserver
import json
from tokenize import String
from enum import Enum

# Own modules
# import set_grpc_headers

# Open the JSON config file
configFile = open('config.json')
# Read the config. Returns JSON object as a dictionary.
config = json.load(configFile)

# Set config variables from config file
LISTEN_PORT = config["port"]

class GrpcStatus(Enum):
    success = 0
    failed_convert_contract_address = 13
    failed_known_transaction = 13

class MyHttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    def set_grpc_headers(self, status: GrpcStatus, message: String):
        self.send_header("grpc-status", status.value)
        self.send_header("grpc-message", message)
        return

    def get_endpoint_config(self, path: String, body: dict):
        """Tries to get the config for this path and body from the config file
        If the config exists, it returns it
        If the config doesn't exist, it returns null
        """
        mapping = config["mapping"]
        for map in mapping:
            if (map["requestPath"] == path):
                pathMapping = map["pathMapping"]
                for element in pathMapping:
                    if (element["requestBody"] == body):
                        return element
        return None


    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        body = self.rfile.read(content_length)
        jsonBody = json.loads(body.decode('utf-8'))
        endpoint = self.path.split('/')[-1]
        pathMapping = self.get_endpoint_config(endpoint, jsonBody)

        # If the path is not configured, return a BadRequest code (400)
        if pathMapping == None:
            self.send_response(400)
            return

        # Set the response code.
        self.send_response(pathMapping["responseCode"])

        # Serialize the response to a string.
        jsonResponseStr = json.dumps(pathMapping["responseBody"])

        # Set the headers.
        self.send_header("Content-type", "application/json")
        self.set_grpc_headers(GrpcStatus[pathMapping["grpcStatus"].lower()], pathMapping["grpcMessage"])
        self.send_header('Content-length', str(len(jsonResponseStr)))
        self.end_headers()

        # Set the response body.
        self.wfile.write(jsonResponseStr.encode(encoding='utf_8'))

        return

# Create an object of the above class
handler = MyHttpRequestHandler

socketserver.TCPServer.allow_reuse_address = True
with socketserver.TCPServer(("", LISTEN_PORT), handler) as httpd:
    print("Server started at localhost:" + str(LISTEN_PORT))
    httpd.serve_forever()